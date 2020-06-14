package com.smzh.superplayer.sing

import android.os.Parcel
import android.os.Parcelable

data class Song(var name: String? = null,
                var singer: String? = null,
                var path: String? = "") : Parcelable, Comparable<Song> {

    constructor(parcel: Parcel) : this(
            parcel.readString(),
            parcel.readString(),
            parcel.readString()
    )

    override fun writeToParcel(dest: Parcel, flags: Int) {
        dest.writeString(name)
        dest.writeString(singer)
        dest.writeString(path)
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