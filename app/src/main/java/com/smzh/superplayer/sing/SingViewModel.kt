package com.smzh.superplayer.sing

import android.os.Handler
import android.os.Looper
import android.text.format.DateUtils
import android.util.Log
import android.widget.Toast
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.smzh.superplayer.App
import com.smzh.superplayer.database.FilterDataManager
import com.smzh.superplayer.isHeadSetOn
import com.smzh.superplayer.player.AudioParam
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer
import com.smzh.superplayer.player.SuperPlayer.Tracker
import com.smzh.superplayer.player.VideoEffect
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.io.InputStream

class SingViewModel(val song: Song) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }

    val singState = MutableLiveData<Int>()
    val singPercent = MutableLiveData<String>()
    val progress = MutableLiveData<Int>()
    val totalMs = MutableLiveData<Long>()
    val currentMs = MutableLiveData<Long>()
    val songName = MutableLiveData<String>()
    val singComplete = MutableLiveData<Boolean>()
    val lyric = MutableLiveData<String>()
    val isVideoMode = MutableLiveData<Boolean>()
    val hideAllButtom = MutableLiveData<Boolean>()
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
        lyric.value = "如果戴上耳机\n效果会更好哦"
        isVideoMode.value = false
        hideAllButtom.value = false
    }

    fun prepare() {
        val audioParam = AudioParam(isRecorder = true,
                accPath = song.path ?: "",
                vocalPath = SingParam.vocalPath,
                withVideo = isVideoMode.value ?: false,
                videoPath = SingParam.videoPath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
        singState.postValue(STATE_PREPARE)
        if (lyric.value == "如果戴上耳机\n效果会更好哦") {
            lyric.postValue(null)
        }
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

    fun switchCamera() {
        player.switchCamera()
    }

    fun setVideoEffect(videoEffect: VideoEffect) {
        player.setVideoEffect(videoEffect)
    }

    fun loadResource() {
        viewModelScope.launch(Dispatchers.Main) {
            copyFilterFiles()
            Toast.makeText(App.context, "资源加载完成", Toast.LENGTH_LONG).show()
        }
    }

    private suspend fun copyFilterFiles() = withContext(Dispatchers.IO) {
        FilterDataManager.getFilterList().forEach {
            val file = File(SingParam.lookupPicPath, it.name + ".png")
            if (!file.exists()) {
                try {
                    val input = App.context.assets.open("newlookupfilter/" + it.name + ".png")
                    val fos = FileOutputStream(file)
                    val buffer = ByteArray(1024)
                    var byteCount: Int
                    while (input.read(buffer).also { it1 -> byteCount = it1 } != -1) {
                        fos.write(buffer, 0, byteCount)
                    }
                    fos.flush()
                    input.close()
                    fos.close()
                } catch (e: IOException) {
                    Log.d("filter file load", e.toString())
                }
            }
        }
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