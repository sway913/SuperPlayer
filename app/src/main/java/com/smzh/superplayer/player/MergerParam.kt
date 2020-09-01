package com.smzh.superplayer.player

class MergerParam(private val mode: Int,
                  private val isVideo: Boolean,
                  private val aacPath: String,
                  private val vocalPath: String,
                  private val videoPath: String,
                  private val outPath: String,
                  private val outVideoPath: String,
                  private val vocalVolume: Float,
                  private val accVolume: Float,
                  private val pitch: Float,
                  private val effect: FloatArray) {


    fun getAccPath(): String {
        return aacPath
    }

    fun getVocalPath(): String {
        return vocalPath
    }

    fun getOutPath(): String {
        return outPath
    }

    fun getVocalVolume(): Float {
        return vocalVolume
    }

    fun getAccVolume(): Float {
        return accVolume
    }

    fun getPitch(): Float {
        return pitch
    }

    fun getEffect(): FloatArray {
        return effect
    }

    fun getMode(): Int {
        return mode
    }

    fun getVideoPath(): String {
        return videoPath
    }

    fun isVideo(): Boolean {
        return isVideo
    }

    fun getOutVideoPath(): String {
        return outVideoPath
    }
}