package com.smzh.superplayer.player;

import android.content.Context
import android.media.AudioManager
import android.os.Handler
import android.os.HandlerThread
import android.view.Surface
import com.smzh.superplayer.App
import com.smzh.superplayer.video.SuperCamera

class SuperPlayer {

    private var handler: Handler
    private var playerJni: PlayerJni
    private val outSample: Int
    private lateinit var superCamera: SuperCamera

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


    fun setFilter(floatArray: FloatArray) {
        handler.post { playerJni.setFilter(floatArray) }
    }

    fun setVolume(volume: Float, track: Tracker) {
        handler.post { playerJni.setVolume(volume, track.index) }
    }

    fun setPitch(pitch: Float) {
        handler.post { playerJni.setPitch(pitch) }
    }

    fun startMerge(mergerParam: MergerParam) {
        handler.post { playerJni.startMerge(mergerParam) }
    }

    fun getTotalMs(): Long {
        return playerJni.getTotalMs()
    }

    fun getCurrentMs(): Long {
        return playerJni.getCurrentMs()
    }

    fun getRealMs(): Long {
        return playerJni.getRealMs()
    }


    fun getMergeProgress(): Int {
        return playerJni.getMergeProgress()
    }

    fun createSurface(surface: Surface, width: Int, height: Int, mode: Int) {
        handler.post { playerJni.onSurfaceCreate(surface, width, height, mode) }
    }

    fun destroySurface() {
        handler.post { playerJni.onSurfaceDestroy() }
    }

    fun onFrameAvailable() {
        playerJni.onFrameAvailable()
    }

    fun switchCamera() {
        handler.post { playerJni.switchCamera() }
    }

    fun setVideoEffect(videoEffect: VideoEffect) {
        handler.post { playerJni.setVideoEffect(videoEffect) }
    }

    fun addPlayerListener(listener: PlayerJni.PlayerStateListener) {
        playerJni.setPlayerListener(listener)
    }

    fun removePlayerListener(listener: PlayerJni.PlayerStateListener) {
        playerJni.removePlayerStateListener(listener)
    }

    fun encodePreviewData(byteArray: ByteArray) {

    }

    enum class Tracker(var index: Int) {
        VOCAL(1),
        ACC(2);
    }
}
