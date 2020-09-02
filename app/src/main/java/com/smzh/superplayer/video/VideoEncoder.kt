package com.smzh.superplayer.video

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.media.MediaMuxer
import android.util.Log
import android.view.Surface
import java.io.File

class VideoEncoder(w: Int, h: Int, bitRate: Int, frameRate: Int, outPath: String) : Thread() {

    private lateinit var codec: MediaCodec
    private lateinit var surface: Surface
    private var bufferInfo: MediaCodec.BufferInfo
    private var muxer: MediaMuxer
    private var startTime = -1L
    private var lastPts = 0L


    private var lastPresentationTimeUs: Long = 0

    private var trackIndex = 0
    private var hasStop = false
    private val lock = Object()

    companion object {

        private const val TAG = "VideoEncoder"
        private const val FRAME_INTERVAL = 2 // sync frame every second
        private const val DEFAULT_TIMEOUT = 10 * 1000L
    }


    init {
        val format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, w, h) //H.264 Advanced Video
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface)
        format.setInteger(MediaFormat.KEY_FRAME_RATE, frameRate)
        format.setInteger(MediaFormat.KEY_BIT_RATE, bitRate)
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, FRAME_INTERVAL)
        format.setInteger(MediaFormat.KEY_BITRATE_MODE, MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_VBR)

        try {
            codec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC)
            codec.configure(format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
            surface = codec.createInputSurface()
            codec.start()
            hasStop = false
        } catch (e: Exception) {
            e.printStackTrace()
        }
        val file = File(outPath)
        if (file.exists()) {
            file.delete()
        }
        muxer = MediaMuxer(outPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
        trackIndex = -1
        bufferInfo = MediaCodec.BufferInfo()
        start()
        Log.d(TAG, "encode param width:$w, height: $h, path: $outPath, bitRate: $bitRate, frameRate: $frameRate")
    }

    fun getInputSurface(): Surface {
        return surface
    }

    override fun run() {
        while (!hasStop) {
            synchronized(lock) {
                lock.wait()
                drainEncoder()
            }
        }
    }

    private fun drainEncoder() {
        try {
            var outputIndex = try {
                codec.dequeueOutputBuffer(bufferInfo, DEFAULT_TIMEOUT)
            } catch (e: IllegalStateException) {
                MediaCodec.INFO_TRY_AGAIN_LATER
            }
            var encodeBuffers = codec.outputBuffers

            if (bufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM != 0) {
                Log.i("tclog", "signalInputEnd: BUFFER_FLAG_END_OF_STREAM")
            } else if (outputIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                Log.i(TAG, "no output available yet")
            } else if (outputIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                Log.i(TAG, " not expected for an encoder")
                outputIndex = try {
                    codec.dequeueOutputBuffer(bufferInfo, DEFAULT_TIMEOUT)
                } catch (e: IllegalStateException) {
                    MediaCodec.INFO_TRY_AGAIN_LATER
                }
                encodeBuffers = codec.outputBuffers
            } else if (outputIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                trackIndex = muxer.addTrack(codec.outputFormat)
                muxer.start()
                Log.d(TAG, "encoder output format changed: ${codec.outputFormat}")
            } else if (outputIndex < 0) {
                Log.w(TAG, "unexpected result from encoder.dequeueOutputBuffer: $outputIndex")
            } else {
                val outputBuffer = encodeBuffers[outputIndex] ?: throw RuntimeException("encoderOutputBuffer $outputIndex was null")

                if (bufferInfo.flags and MediaCodec.BUFFER_FLAG_CODEC_CONFIG != 0) {
                    bufferInfo.size = 0
                }

                if (bufferInfo.presentationTimeUs >= lastPresentationTimeUs) {
                    if (bufferInfo.size != 0) {
                        outputBuffer.position(bufferInfo.offset)
                        outputBuffer.limit(bufferInfo.offset + bufferInfo.size)
                        muxer.writeSampleData(trackIndex, outputBuffer, bufferInfo)
                        lastPresentationTimeUs = bufferInfo.presentationTimeUs
                    } else {
                        Log.i(TAG, "why bufferInfo.size is equals 0")
                    }
                } else {
                    Log.i(TAG, "drainEncoder: presentationTimeUs err")
                }
                codec.releaseOutputBuffer(outputIndex, false)
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun signalInputEnd() {
        synchronized(lock) {
            if (hasStop) {
                Log.i(TAG, "signalInputEnd")
                try {
                    codec.signalEndOfInputStream()
                } catch (e: java.lang.Exception) {
                    Log.i(TAG, "Exception = $e")
                }
            }
        }
    }

    fun encodeFrame() {
        synchronized(lock) {
            lock.notifyAll()
        }
    }

    private fun getPts(): Long {
        var pts: Long
        if (startTime == -1L) {
            startTime = System.nanoTime()
            pts = 0
        } else {
            pts = (System.nanoTime() - startTime) / 1000
        }
        if (pts <= lastPts) {
            pts += lastPts - pts + 1000
        }
        return pts
    }

    fun stopEncoder() {
        try {
            hasStop = true
            encodeFrame()
            join()
            signalInputEnd()
            codec.stop()
            codec.release()
            muxer.stop()
            muxer.release()
        } catch (e: Exception) {

        }
    }
}