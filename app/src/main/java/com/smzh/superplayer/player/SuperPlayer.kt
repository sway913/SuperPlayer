package com.smzh.superplayer.player;

import android.os.Handler
import android.os.HandlerThread

class SuperPlayer {

    private var handler: Handler
    private var playerJni: PlayerJni

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
    }

    fun prepare() {
        handler.post { playerJni.prepare() }
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

    fun setPlayerListener(listener: PlayerJni.PlayerStateListener) {
        playerJni.setPlayerListener(listener)
    }
}
