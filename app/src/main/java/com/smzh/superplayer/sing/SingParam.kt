package com.smzh.superplayer.sing

import android.os.Environment
import com.smzh.superplayer.App
import java.io.File

object SingParam {

    val vocalPath: String
    val mixPath: String
    val decodePath: String

    init {

        cachePath.run {
            val file = File(this)
            file.mkdirs()
            vocalPath = this + "vocal.pcm"
            mixPath = this + "mix.aac"
            decodePath = this + "decode.pcm"
        }
        filePath.run {
            val file = File(this)
            file.mkdirs()
        }

    }

    private val cachePath: String
        get() = if (App.context.externalCacheDir == null) {
            Environment.getExternalStorageDirectory().absolutePath + File.separator + "sing" + File.separator
        } else {
            App.context.externalCacheDir?.path + File.separator + "sing" + File.separator
        }

    val filePath: String
        get() = if (App.context.externalCacheDir == null) {
            Environment.getExternalStorageDirectory().absolutePath + File.separator + "audio" + File.separator
        } else {
            App.context.getExternalFilesDir(null)?.path + File.separator + "audio" + File.separator
        }
}