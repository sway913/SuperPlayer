package com.smzh.superplayer.widget

import android.animation.ValueAnimator
import android.annotation.SuppressLint
import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import com.smzh.superplayer.dp2px

class SwitchButton : View {

    private lateinit var bgPaint: Paint
    private lateinit var textPaint: Paint
    private lateinit var selectPaint: Paint
    private lateinit var audio: String
    private lateinit var video: String
    private var selectAudio = true
    private var radius = 0f
    private var selectBgX = 0f
    private var listener: OnModeChangeListener? = null

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init()
    }

    private fun init() {
        bgPaint = Paint().apply {
            color = Color.parseColor("#29000000")
        }
        textPaint = Paint().apply {
            color = Color.WHITE
            strokeWidth = 5f
            isAntiAlias = true
            style = Paint.Style.FILL
            textAlign = Paint.Align.CENTER
            textSize = 36f
        }
        selectPaint = Paint().apply {
            color = Color.parseColor("#99000000")
        }

        audio = "音频"
        video = "视频"
        radius = dp2px(context, 40).toFloat()
    }

    override fun onDraw(canvas: Canvas?) {
        canvas?.run {
            drawRoundRect(0f, 0f, width.toFloat(), height.toFloat(), radius, radius, bgPaint)
            drawRoundRect(selectBgX, 0f, selectBgX + width / 2f, height.toFloat(), radius, radius, selectPaint)
            val fontMetrics: Paint.FontMetrics = textPaint.fontMetrics
            val top = fontMetrics.top
            val bottom = fontMetrics.bottom
            val baseLineY = (height / 2f - top / 2 - bottom / 2)
            drawText(audio, width / 4f, baseLineY, textPaint)
            drawText(video, width / 4f * 3, baseLineY, textPaint)
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent?): Boolean {
        if (event?.actionMasked == MotionEvent.ACTION_UP) {
            selectAudio = !selectAudio
            doSwitchAnim(!selectAudio)
            listener?.onModeChanged(!selectAudio)
        }
        return true
    }

    private fun doSwitchAnim(toVideo: Boolean) {
        val startX = if (toVideo) 0f else width / 2f
        val endX = if (toVideo) width / 2f else 0f
        ValueAnimator.ofFloat(startX, endX).run {
            duration = 400
            addUpdateListener { animation ->
                selectBgX = animation.animatedValue as Float
                invalidate()
            }
            start()
        }
    }

    fun setOnModeChangeListener(listener: OnModeChangeListener) {
        this.listener = listener
    }

    interface OnModeChangeListener {
        fun onModeChanged(isVideoMode: Boolean)
    }
}