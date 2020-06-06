package com.smzh.superplayer.sing

import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.TextUtils
import com.smzh.superplayer.R

class SingActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sing)
        val accPath = intent.getStringExtra(ACC_PATH)
        if (TextUtils.isEmpty(accPath)) {

        } else {
            supportFragmentManager.beginTransaction()
                    .replace(R.id.rootView, SingFragment.newInstance(accPath, ""))
                    .commitAllowingStateLoss()
        }
    }

    companion object {

        const val ACC_PATH = "music_path"
        fun start(context: Context, accPath: String) {
            val intent = Intent(context, SingActivity::class.java)
            intent.putExtra(ACC_PATH, accPath)
            context.startActivity(intent)
        }
    }
}