package com.smzh.superplayer.sing

import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.smzh.superplayer.MainActivity.Companion.IS_VIDEO
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R

class PreviewActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_preview)
        val song = intent.getParcelableExtra<Song>(SONG)
        val isVideo = intent.getBooleanExtra(IS_VIDEO, false)
        supportFragmentManager.beginTransaction()
                .replace(R.id.rootView, PreviewFragment.newInstance(song, isVideo))
                .commitAllowingStateLoss()
    }

    companion object {
        fun start(ctx: Context, song: Song, isVideo: Boolean = false) {
            val intent = Intent(ctx, PreviewActivity::class.java)
            intent.putExtra(SONG, song)
            intent.putExtra(IS_VIDEO, isVideo)
            ctx.startActivity(intent)
        }
    }
}