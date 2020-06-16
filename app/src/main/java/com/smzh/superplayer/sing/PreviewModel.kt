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
import io.reactivex.*
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.functions.Action
import io.reactivex.functions.Consumer
import io.reactivex.rxkotlin.subscribeBy
import io.reactivex.schedulers.Schedulers
import java.io.*

class PreviewModel(val song: Song) : ViewModel(), PlayerJni.PlayerStateListener {

    private val player by lazy { SuperPlayer.instance }
    val currentMs = MutableLiveData<String>()
    val totalMs = MutableLiveData<String>()
    var progressText = MutableLiveData<String>()
    val progress = MutableLiveData<Int>()
    val songName = MutableLiveData<String>()
    val mergerProgress = MutableLiveData<String>()
    val mergerSuccess = MutableLiveData<Boolean>()
    val handler = Handler(Looper.getMainLooper())

    init {
        songName.value = song.name
        currentMs.value = "00:00"
        totalMs.value = "00:00"
        progressText.value = "准备就绪"
    }

    private val runnable = object : Runnable {
        override fun run() {
            var total = player.getTotalMs()
            if (total <= 0) {
                total = 1L
            }
            currentMs.value = DateUtils.formatElapsedTime((player.getCurrentMs() / 1000f).toLong())
            totalMs.value = DateUtils.formatElapsedTime((total / 1000f).toLong())
            progressText.postValue("正在播放:${currentMs.value}/${totalMs.value}")
            progress.value = (player.getCurrentMs() * 100f / player.getTotalMs()).toInt()
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
        val audioParam = AudioParam(isRecorder = false,
                accPath = song.path ?: "",
                vocalPath = SingParam.vocalPath,
                decodePath = SingParam.decodePath)
        player.addPlayerListener(this)
        player.prepare(audioParam)
    }

    override fun onPrepared() {
        player.start()
        handler.postDelayed(runnable, 20)
    }

    fun stop() {
        player.stop()
        player.removePlayerListener(this)
        handler.removeCallbacksAndMessages(null)
    }

    override fun onCompleted() {

    }

    override fun onError() {

    }

    fun startMerger() {
        val mergerParam = MergerParam(SingParam.decodePath,
                SingParam.vocalPath,
                SingParam.mixPath)
        player.startMerge(mergerParam)
        handler.postDelayed(mergerRunnable, 100)
    }

    fun doOnMergerSuccess() {
        val disposable = Observable.create(ObservableOnSubscribe<Any> { emitter ->
            var input: InputStream? = null
            var output: OutputStream? = null
            val dstPath = SingParam.filePath + System.currentTimeMillis() + ".m4a"
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
    class PreviewFactory(val song: Song) : ViewModelProvider.NewInstanceFactory() {
        override fun <T : ViewModel?> create(modelClass: Class<T>): T {
            return PreviewModel(song) as T
        }
    }


    companion object {


    }
}