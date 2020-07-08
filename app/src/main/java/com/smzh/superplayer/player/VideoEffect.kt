package com.smzh.superplayer.player

class VideoEffect(private var whiteLevel: Float,
                  private var smoothLevel: Float,
                  private var lookupTablePath: String?) {

    fun getWhiteLevel(): Float {
        return whiteLevel
    }

    fun getSmoothLevel(): Float {
        return smoothLevel
    }

    fun getLookupTablePath(): String {
        return lookupTablePath ?: ""
    }


    fun setWhiteLevel(level: Float) {
        this.whiteLevel = level
    }

    fun setSmoothLevel(level: Float) {
        this.smoothLevel = level
    }

    fun setLookupTablePath(path: String?) {
        this.lookupTablePath = path
    }

}