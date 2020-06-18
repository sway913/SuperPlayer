package com.smzh.superplayer.widget

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import android.widget.SeekBar
import com.smzh.superplayer.R
import kotlinx.android.synthetic.main.layout_audio_controller.view.*

class SingControlView : FrameLayout, SeekBar.OnSeekBarChangeListener {

    private var listener: SingControlListener? = null

    constructor(context: Context) : this(context, null)
    constructor(context: Context, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init()
    }

    fun setListener(listener: SingControlListener) {
        this.listener = listener
    }

    private fun init() {
        View.inflate(context, R.layout.layout_audio_controller, this)
        sb_vocal_vol.setOnSeekBarChangeListener(this)
        sb_acc_vol.setOnSeekBarChangeListener(this)
        sb_pitch.setOnSeekBarChangeListener(this)
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
        when (seekBar?.id) {
            R.id.sb_vocal_vol -> {
                listener?.onVocalVolumeChange(seekBar.progress / 100f)
            }
            R.id.sb_acc_vol -> {
                listener?.onAccVolumeChange(seekBar.progress / 100f)
            }
            R.id.sb_pitch -> {
                val pitch: Int = seekBar.progress - 10
                sb_pitch.setProgress(pitch + 10)
                listener?.onPitchChange(pitch.toFloat())
            }
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {

    }

    interface SingControlListener {
        fun onVocalVolumeChange(volume: Float)
        fun onAccVolumeChange(volume: Float)
        fun onPitchChange(pitch: Float)
    }
}