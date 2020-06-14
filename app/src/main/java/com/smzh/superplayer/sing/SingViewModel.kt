package com.smzh.superplayer.sing

import android.os.Handler
import android.os.Looper
import android.text.format.DateUtils
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.player.AudioParam
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer

class SingViewModel(val song: Song) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }

    val singState = MutableLiveData<Int>()
    val singPercent = MutableLiveData<String>()
    val progress = MutableLiveData<Int>()
    val songName = MutableLiveData<String>()
    val singComplete = MutableLiveData<Boolean>()
    val handler = Handler(Looper.getMainLooper())

    private val runnable = object : Runnable {
        override fun run() {
            var total = player.getTotalMs()
            if (total <= 0) {
                total = 1L
            }
            progress.value = (player.getCurrentMs() * 100f / total).toInt()
            singPercent.value = "正在录制: ${DateUtils.formatElapsedTime(player.getCurrentMs() / 1000)} /${DateUtils.formatElapsedTime(total / 1000)}"
            handler.postDelayed(this, 20)
        }
    }

    init {
        singState.value = 0
        songName.value = song.name
        singPercent.value = "准备就绪"
    }

    fun prepare() {
        val audioParam = AudioParam(isRecorder = true,
                accPath = song.path ?: "",
                vocalPath = SingParam.vocalPath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
    }

    override fun onPrepared() {
        singState.postValue(1)
        start()
        handler.postDelayed(runnable, 20)
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
        handler.removeCallbacksAndMessages(null)
    }

    override fun onError() {

    }

    override fun onCompleted() {
        singComplete.postValue(true)
    }

    @Suppress("UNCHECKED_CAST")
    class SingFactory(val song: Song) : ViewModelProvider.NewInstanceFactory() {

        override fun <T : ViewModel?> create(modelClass: Class<T>): T {
            return SingViewModel(song) as T
        }
    }

}