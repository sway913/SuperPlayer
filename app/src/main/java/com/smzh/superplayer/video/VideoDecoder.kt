package com.smzh.superplayer.video

import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.view.Surface

class VideoDecoder(private val filePath: String,
                   private val outSurface: Surface) {

    private val bufferInfo = MediaCodec.BufferInfo()
    private var isStopped = false
    private var width = 0
    private var height = 0
    private val mediaExtractor: MediaExtractor = MediaExtractor()
    private var mediaCodec: MediaCodec
    private var videoTrackIndex = 0

    init {
        mediaExtractor.setDataSource(filePath)
        val trackCount = mediaExtractor.trackCount
        for (i in 0 until trackCount) {
            val trackerFormat = mediaExtractor.getTrackFormat(i)
            val mine = trackerFormat.getString(MediaFormat.KEY_MIME)
            if (mine.contains("video")) {
                videoTrackIndex = i
                break
            }
        }
        if (videoTrackIndex == -1) {
            mediaExtractor.release()
        }
        mediaExtractor.selectTrack(videoTrackIndex)
        mediaCodec = MediaCodec.createDecoderByType(MediaFormat.MIMETYPE_VIDEO_AVC)
        mediaCodec.configure(null, outSurface, null, 0)
        mediaCodec.start()
    }

    fun drainDecoder() {
        val inputBufferIndex = mediaCodec.dequeueInputBuffer(10000)
        if (inputBufferIndex > 0) {
            val buffer = mediaCodec.getInputBuffer(inputBufferIndex)
            val sampleSize = mediaExtractor.readSampleData(buffer, 0)
            if (sampleSize < 0) {
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM)
            } else {
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, sampleSize, mediaExtractor.sampleTime, 0)
                mediaExtractor.advance()
                val outBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 10000)
                if ((bufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    return
                }
                when (outBufferIndex) {
                    MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED,
                    MediaCodec.INFO_OUTPUT_FORMAT_CHANGED,
                    MediaCodec.INFO_TRY_AGAIN_LATER -> {
                    }
                    else -> {
                        mediaCodec.releaseOutputBuffer(outBufferIndex, true)
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
        mediaExtractor.release()
        mediaCodec.stop()
        mediaCodec.release()
    }

}