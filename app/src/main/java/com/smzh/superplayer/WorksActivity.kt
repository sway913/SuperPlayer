package com.smzh.superplayer

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import com.smzh.superplayer.database.AppDataBase
import com.smzh.superplayer.sing.Song
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.activity_main.*

class WorksActivity : AppCompatActivity(), SongAdapter.ChooseSongListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_works)
        recyclerView.run {
            layoutManager = LinearLayoutManager(this@WorksActivity, LinearLayoutManager.VERTICAL, false)
            adapter = SongAdapter(this@WorksActivity, true)
        }
        supportActionBar?.title = "我的歌曲"
        loaSong()
    }

    override fun onResume() {
        super.onResume()
        loaSong()
    }

    private fun loaSong() {
        val disposable = Observable.create(ObservableOnSubscribe<List<Song>> { emitter ->
            emitter.onNext(AppDataBase.instance.songDao().getAll())
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe { t ->
                    (recyclerView.adapter as SongAdapter).refresh(t ?: arrayListOf())
                }
    }

    override fun onSongChosen(song: Song) {
        PlayActivity.start(this, song)
    }

    companion object {
        fun start(ctx: Context) {
            ctx.startActivity(Intent(ctx, WorksActivity::class.java))
        }
    }
}