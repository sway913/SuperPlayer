package com.smzh.superplayer.video

import android.content.Context
import android.graphics.PixelFormat
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.smzh.superplayer.player.SuperPlayer
import com.smzh.superplayer.sing.SingParam

class GLView : SurfaceView, SurfaceHolder.Callback {

    private val player by lazy { SuperPlayer.instance }

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init()
    }

    private fun init() {
        holder.setFormat(PixelFormat.RGBA_8888)
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {

    }


    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        player.createSurface(holder.surface, width, height)
        player.setVideoEffect(SingParam.videoEffect)
    }


    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        player.destroySurface()
    }
}