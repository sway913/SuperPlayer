package com.smzh.superplayer

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.LinearLayoutManager
import com.smzh.superplayer.sing.Song
import kotlinx.android.synthetic.main.activity_main.*

class WorksActivity : AppCompatActivity(), SongAdapter.ChooseSongListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_works)
        recyclerView.run {
            layoutManager = LinearLayoutManager(this@WorksActivity, LinearLayoutManager.VERTICAL, false)
            adapter = SongAdapter(this@WorksActivity)
        }
    }

    private fun loaSong() {

    }

    override fun onSongChosen(song: Song) {

    }
}