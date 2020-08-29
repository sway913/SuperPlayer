package com.smzh.superplayer.video

import android.content.Context
import android.graphics.PixelFormat
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

class GLView : SurfaceView, SurfaceHolder.Callback {

    private var listener: SurfaceHolderListener? = null

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
        listener?.onSurfaceCreate(holder, width, height)
    }


    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        listener?.onSurfaceDestroy()
    }

    fun setSurfaceHolderListener(listener: SurfaceHolderListener?) {
        this.listener = listener
    }

    interface SurfaceHolderListener {
        fun onSurfaceCreate(holder: SurfaceHolder, w: Int, h: Int)
        fun onSurfaceDestroy()
    }
}