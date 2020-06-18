package com.smzh.superplayer.sing

import android.os.Handler
import android.os.Looper
import android.text.format.DateUtils
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.App
import com.smzh.superplayer.isHeadSetOn
import com.smzh.superplayer.player.AudioParam
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer
import com.smzh.superplayer.player.SuperPlayer.Tracker

class SingViewModel(val song: Song) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }

    val singState = MutableLiveData<Int>()
    val singPercent = MutableLiveData<String>()
    val progress = MutableLiveData<Int>()
    val totalMs = MutableLiveData<Long>()
    val currentMs = MutableLiveData<Long>()
    val songName = MutableLiveData<String>()
    val singComplete = MutableLiveData<Boolean>()
    val handler = Handler(Looper.getMainLooper())

    private val runnable = object : Runnable {
        override fun run() {
            var total = totalMs.value!!
            if (total <= 0) {
                total = 1L
            }
            currentMs.value = player.getCurrentMs()
            progress.value = (currentMs.value!! * 100f / total).toInt()
            singPercent.value = "正在录制: ${DateUtils.formatElapsedTime(currentMs.value!! / 1000)} /${DateUtils.formatElapsedTime(total / 1000)}"
            handler.postDelayed(this, 20)
        }
    }

    init {
        singState.value = STATE_IDLE
        songName.value = song.name
        singPercent.value = "准备就绪"
    }

    fun prepare() {
        val audioParam = AudioParam(isRecorder = true,
                accPath = song.path ?: "",
                vocalPath = SingParam.vocalPath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
        singState.postValue(STATE_PREPARE)
    }

    override fun onPrepared() {
        start()
        setVocalVolume(0.5f)
        player.setEcho(isHeadSetOn(App.context))
        totalMs.postValue(player.getTotalMs())
        handler.postDelayed(runnable, 20)
    }

    fun start() {
        player.start()
        singState.postValue(STATE_SING)
    }

    fun resume() {
        player.resume()
        handler.post(runnable)
        singState.postValue(STATE_SING)
    }

    fun pause() {
        player.pause()
        handler.removeCallbacks(runnable)
        singState.postValue(STATE_PAUSE)
    }

    fun seek(ms: Long) {
        player.seek(ms)
    }

    fun stop() {
        player.stop()
        player.removePlayerListener(this)
        handler.removeCallbacksAndMessages(null)
        singState.postValue(STATE_STOP)
    }

    override fun onError() {
        player.stop()
        prepare()
    }

    override fun onCompleted() {
        singComplete.postValue(true)
    }

    fun setVocalVolume(vol: Float) {
        player.setVolume(vol * SingParam.vocalGain, Tracker.VOCAL)
    }

    fun setAccVolume(vol: Float) {
        player.setVolume(vol, Tracker.ACC)
    }

    fun setPitch(pitch: Float) {
        player.setPitch(pitch)
    }


    @Suppress("UNCHECKED_CAST")
    class SingFactory(val song: Song) : ViewModelProvider.NewInstanceFactory() {

        override fun <T : ViewModel?> create(modelClass: Class<T>): T {
            return SingViewModel(song) as T
        }
    }

    companion object {
        const val STATE_IDLE = 0
        const val STATE_PREPARE = 1
        const val STATE_SING = 2
        const val STATE_PAUSE = 3
        const val STATE_STOP = 4
    }

}