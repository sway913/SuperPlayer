package com.smzh.superplayer

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioManager
import android.os.Bundle
import android.os.Environment
import android.support.v4.app.ActivityCompat
import android.support.v4.content.ContextCompat
import android.support.v7.app.AppCompatActivity

class MainActivity : AppCompatActivity(), SwitchFragmentListener {

    var currentIndex = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        if (PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.RECORD_AUDIO), 0)
        }

        supportFragmentManager.beginTransaction().add(R.id.rootView, RecordFragment()).commitAllowingStateLoss()
        initParemeters()
    }

    override fun switchFragment(index: Int) {
        if (index == 1) {
            supportFragmentManager.beginTransaction().setCustomAnimations(R.anim.slide_in_left, R.anim.slide_out_right).replace(R.id.rootView, RecordFragment()).commitAllowingStateLoss()
        } else {
            supportFragmentManager.beginTransaction().setCustomAnimations(R.anim.slide_in_right, R.anim.slide_out_left).replace(R.id.rootView, PreviewFragment()).commitAllowingStateLoss()
        }
        currentIndex = index
    }

    override fun onBackPressed() {
        if (currentIndex != 1) {
            switchFragment(1)
        } else {
            super.onBackPressed()
        }
    }

    private fun initParemeters() {
        val myAudioMgr = getSystemService(Context.AUDIO_SERVICE) as AudioManager
        val sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)
        AudioParemeter.defaultSample = Integer.parseInt(sampleRateStr)
        AudioParemeter.defaultMusicPath = Environment.getExternalStorageDirectory().path + "/000/smzh.mp3"
        AudioParemeter.defaulGuidePath = Environment.getExternalStorageDirectory().path + "/000/wan_guide.m4a"
        AudioParemeter.defaultVocalPath = getExternalFilesDir(null)?.absolutePath + "/record.pcm"
        AudioParemeter.defaultTmpPath = getExternalFilesDir(null)?.absolutePath + "/music_tmp.pcm"
        AudioParemeter.outputPath = getExternalFilesDir(null)?.absolutePath + "/mix.aac"
    }
}
