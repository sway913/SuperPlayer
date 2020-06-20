package com.smzh.superplayer.widget

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import android.widget.SeekBar
import com.smzh.superplayer.R
import com.smzh.superplayer.sing.SingParam.AudioEffect
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
        sb_rb_ratio.progress = (AudioEffect[0] * 100).toInt()
        sb_rb_depth.setOnSeekBarChangeListener(this)
        sb_rb_depth.progress = (AudioEffect[1] * 100).toInt()
        sb_rb_grain.setOnSeekBarChangeListener(this)
        sb_rb_grain.progress = (AudioEffect[2] * 100).toInt()
        sb_min_delay.setOnSeekBarChangeListener(this)
        sb_min_delay.progress = (AudioEffect[3] * 100).toInt()
        sb_max_delay.setOnSeekBarChangeListener(this)
        sb_max_delay.progress = (AudioEffect[4] * 100).toInt()
        sb_cmp_limit.setOnSeekBarChangeListener(this)
        sb_cmp_limit.progress = (AudioEffect[5] * 100).toInt()
        sb_cmp_range.setOnSeekBarChangeListener(this)
        sb_cmp_range.progress = (AudioEffect[6] * 100).toInt()
        sb_cmp_grain.setOnSeekBarChangeListener(this)
        sb_cmp_grain.progress = (AudioEffect[7] * 100).toInt()
        close_btn.setOnClickListener { hide() }
    }

    override fun onProgressChanged(p0: SeekBar?, p1: Int, p2: Boolean) {

    }

    override fun onStartTrackingTouch(p0: SeekBar?) {

    }

    override fun onStopTrackingTouch(p0: SeekBar?) {
        when (p0?.id) {
            R.id.sb_rb_ratio -> {
                AudioEffect[0] = p0.progress / 100f
            }
            R.id.sb_rb_depth -> {
                AudioEffect[1] = p0.progress / 100f
            }
            R.id.sb_rb_grain -> {
                AudioEffect[2] = p0.progress / 100f
            }
            R.id.sb_min_delay -> {
                AudioEffect[3] = p0.progress / 100f
            }
            R.id.sb_max_delay -> {
                AudioEffect[4] = p0.progress / 100f
            }
            R.id.sb_cmp_limit -> {
                AudioEffect[5] = p0.progress / 100f
            }
            R.id.sb_cmp_range -> {
                AudioEffect[6] = p0.progress / 100f
            }
            R.id.sb_cmp_grain -> {
                AudioEffect[7] = p0.progress / 100f
            }

            else -> {
            }
        }
        listener?.onCustomEffectChanged(15)
    }

    fun show() {
        visibility = View.VISIBLE
        listener?.onCustomEffectChanged(15)
    }

    fun hide() {
        visibility = View.GONE
    }

    fun setCustomEffectChangeListener(listener: CustomEffectChangedListener) {
        this.listener = listener
    }


    interface CustomEffectChangedListener {

        fun onCustomEffectChanged(index: Int)

    }
}