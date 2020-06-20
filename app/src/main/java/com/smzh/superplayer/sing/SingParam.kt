package com.smzh.superplayer.sing

import android.os.Environment
import com.smzh.superplayer.App
import java.io.File

object SingParam {

    var vocalFactor = 0.5f
    var accFactor = 1f
    var pitch = 0f

    val vocalPath: String
    val mixPath: String
    val decodePath: String
    val testVocalPath: String
    const val vocalGain = 4F

    val AudioEffect = floatArrayOf(0.5f, 0.96f, 0.5f, 0.76f, 0.08f, 0.62f, 0.33f, 0.92f, 14f)

    init {

        cachePath.run {
            val file = File(this)
            file.mkdirs()
            vocalPath = this + "vocal.pcm"
            mixPath = this + "mix.aac"
            decodePath = this + "decode.pcm"
            testVocalPath = this + "test_vocal.pcm"
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