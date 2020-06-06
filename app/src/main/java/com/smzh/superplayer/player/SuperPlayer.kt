package com.smzh.superplayer.player;

import android.content.Context
import android.media.AudioManager
import android.os.Handler
import android.os.HandlerThread
import com.smzh.superplayer.App

class SuperPlayer {

    private var handler: Handler
    private var playerJni: PlayerJni
    private val outSample: Int

    companion object {
        val instance = SingletonHolder.holder
    }

    private object SingletonHolder {
        val holder = SuperPlayer()
    }

    init {
        val playerThread = HandlerThread("player_thread");
        playerThread.start();
        handler = Handler(playerThread.looper);
        playerJni = PlayerJni()
        val myAudioMgr = App.context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
        val sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)
        outSample = sampleRateStr.toInt()
    }

    fun prepare(audioParam: AudioParam) {
        audioParam.setOutSample(outSample)
        handler.post { playerJni.prepare(audioParam) }
    }

    fun start() {
        handler.post { playerJni.start() }
    }

    fun resume() {
        handler.post { playerJni.resume() }
    }

    fun pause() {
        handler.post { playerJni.pause() }
    }

    fun stop() {
        handler.post { playerJni.stop() }
    }


    fun seek(ms: Long) {
        handler.post { playerJni.seek(ms) }
    }

    fun setEcho(isEcho: Boolean) {
        handler.post { playerJni.setEcho(isEcho) }
    }


    fun setFilter(type: Int) {
        handler.post { playerJni.setFilter(type) }
    }

    fun setVolume(volume: Float, track: Int) {
        handler.post { playerJni.setVolume(volume, track) }
    }

    fun setPitch(pitch: Int) {
        handler.post { playerJni.setPitch(pitch) }
    }

    fun startMerge() {
        handler.post { startMerge() }
    }

    fun stopMerge() {
        handler.post { playerJni.stopMerge() }
    }

    fun getTotalMs(): Long {
        return playerJni.getTotalMs()
    }

    fun getCurrentMs(): Long {
        return playerJni.getCurrentMs()
    }

    fun getMergeProgress(): Int {
        return playerJni.getMergeProgress()
    }

    fun addPlayerListener(listener: PlayerJni.PlayerStateListener) {
        playerJni.setPlayerListener(listener)
    }

    fun removePlayerListener(listener: PlayerJni.PlayerStateListener) {
        playerJni.removePlayerStateListener(listener)
    }
}
