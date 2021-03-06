package com.smzh.superplayer

import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.animation.Animation
import android.view.animation.RotateAnimation
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.exoplayer2.*
import com.google.android.exoplayer2.source.ProgressiveMediaSource
import com.google.android.exoplayer2.trackselection.DefaultTrackSelector
import com.google.android.exoplayer2.upstream.DefaultDataSourceFactory
import com.smzh.superplayer.database.AppDataBase
import com.smzh.superplayer.sing.Song
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.disposables.Disposable
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.activity_play.*
import java.util.concurrent.TimeUnit


class PlayActivity : AppCompatActivity() {

    private lateinit var exoPlayer: SimpleExoPlayer
    private val disposable = CompositeDisposable()
    private lateinit var song: Song

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_play)
        song = intent.getParcelableExtra(MainActivity.SONG)

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
        video_view.post {
            video_view.player = exoPlayer
            video_view.showController()
        }
        if (song.isVideo == 0) {
            progress_bar.visibility = View.VISIBLE
        }
        val anim = RotateAnimation(0f, 360f, Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f)
        anim.duration = 10000
        anim.fillAfter = true
        anim.repeatMode = Animation.RESTART
        anim.repeatCount = Animation.INFINITE
        progress_bar.startAnimation(anim)
    }

    private fun deleteSong(song: Song) {
        val disposable1 = Observable.create(ObservableOnSubscribe<Any> { emitter ->
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
        disposable.clear()
        if (progress_bar.visibility == View.VISIBLE) {
            progress_bar.clearAnimation()
        }
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        super.onCreateOptionsMenu(menu)
        menu?.add(Menu.NONE, Menu.FIRST, 0, "??????")
        menu?.add(Menu.NONE, Menu.FIRST + 1, 0, "??????")
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem?): Boolean {
        when (item?.itemId) {
            Menu.FIRST -> {
                val intent = Intent().apply {
//                    val comp = ComponentName("com.tencent.mm", "com.tencent.mm.ui.tools.ShareToTimeLineUI")
//                    intent.component = comp
                    action = Intent.ACTION_SEND
                    type = "video/*"
                    putExtra(Intent.EXTRA_STREAM, Uri.parse(song.path))
                }
                startActivity(Intent.createChooser(intent, "?????????"))
            }
            Menu.FIRST + 1 -> {
                AlertDialog.Builder(this)
                        .setMessage("???????????????????????????")
                        .setPositiveButton("??????") { _, _ ->
                            exoPlayer.stop()
                            exoPlayer.release()
                            disposable.dispose()
                            deleteSong(song)
                        }
                        .setNegativeButton("??????", null)
                        .create()
                        .show()
            }
        }
        return super.onOptionsItemSelected(item)
    }

    companion object {

        fun start(context: Context, song: Song) {
            val intent = Intent(context, PlayActivity::class.java)
            intent.putExtra(MainActivity.SONG, song)
            context.startActivity(intent)
        }
    }
}