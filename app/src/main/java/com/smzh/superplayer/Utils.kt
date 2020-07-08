package com.smzh.superplayer

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothProfile
import android.content.Context
import android.content.res.Resources
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.media.AudioDeviceInfo
import android.media.AudioManager
import android.os.Build
import android.provider.MediaStore
import com.smzh.superplayer.sing.Song
import java.io.IOException
import java.io.InputStream

fun dp2px(context: Context, dp: Int): Int {
    val resources: Resources = context.getResources()
    val metrics = resources.displayMetrics
    return dp * (metrics.densityDpi / 160f).toInt()
}

fun getMusicData(context: Context): MutableList<Song> {
    val list = mutableListOf<Song>()
    val cursor = context.contentResolver.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, null, null,
            null, MediaStore.Audio.AudioColumns.IS_MUSIC);
    if (cursor != null) {
        while (cursor.moveToNext()) {
            val song = Song()
            cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DISPLAY_NAME)).apply {
                val index = this.indexOf(".")
                if (index > 0) {
                    song.name = this.substring(0, index)
                }
            }
            song.singer = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ARTIST))
            song.path = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DATA))
            list.add(song)
        }
        cursor.close()
    }
    return list
}

fun isWiredHeadsetOn(context: Context): Boolean {
    val am = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        var result = false
        val infos = am.getDevices(AudioManager.GET_DEVICES_ALL)
        for (info in infos) {
            if (info.type == AudioDeviceInfo.TYPE_WIRED_HEADSET ||
                    info.type == AudioDeviceInfo.TYPE_USB_HEADSET ||
                    info.type == AudioDeviceInfo.TYPE_USB_DEVICE) {
                result = true
            }
        }
        if (am.isWiredHeadsetOn && !result) {
            var devices = ""
            for (info in infos) {
                devices += "${info.type},"
            }
        }
        return am.isWiredHeadsetOn || result
    } else {
        return am.isWiredHeadsetOn
    }
}

fun isWirelessHeadsetOn(): Boolean {
    val ba = BluetoothAdapter.getDefaultAdapter() ?: return false
    val a2dp = ba.getProfileConnectionState(BluetoothProfile.A2DP)              //可操控蓝牙设备，
    val headset = ba.getProfileConnectionState(BluetoothProfile.HEADSET)        //蓝牙头戴式耳机，支持语音输入输出
    val health = ba.getProfileConnectionState(BluetoothProfile.HEALTH)          //蓝牙穿戴式设备
    return when {
        a2dp == BluetoothProfile.STATE_CONNECTED -> true
        headset == BluetoothProfile.STATE_CONNECTED -> true
        health == BluetoothProfile.STATE_CONNECTED -> true
        else -> false
    }
}

fun isHeadSetOn(context: Context): Boolean {
    return isWiredHeadsetOn(context) || isWirelessHeadsetOn()
}

fun loadBitmapSync(assetFileName: String): Bitmap? {
    return try {
        val inputStream: InputStream = App.context.getAssets().open(assetFileName)
        BitmapFactory.decodeStream(inputStream)
    } catch (e: IOException) {
        null
    }
}