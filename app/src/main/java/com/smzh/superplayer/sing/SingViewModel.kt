package com.smzh.superplayer.sing

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.player.AudioParam
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer

class SingViewModel(private val accPath: String) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }


    fun prepare() {
        val audioParam = AudioParam(isRecorder = true,
                accPath = accPath,
                guidePath = "",
                vocalPath = SingParam.vocalPath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
    }

    fun start() {
        player.start()
    }

    fun resume() {

    }

    fun pause() {

    }

    fun stop() {
        player.stop()
        player.removePlayerListener(this)
    }


    override fun onPrepared() {
        start()
    }


    override fun onError() {

    }

    override fun onCompleted() {

    }

    @Suppress("UNCHECKED_CAST")
    class SingFactory(private val accPath: String) : ViewModelProvider.NewInstanceFactory() {

        override fun <T : ViewModel?> create(modelClass: Class<T>): T {
            return SingViewModel(accPath) as T
        }
    }

}