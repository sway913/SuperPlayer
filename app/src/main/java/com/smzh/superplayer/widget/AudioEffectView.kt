package com.smzh.superplayer.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Typeface
import android.util.AttributeSet
import android.view.View

class AudioEffectView : View {

    private var text: String = ""


    private val whitePaint: Paint = Paint().apply {
        color = Color.WHITE
        strokeWidth = 3f
        isAntiAlias = true
        style = Paint.Style.FILL
        textAlign = Paint.Align.CENTER
        textSize = 34f
        typeface = Typeface.create(Typeface.MONOSPACE, Typeface.BOLD)
    }
    private var blackPaint: Paint = Paint().apply {
        color = Color.parseColor("#99000000")
        strokeWidth = 3f
        isAntiAlias = true
        style = Paint.Style.FILL
        textAlign = Paint.Align.CENTER
        textSize = 34f
        typeface = Typeface.create(Typeface.MONOSPACE, Typeface.BOLD)
    }

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr)


    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val w = MeasureSpec.getSize(widthMeasureSpec)
        super.onMeasure(widthMeasureSpec, MeasureSpec.makeMeasureSpec(w, MeasureSpec.EXACTLY))
    }

    override fun setSelected(selected: Boolean) {
        super.setSelected(selected)
        invalidate()
    }

    fun setText(text: String) {
        this.text = text
    }

    override fun onDraw(canvas: Canvas?) {
        if (isSelected) {
            blackPaint.style = Paint.Style.FILL
            canvas?.drawCircle(width / 2f, height / 2f, width / 2f - 5f, blackPaint)

            val fontMetrics: Paint.FontMetrics = whitePaint.fontMetrics
            val top = fontMetrics.top
            val bottom = fontMetrics.bottom
            val baseLineY = (height / 2f - top / 2 - bottom / 2)
            canvas?.drawText(text, width / 2f, baseLineY, whitePaint)
        } else {
            blackPaint.style = Paint.Style.STROKE
            canvas?.drawCircle(width / 2f, height / 2f, width / 2f - 5f, blackPaint)

            val fontMetrics: Paint.FontMetrics = blackPaint.fontMetrics
            val top = fontMetrics.top
            val bottom = fontMetrics.bottom
            val baseLineY = (height / 2f - top / 2 - bottom / 2)
            blackPaint.style = Paint.Style.FILL
            canvas?.drawText(text, width / 2f, baseLineY, blackPaint)
        }
    }
}