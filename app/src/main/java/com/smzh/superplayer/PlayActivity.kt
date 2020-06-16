package com.smzh.superplayer

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.exoplayer2.*
import com.google.android.exoplayer2.source.ConcatenatingMediaSource
import com.google.android.exoplayer2.source.ExtractorMediaSource
import com.google.android.exoplayer2.source.ProgressiveMediaSource
import com.google.android.exoplayer2.trackselection.DefaultTrackSelector
import com.google.android.exoplayer2.upstream.DefaultDataSourceFactory
import com.smzh.superplayer.database.AppDataBase
import com.smzh.superplayer.sing.Song
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.Scheduler
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.Disposable
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.activity_play.*
import java.util.concurrent.TimeUnit

class PlayActivity : AppCompatActivity() {

    private lateinit var exoPlayer: SimpleExoPlayer
    private lateinit var disposable: Disposable

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_play)
        val song = intent.getParcelableExtra<Song>(MainActivity.SONG)

        exoPlayer = ExoPlayerFactory.newSimpleInstance(
                this,
                DefaultRenderersFactory(this),
                DefaultTrackSelector()
        ).apply {
            playWhenReady = true
            addListener(object : Player.EventListener {
                override fun onPlayerError(error: ExoPlaybackException?) {

                }

                override fun onPlayerStateChanged(playWhenReady: Boolean, playbackState: Int) {

                }
            })
        }
        val defaultDataSourceFactory = DefaultDataSourceFactory(this, "SuperPlayer")
        exoPlayer.prepare(ProgressiveMediaSource
                .Factory(defaultDataSourceFactory)
                .createMediaSource(Uri.parse(song.path)))

        supportActionBar?.title = song.name

        disposable = Observable.interval(100, TimeUnit.MILLISECONDS, AndroidSchedulers.mainThread())
                .filter { exoPlayer.playWhenReady }
                .map {
                    if (exoPlayer.duration == 0L) {
                        0
                    } else {
                        exoPlayer.currentPosition
                    }
                }.subscribe {
                    player_view.setTotalMs(exoPlayer.duration)
                    player_view.setProgress(it)
                }

        btn_share.setOnClickListener {
            val intent = Intent().apply {
                action = Intent.ACTION_SEND
                type = "audio/*"
                putExtra(Intent.EXTRA_STREAM,Uri.parse(song.path))
            }
            startActivity(Intent.createChooser(intent,"分享到"))
        }

        btn_delete.setOnClickListener {
            AlertDialog.Builder(this)
                    .setMessage("确定删除此作品吗？")
                    .setPositiveButton("确定") { _, _ ->
                        exoPlayer.stop()
                        exoPlayer.release()
                        disposable.dispose()
                        deleteSong(song)
                    }
                    .setNegativeButton("取消", null)
                    .create()
                    .show()
        }
    }

    private fun deleteSong(song: Song) {
        val disposable = Observable.create(ObservableOnSubscribe<Any> { emitter ->
            AppDataBase.instance.songDao().deleteSong(song)
            emitter.onNext("")
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
                    finish()
                }
    }

    override fun onDestroy() {
        super.onDestroy()
        exoPlayer.stop()
        exoPlayer.release()
        disposable.dispose()
    }

    companion object {

        fun start(context: Context, song: Song) {
            val intent = Intent(context, PlayActivity::class.java)
            intent.putExtra(MainActivity.SONG, song)
            context.startActivity(intent)
        }
    }
}