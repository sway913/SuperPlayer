package com.smzh.superplayer.sing

import android.os.Handler
import android.os.Looper
import android.text.format.DateUtils
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.database.AppDataBase
import com.smzh.superplayer.player.AudioParam
import com.smzh.superplayer.player.MergerParam
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer
import com.smzh.superplayer.video.H264_AAC_toMp4_MediaMuxer
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.io.*

class PreviewModel(val song: Song, val isVideo: Boolean) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }
    val currentMs = MutableLiveData<Long>()
    val totalMs = MutableLiveData<Long>()
    var progressText = MutableLiveData<String>()
    val songName = MutableLiveData<String>()
    val mergerProgress = MutableLiveData<String>()
    val mergerSuccess = MutableLiveData<Boolean>()
    val playState = MutableLiveData<Int>()
    val handler = Handler(Looper.getMainLooper())
    val isVideoMode = MutableLiveData<Boolean>()

    init {
        songName.value = song.name
        progressText.value = "准备就绪"
        playState.value = STATE_IDLE
        isVideoMode.value = false
    }

    private val runnable = object : Runnable {
        override fun run() {
            var total = player.getTotalMs()
            if (total <= 0) {
                total = 1L
            }
            currentMs.value = player.getCurrentMs()
            totalMs.value = total
            progressText.postValue("正在播放: ${DateUtils.formatElapsedTime(currentMs.value!! / 1000)}/${DateUtils.formatElapsedTime(totalMs.value!! / 1000)}")
            handler.postDelayed(this, 20)
        }
    }

    private val mergerRunnable = object : Runnable {
        override fun run() {
            val progress = player.getMergeProgress()
            when {
                progress < 0 -> {
                    mergerSuccess.value = false
                }
                progress == 100 -> {
                    mergerProgress.value = "${progress}%"
                    doOnMergerSuccess()
                }
                else -> {
                    mergerProgress.value = "${progress}%"
                    handler.postDelayed(this, 20)
                }
            }
        }
    }

    fun prepare() {
        isVideoMode.value = isVideo
        val audioParam = AudioParam(isRecorder = false,
                accPath = song.path ?: "",
                vocalPath = SingParam.vocalPath,
                decodePath = SingParam.decodePath,
                withVideo = isVideo,
                videoPath = SingParam.videoPath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
        playState.postValue(STATE_PREPARE)
    }

    override fun onPrepared() {
        player.start()
        setVocalVolume(0.5f)
        handler.postDelayed(runnable, 20)
        playState.postValue(STATE_PLAY)
    }

    fun resume() {
        player.resume()
        playState.postValue(STATE_PLAY)
        handler.post(runnable)
    }

    fun pause() {
        player.pause()
        handler.removeCallbacks(runnable)
        playState.postValue(STATE_PAUSE)
    }

    fun stop() {
        player.removePlayerListener(this)
        handler.removeCallbacksAndMessages(null)
        playState.postValue(STATE_STOP)
        player.stop()
    }

    fun seek(ms: Long) {
        player.seek(ms)
    }

    fun setVocalVolume(vol: Float) {
        player.setVolume(vol * SingParam.vocalGain, SuperPlayer.Tracker.VOCAL)
        SingParam.vocalFactor = vol
    }

    fun setAccVolume(vol: Float) {
        player.setVolume(vol, SuperPlayer.Tracker.ACC)
        SingParam.accFactor = vol
    }

    fun setPitch(pitch: Float) {
        player.setPitch(pitch)
        SingParam.pitch = pitch
    }

    fun setFilter(floatArray: FloatArray) {
        player.setFilter(floatArray)
    }

    override fun onCompleted() {
        pause()
        seek(0L)
    }

    override fun onError() {
        player.stop()
        prepare()
    }

    fun startMerger() {
        val mergerParam = MergerParam(
                mode = if (!isVideo) 1 else 2,
                isVideo = isVideo,
                aacPath = SingParam.decodePath,
                vocalPath = SingParam.vocalPath,
                videoPath = SingParam.videoPath,
                outPath = SingParam.mixPath,
                outVideoPath = SingParam.outVideoPath,
                vocalVolume = SingParam.vocalFactor * SingParam.vocalGain,
                accVolume = SingParam.accFactor,
                pitch = SingParam.pitch,
                effect = SingParam.AudioEffect)
        player.startMerge(mergerParam)
        handler.postDelayed(mergerRunnable, 100)
    }

    fun doOnMergerSuccess() {
        val disposable = Observable.create(ObservableOnSubscribe<Any> { emitter ->
            var input: InputStream? = null
            var output: OutputStream? = null
            val dstPath = SingParam.filePath + System.currentTimeMillis() + if (isVideo) ".mp4" else ".aac"
            try {
                input = FileInputStream(SingParam.mixPath)
                output = FileOutputStream(dstPath)
                val buf = ByteArray(1024)
                var bytesRead: Int
                while (input.read(buf).also { bytesRead = it } > 0) {
                    output.write(buf, 0, bytesRead)
                }
            } finally {
                input?.close()
                output?.close()
                song.path = dstPath
                AppDataBase.instance.songDao().insert(song)
            }
            emitter.onNext(song)
        })
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
                    mergerSuccess.value = true
                }

    }


    @Suppress("UNCHECKED_CAST")
    class PreviewFactory(val song: Song, private val isVideo: Boolean) : ViewModelProvider.NewInstanceFactory() {
        override fun <T : ViewModel?> create(modelClass: Class<T>): T {
            return PreviewModel(song, isVideo) as T
        }
    }


    companion object {
        const val STATE_IDLE = 0
        const val STATE_PREPARE = 1
        const val STATE_PLAY = 2
        const val STATE_PAUSE = 3
        const val STATE_STOP = 4
    }
}