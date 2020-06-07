package com.smzh.superplayer.sing

import android.os.Environment
import com.smzh.superplayer.App
import java.io.File

object SingParam {

    val vocalPath: String
    val mixPath: String
    val outPath: String
    val decodePath: String

    init {

        basePath.run {
            val file = File(this)
            file.mkdirs()
            vocalPath = this + "vocal.pcm"
            mixPath = this + "mix.aac"
            outPath = this + "out.m4a"
            decodePath = this + "decode.pcm"
        }

    }

    private val basePath: String
        get() = if (App.context.externalCacheDir == null) {
            Environment.getExternalStorageDirectory().absolutePath + File.separator + "sing" + File.separator
        } else {
            App.context.externalCacheDir?.path + File.separator + "sing" + File.separator
        }


}