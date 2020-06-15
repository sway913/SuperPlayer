package com.smzh.superplayer.database

import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.room.TypeConverters
import com.smzh.superplayer.App
import com.smzh.superplayer.sing.Song

@Database(
        entities = [Song::class],
        version = 1,
        exportSchema = false
)

@TypeConverters(
        ListConverter::class
)

abstract class AppDataBase : RoomDatabase() {

    abstract fun songDao(): SongDao

    companion object {
        val instance = Room.databaseBuilder(App.context.applicationContext, AppDataBase::class.java, "super_ktv.db")
                .build()
    }

}