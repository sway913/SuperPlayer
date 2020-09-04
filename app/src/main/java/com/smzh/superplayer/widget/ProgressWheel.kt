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
            style  = Paint.Style.STROKE
        }
        textPaint = Paint().apply {
            color = Color.parseColor("#99000000")
            strokeWidth = 5f
            isAntiAlias = true
            style = Paint.Style.FILL
            textAlign = Paint.Align.CENTER
            textSize = 120f
        }
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val w = MeasureSpec.getSize(widthMeasureSpec)
        super.onMeasure(widthMeasureSpec, MeasureSpec.makeMeasureSpec(w, MeasureSpec.EXACTLY))
    }

    override fun onDraw(canvas: Canvas?) {
        canvas?.run {
            val fontMetrics: Paint.FontMetrics = textPaint.fontMetrics
            val top = fontMetrics.top
            val bottom = fontMetrics.bottom
            val baseLineY = (height / 2f - top / 2 - bottom / 2)

            drawText("Dream", width / 2f, baseLineY, textPaint)
            bgPaint2.strokeWidth =  120f
            drawCircle(width / 2f, height / 2f, width / 2f - 60f, bgPaint2)
            drawCircle(width / 2f, height / 2f, width / 2f - 120f, bgPaint)
        }
    }
}