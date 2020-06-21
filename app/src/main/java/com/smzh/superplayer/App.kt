package com.smzh.superplayer

import android.app.Application
import android.content.Context
import com.smzh.superplayer.http.HttpManager

class App : Application() {

    companion object {
        lateinit var context: Context
    }

    init {
        context = this
    }

    override fun onCreate() {
        super.onCreate()
        HttpManager.init()
    }

}