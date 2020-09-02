package com.smzh.superplayer.video

import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.media.MediaMuxer
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import java.io.IOException
import java.nio.ByteBuffer
import kotlin.math.min


class H264_AAC_toMp4_MediaMuxer {

    private val TAG = "H264_AAC_toMp4"

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)
    private fun muxerAudio(aacPath: String, tmpAacPath: String) {
        val audioMuxer: MediaMuxer
        val mediaExtractor: MediaExtractor
        try {
            mediaExtractor = MediaExtractor()
            audioMuxer = MediaMuxer(tmpAacPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
            mediaExtractor.setDataSource(aacPath)
            val trackCount = mediaExtractor.trackCount
            var audioTrackIndex = -1
            for (i in 0 until trackCount) {
                val trackFormat = mediaExtractor.getTrackFormat(i)
                val mineType = trackFormat.getString(MediaFormat.KEY_MIME)
                //音频信道
                if (mineType.startsWith("audio/")) {
                    audioTrackIndex = i
                }
            }
            val byteBuffer = ByteBuffer.allocate(500 * 1024)
            val bufferInfo = MediaCodec.BufferInfo()
            //切换到音频信道
            mediaExtractor.selectTrack(audioTrackIndex)
            val trackFormat = mediaExtractor.getTrackFormat(audioTrackIndex)
            val writeAudioIndex = audioMuxer.addTrack(trackFormat)
            audioMuxer.start()
            val sampletime: Long
            val first_sampletime: Long
            val second_sampletime: Long
            mediaExtractor.readSampleData(byteBuffer, 0)
            first_sampletime = mediaExtractor.sampleTime
            mediaExtractor.advance()
            second_sampletime = mediaExtractor.sampleTime
            sampletime = Math.abs(second_sampletime - first_sampletime) //时间戳

            mediaExtractor.unselectTrack(audioTrackIndex)
            mediaExtractor.selectTrack(audioTrackIndex)
            while (true) {
                val readSampleCount = mediaExtractor.readSampleData(byteBuffer, 0)
                Log.d(TAG, "audio:readSampleCount:$readSampleCount")
                if (readSampleCount < 0) {
                    break
                }
                bufferInfo.size = readSampleCount
                bufferInfo.offset = 0
                bufferInfo.flags = mediaExtractor.sampleFlags
                bufferInfo.presentationTimeUs += sampletime
                audioMuxer.writeSampleData(writeAudioIndex, byteBuffer, bufferInfo)
                byteBuffer.clear()
                mediaExtractor.advance()
            }
            Log.d(TAG, "muxerAudio finished!\n")
            audioMuxer.stop()
            audioMuxer.release()
            mediaExtractor.release()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }

    /**
     * h264 复用 成 mp4（无音频）
     */
    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)
    private fun muxerVideo(h264Path: String, tmpH264Path: String) {
        val videoMuxer: MediaMuxer
        val mediaExtractor: MediaExtractor
        try {
            mediaExtractor = MediaExtractor()
            videoMuxer = MediaMuxer(tmpH264Path, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
            mediaExtractor.setDataSource(h264Path)
            val trackCount = mediaExtractor.trackCount
            var videoTrackIndex = -1
            for (i in 0 until trackCount) {
                val trackFormat = mediaExtractor.getTrackFormat(i)
                val mineType = trackFormat.getString(MediaFormat.KEY_MIME)
                //音频信道
                if (mineType.startsWith("video/")) {
                    videoTrackIndex = i
                }
            }
            val byteBuffer = ByteBuffer.allocate(500 * 1024)
            val bufferInfo = MediaCodec.BufferInfo()
            //切换到音频信道
            mediaExtractor.selectTrack(videoTrackIndex)
            val trackFormat = mediaExtractor.getTrackFormat(videoTrackIndex)
            val writeVideoIndex = videoMuxer.addTrack(trackFormat)
            videoMuxer.start()
            var sampletime: Long
            var first_sampletime: Long
            var second_sampletime: Long
            run {
                mediaExtractor.readSampleData(byteBuffer, 0)
                first_sampletime = mediaExtractor.sampleTime
                mediaExtractor.advance()
                second_sampletime = mediaExtractor.sampleTime
                sampletime = Math.abs(second_sampletime - first_sampletime) //时间戳
            }
            //上面只是获取时间戳，获取完后，重新选择下track
            mediaExtractor.unselectTrack(videoTrackIndex)
            mediaExtractor.selectTrack(videoTrackIndex)
            while (true) {
                val readSampleCount = mediaExtractor.readSampleData(byteBuffer, 0)
                Log.d(TAG, "audio:readSampleCount:$readSampleCount")
                if (readSampleCount < 0) {
                    break
                }
                bufferInfo.size = readSampleCount
                bufferInfo.offset = 0
                bufferInfo.flags = mediaExtractor.sampleFlags
                bufferInfo.presentationTimeUs += sampletime
                videoMuxer.writeSampleData(writeVideoIndex, byteBuffer, bufferInfo)
                byteBuffer.clear()
                mediaExtractor.advance()
            }
            Log.d(TAG, "muxerVideo finished!\n")
            videoMuxer.stop()
            videoMuxer.release()
            mediaExtractor.release()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }


    /**
     * 合并上面生成的aac（mp4容器） 和 h264（mp4容器）
     *
     * @param outPath
     */
    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)
    fun mergeVideo(aacPath: String, h264Path: String, outPath: String) {
        val mediaMuxer: MediaMuxer
        val videoExtractor: MediaExtractor
        val audioExtractor: MediaExtractor
        var minDuration: Long = 0
        try {
            audioExtractor = MediaExtractor()
            videoExtractor = MediaExtractor()
            mediaMuxer = MediaMuxer(outPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
            videoExtractor.setDataSource(h264Path)
            audioExtractor.setDataSource(aacPath)
            val videoTrackCount = videoExtractor.trackCount
            val audioTrackCount = audioExtractor.trackCount
            var videoTrackIndex = -1
            var audioTrackIndex = -1
            var maxVideoSize = 0
            var maxAudioSize = 0

            for (i in 0 until videoTrackCount) {
                val videoFormat = videoExtractor.getTrackFormat(i)
                val mineType = videoFormat.getString(MediaFormat.KEY_MIME)
                //视频信道
                if (mineType.startsWith("video/")) {
                    videoTrackIndex = i
                    minDuration = videoFormat.getLong(MediaFormat.KEY_DURATION)
                    maxVideoSize = videoFormat.getInteger(MediaFormat.KEY_MAX_INPUT_SIZE)
                    break
                }
            }
            for (i in 0 until audioTrackCount) {
                val audioFormat = audioExtractor.getTrackFormat(i)
                val mineType = audioFormat.getString(MediaFormat.KEY_MIME)
                if (mineType.startsWith("audio/")) {
                    audioTrackIndex = i
                    minDuration = min(minDuration, audioFormat.getLong(MediaFormat.KEY_DURATION))
                    maxAudioSize = audioFormat.getInteger(MediaFormat.KEY_MAX_INPUT_SIZE)
                    break
                }
            }

            val audiobufferInfo = MediaCodec.BufferInfo()
            val videobufferInfo = MediaCodec.BufferInfo()
            videoExtractor.selectTrack(videoTrackIndex)
            audioExtractor.selectTrack(audioTrackIndex)
            val videotrackFormat = videoExtractor.getTrackFormat(videoTrackIndex)
            val writeVideoIndex = mediaMuxer.addTrack(videotrackFormat)
            val audiotrackFormat = audioExtractor.getTrackFormat(audioTrackIndex)
            val writeAudioIndex = mediaMuxer.addTrack(audiotrackFormat)
            mediaMuxer.start()
            //video

            val videoByteBuffer = ByteBuffer.allocate(maxVideoSize)
            while (true) {
                val readSampleCount = videoExtractor.readSampleData(videoByteBuffer, 0)
                Log.d(TAG, "audio:readSampleCount:$readSampleCount")
                if (readSampleCount < 0) {
                    videoExtractor.unselectTrack(videoTrackIndex)
                    break
                }
                videobufferInfo.size = readSampleCount
                videobufferInfo.offset = 0
                videobufferInfo.flags = videoExtractor.sampleFlags
                videobufferInfo.presentationTimeUs = videoExtractor.sampleTime
//                Log.d("video muxer timestamp", videobufferInfo.presentationTimeUs.toString())
                mediaMuxer.writeSampleData(writeVideoIndex, videoByteBuffer, videobufferInfo)
                videoExtractor.advance()
                if (videobufferInfo.presentationTimeUs > minDuration) {
                    break
                }
            }
            //audio
            val audioByteBuffer = ByteBuffer.allocate(maxAudioSize)
            while (true) {
                val readSampleCount = audioExtractor.readSampleData(audioByteBuffer, 0)
                Log.d(TAG, "audio:readSampleCount:$readSampleCount")
                if (readSampleCount < 0) {
                    break
                }
                audiobufferInfo.size = readSampleCount
                audiobufferInfo.offset = 0
                audiobufferInfo.flags = audioExtractor.sampleFlags
                audiobufferInfo.presentationTimeUs = audioExtractor.sampleTime
//                Log.d("audio muxer timestamp", audiobufferInfo.presentationTimeUs.toString())
                mediaMuxer.writeSampleData(writeAudioIndex, audioByteBuffer, audiobufferInfo)
                audioExtractor.advance()
                if (audiobufferInfo.presentationTimeUs >= minDuration) {
                    break
                }
            }
            Log.d(TAG, "combineVideo finished!\n")
            mediaMuxer.stop()
            mediaMuxer.release()
            audioExtractor.release()
            videoExtractor.release()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }
}