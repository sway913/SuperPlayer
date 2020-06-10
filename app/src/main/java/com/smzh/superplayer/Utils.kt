package com.smzh.superplayer

import android.content.Context
import android.content.res.Resources
import android.provider.MediaStore
import com.smzh.superplayer.sing.Song

fun dp2px(context: Context, dp: Int): Int {
    val resources: Resources = context.getResources()
    val metrics = resources.displayMetrics
    return dp * (metrics.densityDpi / 160f).toInt()
}

fun getMusicData(context: Context): List<Song> {
    val list = ArrayList<Song>()
    val cursor = context.contentResolver.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, null, null,
            null, MediaStore.Audio.AudioColumns.IS_MUSIC);
    if (cursor != null) {
        while (cursor.moveToNext()) {
            val song = Song()
            song.name = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DISPLAY_NAME));
            song.singer = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ARTIST));
            song.path = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DATA));
            list.add(song)
        }
        cursor.close();
    }
    return list;
}