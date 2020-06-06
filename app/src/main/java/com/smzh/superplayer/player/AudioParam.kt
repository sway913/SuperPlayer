package com.smzh.superplayer.player

class AudioParam(private val isRecorder: Boolean,
                 private val vocalPath: String,
                 private val accPath: String,
                 private val functionPath: String) {

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

    fun getFunctionPath(): String {
        return functionPath
    }

}
