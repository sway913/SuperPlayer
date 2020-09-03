package com.smzh.superplayer.sing

import android.os.Parcel
import android.os.Parcelable
import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "my_works")
data class Song(
        @PrimaryKey(autoGenerate = true)
        @ColumnInfo(name = "key")
        var key: Int = 0,
        @ColumnInfo(name = "song_name")
        var name: String? = null,
        @ColumnInfo(name = "song_singer")
        var singer: String? = null,
        @ColumnInfo(name = "song_path")
        var path: String? = "",
        @ColumnInfo(name = "is_video")
        var isVideo: Int = 0
) : Parcelable, Comparable<Song> {

    constructor(parcel: Parcel) : this(
            parcel.readInt(),
            parcel.readString(),
            parcel.readString(),
            parcel.readString(),
            parcel.readInt()
    )

    override fun writeToParcel(dest: Parcel, flags: Int) {
        dest.writeInt(key)
        dest.writeString(name)
        dest.writeString(singer)
        dest.writeString(path)
        dest.writeInt(isVideo)
    }

    override fun describeContents(): Int {
        return 0
    }

    override fun compareTo(other: Song): Int {
        return -(name?.compareTo(other.name ?: "") ?: 0)
    }

    companion object CREATOR : Parcelable.Creator<Song> {
        override fun createFromParcel(source: Parcel): Song {
            return Song(source)
        }

        override fun newArray(size: Int): Array<Song?> {
            return arrayOfNulls(size)
        }
    }
}