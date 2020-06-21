package com.smzh.superplayer.player

class MergerParam(private val aacPath: String,
                  private val vocalPath: String,
                  private val outPath: String,
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
}