package com.smzh.superplayer.video

import android.media.*
import android.util.Log
import java.nio.ByteBuffer
import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

class AudioEncoder : Thread() {


    private lateinit var mMediaCodec: MediaCodec
    private lateinit var mBufferInfo: MediaCodec.BufferInfo
    private lateinit var mAudioFormat: MediaFormat
    private lateinit var mMediaMuxer: MediaMuxer
    private var mCurrentTrackIndex = 0
    private val lock = ReentrantLock()
    private val cond = lock.newCondition()
    private var hasStop = false


    fun init(aacPath: String, sampleRate: Int, channels: Int) {
        mBufferInfo = MediaCodec.BufferInfo()
        mMediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_AUDIO_AAC)
        mAudioFormat = MediaFormat.createAudioFormat(MediaFormat.MIMETYPE_AUDIO_AAC, sampleRate, channels)
        mAudioFormat.setInteger(MediaFormat.KEY_BIT_RATE, 64000)
        mAudioFormat.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC)
        mAudioFormat.setInteger(MediaFormat.KEY_CHANNEL_MASK, if (channels == 1) AudioFormat.CHANNEL_IN_MONO else AudioFormat.CHANNEL_IN_STEREO)
        mMediaCodec.configure(mAudioFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
        mMediaCodec.start()
        start()
        mMediaMuxer = MediaMuxer(aacPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
    }

    override fun run() {
        while (!hasStop) {
            lock.withLock {
                cond.await()
                drainEncode()
            }
        }
    }


    fun encodeFrame(data: ByteArray, timestamp: Long) {
        lock.withLock {
            val inputBufferIndex = mMediaCodec.dequeueInputBuffer(10000)
            if (inputBufferIndex >= 0) {
                val inputBuffer = mMediaCodec.getInputBuffer(inputBufferIndex)
                inputBuffer?.clear()
                inputBuffer?.put(data)
                if (data.isEmpty()) {
                    mMediaCodec.queueInputBuffer(inputBufferIndex, 0, 0, timestamp, MediaCodec.BUFFER_FLAG_END_OF_STREAM)
                    return
                } else {
                    mMediaCodec.queueInputBuffer(inputBufferIndex, 0, data.size, timestamp, 0)
                }
            } else if (inputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                Log.d("INFO_TRY_AGAIN_LATER", "")
            }
            cond.signalAll()
        }
    }

    private fun drainEncode() {
        var encoderOutputBuffers = mMediaCodec.outputBuffers
        var outBufferIndex = MediaCodec.INFO_TRY_AGAIN_LATER
        try {
            outBufferIndex = mMediaCodec.dequeueOutputBuffer(mBufferInfo, 0)
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }

        if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM != 0) {
            try {
                mMediaCodec.signalEndOfInputStream()
            } catch (e: Exception) {

            }
            Log.i("tclog", "signalInputEnd: BUFFER_FLAG_END_OF_STREAM")
        } else if (outBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {

        } else if (outBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
            outBufferIndex = try {
                mMediaCodec.dequeueOutputBuffer(mBufferInfo, 0)
            } catch (e: IllegalStateException) {
                MediaCodec.INFO_TRY_AGAIN_LATER
            }
            encoderOutputBuffers = mMediaCodec.outputBuffers
        } else if (outBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
            mCurrentTrackIndex = mMediaMuxer.addTrack(mMediaCodec.outputFormat)
            mMediaMuxer.start()
        } else if (outBufferIndex < 0) {
            Log.e("AAC_ENCODER", "unexpected result from encoder.dequeueOutputBuffer: $outBufferIndex")
        } else {
            val encodedData = encoderOutputBuffers[outBufferIndex] ?: throw RuntimeException("encoderOutputBuffer " + outBufferIndex + " was null")
            if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_CODEC_CONFIG != 0) {
                mBufferInfo.size = 0
            }
            if (mBufferInfo.size > 0) {
                encodedData.position(mBufferInfo.offset)
                encodedData.limit(mBufferInfo.offset + mBufferInfo.size)
                mMediaMuxer.writeSampleData(mCurrentTrackIndex, encodedData, mBufferInfo)
            }
//            Log.d("aac encode timestamp", "out +: " + mBufferInfo.presentationTimeUs)
            mMediaCodec.releaseOutputBuffer(outBufferIndex, false)
        }
    }

    fun endEncode() {
        lock.withLock {
            hasStop = true
            cond.signalAll()
        }
        join()
        try {
            mMediaCodec.signalEndOfInputStream()
        } catch (e: Exception) {

        } finally {
            mMediaCodec.stop()
            mMediaCodec.release()
            mMediaMuxer.stop()
            mMediaMuxer.release()
        }
    }
}