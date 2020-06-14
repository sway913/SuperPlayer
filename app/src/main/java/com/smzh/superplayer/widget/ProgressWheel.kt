package com.smzh.superplayer.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.text.format.DateUtils
import android.util.AttributeSet
import android.view.View
import com.smzh.superplayer.dp2px

class ProgressWheel : View {

    private lateinit var bgPaint: Paint
    private lateinit var textPaint: Paint
    private lateinit var drawText: String
    private var currentProgress = 0L

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
            color = Color.parseColor("#99000000")
            strokeWidth = 5f
            isAntiAlias = true
            style = Paint.Style.FILL
            textAlign = Paint.Align.CENTER
            textSize = 36f
        }
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val w = MeasureSpec.getSize(widthMeasureSpec)
        super.onMeasure(widthMeasureSpec, MeasureSpec.makeMeasureSpec(w, MeasureSpec.EXACTLY))
    }

    fun setProgress(progress: Long) {
        currentProgress = progress
        invalidate()
    }

    override fun onDraw(canvas: Canvas?) {
        canvas?.drawCircle(width / 2f, height / 2f, height / 2f, bgPaint)
        val fontMetrics: Paint.FontMetrics = textPaint.fontMetrics
        val top = fontMetrics.top
        val bottom = fontMetrics.bottom
        val baseLineY = (height / 2f - top / 2 - bottom / 2)

        canvas?.drawText(DateUtils.formatElapsedTime(currentProgress), width / 2f, baseLineY, textPaint)
    }
}