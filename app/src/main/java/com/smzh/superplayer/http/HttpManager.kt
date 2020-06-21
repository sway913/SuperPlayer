package com.smzh.superplayer.http

import com.jakewharton.retrofit2.adapter.kotlin.coroutines.CoroutineCallAdapterFactory
import okhttp3.OkHttpClient
import okhttp3.RequestBody
import okhttp3.ResponseBody
import retrofit2.Converter
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import java.lang.reflect.Type

object HttpManager {

    lateinit var httpApi: HttpApi

    fun init() {
        httpApi = lyricRetrofit.create(HttpApi::class.java)
    }

    val lyricRetrofit by lazy {
        Retrofit.Builder()
                .baseUrl("https://www.90lrc.cn/")
                .addConverterFactory(object : Converter.Factory() {
                    override fun responseBodyConverter(type: Type, annotations: Array<Annotation>, retrofit: Retrofit): Converter<ResponseBody, *>? {
                        return Converter<ResponseBody, String> { value ->
                            value.string()
                        }
                    }
                })
                .addCallAdapterFactory(CoroutineCallAdapterFactory())
                .client(OkHttpClient())
                .build()

    }
}