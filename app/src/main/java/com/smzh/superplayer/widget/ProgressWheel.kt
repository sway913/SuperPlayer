package com.smzh.superplayer.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.text.format.DateUtils
import android.util.AttributeSet
import android.view.View

class ProgressWheel : View {

    private lateinit var bgPaint: Paint
    private lateinit var bgPaint2: Paint
    private lateinit var textPaint: Paint
    private var currentProgress = 0L
    private var totalProgress = 360L
    private var angle = 0F

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init()
    }

    private fun init() {
        bgPaint = Paint().apply {
            color = Color.parseColor("#29000000")
        }
        bgPaint2 = Paint().apply {
            color = Color.parseColor("#40000000")
        }
        textPaint = Paint().apply {
            color = Color.parseColor("#99000000")
            strokeWidth = 5f
            isAntiAlias = true
            style = Paint.Style.FILL
            textAlign = Paint.Align.CENTER
            textSize = 40f
        }
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val w = MeasureSpec.getSize(widthMeasureSpec)
        super.onMeasure(widthMeasureSpec, MeasureSpec.makeMeasureSpec(w, MeasureSpec.EXACTLY))
    }

    fun setProgress(progress: Long?) {
        if (progress != null) {
            currentProgress = progress / 1000
            if (totalProgress == 0L) {
                angle = (currentProgress * 360f / 60f)
            } else {
                angle = (currentProgress * 360f / totalProgress)
            }
            if (angle > 360) {
                angle = 360f
            }
            invalidate()
        }
    }

    fun setTotalMs(progress: Long?) {
        if (progress == totalProgress / 1000) {
            return
        }
        progress?.let {
            totalProgress = progress / 1000
        }
    }

    override fun onDraw(canvas: Canvas?) {
        canvas?.run {
            val startX = 0
            val endX = width
            val startY = 0
            val endY = height
            drawArc(startX.toFloat(), startY.toFloat(), endX.toFloat(), endY.toFloat(), -90f, 360f, true, bgPaint)
            drawArc(startX.toFloat(), startY.toFloat(), endX.toFloat(), endY.toFloat(), -90f, angle, true, bgPaint2)

            val fontMetrics: Paint.FontMetrics = textPaint.fontMetrics
            val top = fontMetrics.top
            val bottom = fontMetrics.bottom
            val baseLineY = (height / 2f - top / 2 - bottom / 2)

            drawText(DateUtils.formatElapsedTime(currentProgress), width / 2f, baseLineY, textPaint)
        }
    }
}