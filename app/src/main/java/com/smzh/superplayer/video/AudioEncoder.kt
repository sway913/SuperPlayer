package com.smzh.superplayer.video

import android.media.*
import android.util.Log
import java.nio.ByteBuffer

class AudioEncoder {


    private lateinit var mMediaCodec: MediaCodec
    private lateinit var mBufferInfo: MediaCodec.BufferInfo
    private lateinit var mAudioFormat: MediaFormat
    private lateinit var mMediaMuxer: MediaMuxer
    private var mCurrentTrackIndex = 0


    fun init(aacPath: String, sampleRate: Int, channels: Int) {
        mBufferInfo = MediaCodec.BufferInfo()
        mMediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_AUDIO_AAC)
        mAudioFormat = MediaFormat.createAudioFormat(MediaFormat.MIMETYPE_AUDIO_AAC, sampleRate, channels)
        mAudioFormat.setInteger(MediaFormat.KEY_BIT_RATE, 64000)
        mAudioFormat.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC)
        mAudioFormat.setInteger(MediaFormat.KEY_CHANNEL_MASK, if (channels == 1) AudioFormat.CHANNEL_IN_MONO else AudioFormat.CHANNEL_IN_STEREO)
        mMediaCodec.configure(mAudioFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
        mMediaCodec.start()

        mMediaMuxer = MediaMuxer(aacPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
    }

    fun encodeFrame(data: ByteArray, timestamp: Long) {
        if (data.isEmpty()) {
            return
        }
        val inputBufferIndex = mMediaCodec.dequeueInputBuffer(1000)
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


        var encoderOutputBuffers: Array<ByteBuffer?>
        encoderOutputBuffers = try {
            mMediaCodec.outputBuffers
        } catch (e: IllegalStateException) {
            return
        }
        var encoderStatus = MediaCodec.INFO_TRY_AGAIN_LATER
        try {
            encoderStatus = mMediaCodec.dequeueOutputBuffer(mBufferInfo, 10000)
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }

        if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
            return
        } else if (encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
            encoderOutputBuffers = mMediaCodec.outputBuffers
        } else if (encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
            mCurrentTrackIndex = mMediaMuxer.addTrack(mMediaCodec.outputFormat)
            mMediaMuxer.start()
        } else if (encoderStatus < 0) {
            Log.e("AAC_ENCODER", "unexpected result from encoder.dequeueOutputBuffer: $encoderStatus")
        } else {
            val encodedData = encoderOutputBuffers[encoderStatus] ?: throw RuntimeException("encoderOutputBuffer " + encoderStatus + " was null")
            if (mBufferInfo.size > 0) {
                mBufferInfo.presentationTimeUs = timestamp
                encodedData.position(mBufferInfo.offset)
                encodedData.limit(mBufferInfo.offset + mBufferInfo.size)
                mMediaMuxer.writeSampleData(mCurrentTrackIndex, encodedData, mBufferInfo)
            }
            mMediaCodec.releaseOutputBuffer(encoderStatus, false)
            if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM != 0) {
                return
            }
        }
    }

    fun endEncode() {
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