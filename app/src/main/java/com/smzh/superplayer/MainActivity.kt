package com.smzh.superplayer

import android.Manifest
import android.os.Bundle
import android.os.Environment
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.smzh.superplayer.sing.SingActivity
import com.tbruyelle.rxpermissions2.RxPermissions
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private var musicPath = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        btn_no_acc.setOnClickListener {
            gotoSing("")
        }
        btn_with_acc.setOnClickListener {
            gotoSing(Environment.getExternalStorageDirectory().path + "/000/音乐资源/smzh.mp3")
        }
    }

    private fun gotoSing(accPath: String) {
        val disposable = RxPermissions(this)
                .request(Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .subscribe { granted ->
                    if (granted) {
                        SingActivity.start(this, accPath)
                    } else {
                        Toast.makeText(this, "未获取权限", Toast.LENGTH_LONG).show()
                    }
                }
    }
}
