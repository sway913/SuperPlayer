package com.smzh.superplayer.sing

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R

class SingActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sing)
        val song = intent.getParcelableExtra<Song>(SONG)
        supportFragmentManager.beginTransaction()
                .replace(R.id.rootView, SingFragment.newInstance(song))
                .commitAllowingStateLoss()
    }

    companion object {

        fun start(context: Context, song: Song) {
            val intent = Intent(context, SingActivity::class.java)
            intent.putExtra(SONG, song)
            context.startActivity(intent)
        }
    }
}