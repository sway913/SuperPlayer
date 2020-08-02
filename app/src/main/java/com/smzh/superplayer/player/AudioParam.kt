package com.smzh.superplayer.player

class AudioParam(private val isRecorder: Boolean = false,
                 private val vocalPath: String = "",
                 private val accPath: String = "",
                 private val guidePath: String = "",
                 private val decodePath: String = "",
                 private val videoPath: String = "",
                 private val withVideo: Boolean = false) {

    private var outSample: Int = 0

    fun setOutSample(sample: Int) {
        this.outSample = sample
    }

    fun getOutSample(): Int {
        return outSample;
    }

    fun isRecorder(): Boolean {
        return isRecorder;
    }

    fun getAccPath(): String {
        return accPath
    }

    fun getVocalPath(): String {
        return vocalPath
    }

    fun getDecodePath(): String {
        return decodePath
    }

    fun getGuidePath(): String {
        return guidePath;
    }

    fun isVideoMode(): Boolean {
        return withVideo
    }

    fun getVideoPath(): String {
        return videoPath;
    }

}
