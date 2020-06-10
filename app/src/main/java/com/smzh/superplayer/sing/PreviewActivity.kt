package com.smzh.superplayer.sing

import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R

class PreviewActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_preview)
        val song = intent.getParcelableExtra<Song>(SONG)
        supportFragmentManager.beginTransaction()
                .replace(R.id.rootView, PreviewFragment.newInstance(song))
                .commitAllowingStateLoss()
    }

    companion object {
        fun start(ctx: Context, song: Song) {
            val intent = Intent(ctx, PreviewActivity::class.java)
            intent.putExtra(SONG, song)
            ctx.startActivity(intent)
        }
    }
}