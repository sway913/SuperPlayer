package com.smzh.superplayer.database

import androidx.room.*
import com.smzh.superplayer.sing.Song

@Dao
interface SongDao {

    @Query("select *  from my_works")
    fun getAll(): List<Song>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    fun insert(vararg song: Song)

    @Delete
    fun deleteSong(song: Song)
}