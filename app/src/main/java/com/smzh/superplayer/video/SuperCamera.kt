package com.smzh.superplayer.video


open class SuperCamera() {

    open fun open() {

    }

    open fun close() {

    }

    open fun updateImage() {

    }

    open fun switchCamera() {

    }

    open fun isFront(): Boolean {
        return true
    }

    open fun startPreview(textureId: Int) {

    }

    companion object {

        @JvmStatic
        fun getCamera(version: Int): SuperCamera {
            return Camera1()
        }
    }
}