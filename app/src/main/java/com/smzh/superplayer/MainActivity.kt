package com.smzh.superplayer

import android.Manifest
import android.os.Bundle
import android.os.Environment
import android.view.Gravity
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import com.smzh.superplayer.sing.SingActivity
import com.smzh.superplayer.sing.Song
import com.tbruyelle.rxpermissions2.RxPermissions
import io.reactivex.Observable
import io.reactivex.Single
import io.reactivex.SingleEmitter
import io.reactivex.SingleObserver
import io.reactivex.SingleOnSubscribe
import io.reactivex.SingleSource
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.disposables.Disposable
import io.reactivex.internal.operators.mixed.SingleFlatMapObservable
import io.reactivex.rxkotlin.subscribeBy
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.activity_main.*
import org.reactivestreams.Subscriber
import java.util.*

class MainActivity : AppCompatActivity(), SongAdapter.ChooseSongListener {

    private val disposable = CompositeDisposable()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initView()

        disposable.add(Single.create(SingleOnSubscribe<List<Song>> {
            it.onSuccess(getMusicData(this@MainActivity))
        })
                .map {
                    Collections.sort(it)
                    it
                }
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribeBy(
                        onSuccess = {
                            ((recyclerView.adapter) as SongAdapter).refresh(it)
                        }
                )
        )
    }

    private fun initView() {
        recyclerView.run {
            layoutManager = LinearLayoutManager(this@MainActivity, LinearLayoutManager.VERTICAL, false)
            adapter = SongAdapter(this@MainActivity)
        }
    }

    override fun onSongChosen(song: Song) {
        gotoSing(song)
    }

    private fun gotoSing(song: Song) {
        disposable.add(RxPermissions(this)
                .request(Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .subscribe { granted ->
                    if (granted) {
                        SingActivity.start(this, song)
                    } else {
                        Toast.makeText(this, "未获取权限", Toast.LENGTH_LONG).show()
                    }
                }
        )
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.main_menu, menu)
        return super.onCreateOptionsMenu(menu)
    }

    override fun onOptionsItemSelected(item: MenuItem?): Boolean {
        when (item?.itemId) {
            R.id.menu_works -> {
                WorksActivity.start(this)
            }
        }
        return true
    }

    override fun onDestroy() {
        super.onDestroy()
        disposable.clear()
    }

    companion object {
        const val SONG = "song"
    }
}
