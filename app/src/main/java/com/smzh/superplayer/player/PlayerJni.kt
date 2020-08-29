package com.smzh.superplayer.player

import android.view.Surface
import com.smzh.superplayer.video.GLView
import java.util.*

class PlayerJni {

    companion object {

        init {
            System.loadLibrary("superplayer")
        }

        val listeners = Vector<PlayerStateListener>()
    }


    //声明周期方法请按顺序调用
    external fun prepare(audioParam: AudioParam)

    external fun start()

    external fun resume()

    external fun pause()

    external fun stop()

    external fun seek(millis: Long)

    external fun setEcho(isEcho: Boolean)

    external fun setFilter(floatArray: FloatArray)

    external fun setVolume(volume: Float, track: Int)

    external fun setPitch(pitch: Float)

    external fun getTotalMs(): Long

    external fun getCurrentMs(): Long

    external fun getRealMs(): Long

    external fun startMerge(mergerParam: MergerParam)

    external fun getMergeProgress(): Int

    external fun onSurfaceCreate(surface: Surface, width: Int, height: Int, mode: Int)

    external fun onSurfaceDestroy()

    external fun onFrameAvailable()

    external fun switchCamera()

    external fun setVideoEffect(videoEffect: VideoEffect)

    fun setPlayerListener(listener: PlayerStateListener) {
        if (!listeners.contains(listener)) {
            listeners.add(listener)
        }
    }

    fun removePlayerStateListener(listener: PlayerStateListener) {
        listeners.remove(listener)
    }


    fun onPrepared() {
        for (listener in listeners) {
            listener.onPrepared()
        }
    }

    fun onCompleted() {
        for (listener in listeners) {
            listener.onCompleted()
        }
    }

    fun onError() {
        for (listener in listeners) {
            listener.onError()
        }
    }

    interface PlayerStateListener {

        fun onPrepared()

        fun onCompleted()

        fun onError()

    }
}
