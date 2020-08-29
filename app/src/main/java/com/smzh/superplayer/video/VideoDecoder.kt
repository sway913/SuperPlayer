package com.smzh.superplayer.video

import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.util.Log
import android.view.Surface
import com.smzh.superplayer.player.SuperPlayer

class VideoDecoder : Thread() {

    private val bufferInfo = MediaCodec.BufferInfo()
    private var width = 0
    private var height = 0
    private val mediaExtractor: MediaExtractor = MediaExtractor()
    private lateinit var mediaCodec: MediaCodec
    private var videoTrackIndex = 0
    private var duration = 0L
    private var lastFrameTime = 0L
    private var hasStop = false

    fun init(filePath: String, outSurface: Surface) {
        mediaExtractor.setDataSource(filePath)
        val trackCount = mediaExtractor.trackCount
        for (i in 0 until trackCount) {
            val trackerFormat = mediaExtractor.getTrackFormat(i)
            val mine = trackerFormat.getString(MediaFormat.KEY_MIME)
            if (mine.startsWith("video/")) {
                videoTrackIndex = i
                break
            }
        }
        if (videoTrackIndex < 0) {
            mediaExtractor.release()
            Log.d("Video decoder", " can not find video track")
            return
        }
        mediaExtractor.selectTrack(videoTrackIndex)


        val mediaFormat = mediaExtractor.getTrackFormat(videoTrackIndex)
        val mime: String = mediaFormat.getString(MediaFormat.KEY_MIME)
        duration = mediaFormat.getLong(MediaFormat.KEY_DURATION)

        mediaCodec = MediaCodec.createDecoderByType(mime)
        mediaCodec.configure(mediaFormat, outSurface, null, 0)
        mediaCodec.start()
        Log.d("Video Decode duration :", duration.toString())
        hasStop = false
        start()
    }

    override fun run() {

        if (lastFrameTime == 0L) {
            lastFrameTime = System.currentTimeMillis()
        }
        while (!hasStop) {
            drainDecoder(SuperPlayer.instance.getRealMs())
        }
    }

    private fun drainDecoder(currentMs: Long) {
        if (currentMs > duration) {
            return
        }

        while (true) {
            val inputBufferIndex = mediaCodec.dequeueInputBuffer(10000)
            if (inputBufferIndex > 0) {
                val buffer = mediaCodec.getInputBuffer(inputBufferIndex)
                val sampleSize = mediaExtractor.readSampleData(buffer!!, 0)
                if (sampleSize < 0) {
                    mediaCodec.queueInputBuffer(inputBufferIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM)
                    return
                } else {
                    val presentationTimeUs = mediaExtractor.sampleTime
                    mediaCodec.queueInputBuffer(inputBufferIndex, 0, sampleSize, presentationTimeUs, 0)
                    val outBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 10000)
                    if ((bufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                        return
                    }

                    if (outBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED
                            || outBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED
                            || outBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                        Log.d("Video Decode", "decode useless")
                    } else {

                        val diff = bufferInfo.presentationTimeUs / 1000f + (System.currentTimeMillis() - lastFrameTime) - currentMs
                        if (diff > 30) {
                            sleep(diff.toLong())
                        } else if (diff < -100) {
                            mediaExtractor.advance()
                            mediaCodec.releaseOutputBuffer(outBufferIndex, false)
                            continue
                        }
                        mediaExtractor.advance()
                        mediaCodec.releaseOutputBuffer(outBufferIndex, true)
                        lastFrameTime = System.currentTimeMillis()
                        return
                    }
                }
            }
        }
    }

    fun getWidth(): Int {
        return width
    }

    fun getHeight(): Int {
        return height
    }

    fun seek(ms: Long) {
        mediaExtractor.seekTo(ms, MediaExtractor.SEEK_TO_PREVIOUS_SYNC)
    }

    fun release() {
        hasStop = true
        join()
        try {
            mediaCodec.stop()
            mediaCodec.release()
            mediaExtractor.release()
        } catch (e: Exception) {
        }

    }

}