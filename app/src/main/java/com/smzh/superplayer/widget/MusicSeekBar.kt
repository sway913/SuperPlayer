package com.smzh.superplayer.widget

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.SeekBar
import com.smzh.superplayer.R
import kotlinx.android.synthetic.main.music_seek_bar_layout.view.*

class MusicSeekBar : FrameLayout, View.OnClickListener, SeekBar.OnSeekBarChangeListener {

    private var totalMs: Long = 0L
    private var listener: PlayStatus? = null
    private var status = 0

    constructor(context: Context) : this(context, null)
    constructor(context: Context, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        initView()
    }

    private fun initView() {
        val view = View.inflate(context, R.layout.music_seek_bar_layout, null)
        addView(view, LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT))
        iv_switch.setOnClickListener(this)
        sb_progress.max = 100
        sb_progress.setOnSeekBarChangeListener(this)

    }

    fun setTotalMs(totalMs: Long) {
        this.totalMs = totalMs
    }


    fun setCurrentMs(currentMs: Long) {
        val sumMinutes = if (totalMs / 60 < 10) "0" + (totalMs / 60).toString() else (totalMs / 60).toString()
        val sumSecond = if (totalMs % 60 < 10) "0" + (totalMs % 60).toString() else (totalMs % 60).toString()

        val nowMinutes = if (currentMs / 60 < 10) "0" + (currentMs / 60).toString() else (currentMs / 60).toString()
        val nowSecond = if (currentMs % 60 < 10) "0" + (currentMs % 60).toString() else (currentMs % 60).toString()

        val str = "$nowMinutes:$nowSecond/$sumMinutes:$sumSecond"
        tv_time.text = str
        sb_progress.progress = (currentMs.toDouble() / totalMs.toDouble() * 100).toInt()

    }

    fun resume() {
        iv_switch.setImageResource(R.drawable.noti_play_small)
        status = 1
    }

    fun pause() {
        iv_switch.setImageResource(R.drawable.noti_pause_small)
        status = 0
    }

    override fun onClick(v: View?) {
        v?.let {
            when (it.id) {
                R.id.iv_switch -> {
                    if (status == 0) {
                        resume()
                        listener?.resumeMusic()

                    } else if (status == 1) {
                        pause()
                        listener?.pauseMusic()
                    } else {

                    }
                }
                else -> {
                }
            }
        }
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
        //pause()
        listener?.pauseMusic()
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        seekBar?.run {
            listener?.seek((totalMs * 1000 * progress.toDouble() / sb_progress.max.toDouble()).toLong())
            resume()
            listener?.resumeMusic()
        }
    }


    fun setPlayStatusListener(listener: PlayStatus) {
        this.listener = listener
    }


    interface PlayStatus {
        fun resumeMusic()
        fun pauseMusic()
        fun seek(millis: Long)
    }


}