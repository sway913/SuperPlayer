package com.smzh.superplayer.widget

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import android.widget.SeekBar
import com.smzh.superplayer.R
import kotlinx.android.synthetic.main.custom_effect_layout.view.*

class CustomEffectView : FrameLayout, SeekBar.OnSeekBarChangeListener {

    private var listener: CustomEffectChangedListener? = null

    constructor(context: Context) : this(context, null)
    constructor(context: Context, attrs: AttributeSet?) : this(context, attrs, 0)

    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        initView()
    }

    private fun initView() {
        View.inflate(context, R.layout.custom_effect_layout, this)
        sb_rb_ratio.setOnSeekBarChangeListener(this)
        sb_rb_ratio.progress = (customEffect[0] * 100).toInt()
        sb_rb_depth.setOnSeekBarChangeListener(this)
        sb_rb_depth.progress = (customEffect[1] * 100).toInt()
        sb_rb_grain.setOnSeekBarChangeListener(this)
        sb_rb_grain.progress = (customEffect[2] * 100).toInt()
        sb_min_delay.setOnSeekBarChangeListener(this)
        sb_min_delay.progress = (customEffect[3] * 100).toInt()
        sb_max_delay.setOnSeekBarChangeListener(this)
        sb_max_delay.progress = (customEffect[4] * 100).toInt()
        sb_cmp_limit.setOnSeekBarChangeListener(this)
        sb_cmp_limit.progress = (customEffect[5] * 100).toInt()
        sb_cmp_range.setOnSeekBarChangeListener(this)
        sb_cmp_range.progress = (customEffect[6] * 100).toInt()
        sb_cmp_grain.setOnSeekBarChangeListener(this)
        sb_cmp_grain.progress = (customEffect[7] * 100).toInt()
        close_btn.setOnClickListener { hide() }
    }

    override fun onProgressChanged(p0: SeekBar?, p1: Int, p2: Boolean) {

    }

    override fun onStartTrackingTouch(p0: SeekBar?) {

    }

    override fun onStopTrackingTouch(p0: SeekBar?) {
        when (p0?.id) {
            R.id.sb_rb_ratio -> {
                customEffect[0] = p0.progress / 100f
            }
            R.id.sb_rb_depth -> {
                customEffect[1] = p0.progress / 100f
            }
            R.id.sb_rb_grain -> {
                customEffect[2] = p0.progress / 100f
            }
            R.id.sb_min_delay -> {
                customEffect[3] = p0.progress / 100f
            }
            R.id.sb_max_delay -> {
                customEffect[4] = p0.progress / 100f
            }
            R.id.sb_cmp_limit -> {
                customEffect[5] = p0.progress / 100f
            }
            R.id.sb_cmp_range -> {
                customEffect[6] = p0.progress / 100f
            }
            R.id.sb_cmp_grain -> {
                customEffect[7] = p0.progress / 100f
            }

            else -> {
            }
        }
        listener?.onCustomEffectChanged(customEffect)
    }

    fun show() {
        visibility = View.VISIBLE
        listener?.onCustomEffectChanged(customEffect)
    }

    fun hide() {
        visibility = View.GONE
    }

    fun setCustomEffectChangeListener(listener: CustomEffectChangedListener) {
        this.listener = listener
    }


    companion object {

        @JvmStatic
        private val customEffect = floatArrayOf(
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f
        )
    }

    interface CustomEffectChangedListener {

        fun onCustomEffectChanged(customEffect: FloatArray)

    }
}