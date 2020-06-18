package com.smzh.superplayer.widget

import android.content.Context
import android.text.format.DateUtils
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import android.widget.SeekBar
import com.smzh.superplayer.R
import kotlinx.android.synthetic.main.custom_seek_bar_layout.view.*

class CustomSeekBar : FrameLayout, SeekBar.OnSeekBarChangeListener {

    private var listener: SeekListener? = null
    private var totalMs: Long = 0L

    constructor(context: Context) : this(context, null)
    constructor(context: Context, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        initView()
    }

    private fun initView() {
        View.inflate(context, R.layout.custom_seek_bar_layout, this)
        iv_switch.isSelected = false
        iv_switch_container.setOnClickListener {
            listener?.play(!iv_switch.isSelected)
        }
        play_progress.setOnSeekBarChangeListener(this)
    }

    fun setTotalMs(totalMs: Long) {
        total_ms.text = DateUtils.formatElapsedTime(totalMs / 1000)
        this.totalMs = totalMs
    }

    fun setCurrentMs(currentMs: Long) {
        if (totalMs == 0L) {
            return
        }
        play_progress.progress = (currentMs * 100f / totalMs).toInt()
        current_ms.text = DateUtils.formatElapsedTime(currentMs / 1000L)
    }

    fun toggle(playing: Boolean) {
        if (iv_switch.isSelected == playing) {
            return
        }
        iv_switch.isSelected = playing
        if (iv_switch.isSelected) {
            iv_switch.setImageResource(R.drawable.noti_play_small)
        } else {
            iv_switch.setImageResource(R.drawable.noti_pause_small)
        }
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
        listener?.startSeek()
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        seekBar?.run {
            listener?.seek((this.progress / 100F * totalMs).toLong())
        }
    }


    fun setPlayStatusListener(listener: SeekListener) {
        this.listener = listener
    }


    interface SeekListener {
        fun startSeek()
        fun seek(millis: Long)
        fun play(playing: Boolean)
    }


}