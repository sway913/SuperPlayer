package com.smzh.superplayer.player

class MergerParam(private val aacPath: String = "",
                  private val vocalPath: String = "",
                  private val outPath: String = "") {


    fun getAccPath(): String {
        return aacPath
    }

    fun getVocalPath(): String {
        return vocalPath
    }

    fun getOutPath(): String {
        return outPath
    }

}