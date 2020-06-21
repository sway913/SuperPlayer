package com.smzh.superplayer.http

import kotlinx.coroutines.Deferred
import retrofit2.http.GET
import retrofit2.http.Path
import retrofit2.http.Query

interface HttpApi {

    @GET("so.php")
    fun getLyricListHtml(@Query("wd") name: String): Deferred<String>

    @GET("/{path}")
    fun getLyricHtml(@Path("path") path: String): Deferred<String>

}