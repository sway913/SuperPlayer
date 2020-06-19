package com.smzh.superplayer.sing

import android.view.ViewGroup
import android.widget.FrameLayout
import androidx.recyclerview.widget.RecyclerView
import com.smzh.superplayer.dp2px
import com.smzh.superplayer.widget.AudioEffectView

class AudioFilterAdapter(val listener: AudioEffectSelectListener) : RecyclerView.Adapter<AudioFilterAdapter.AudioEffectHolder>() {

    private var selectPostion = 0
    private var selectHolder: AudioEffectHolder? = null

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): AudioEffectHolder {
        val view = AudioEffectView(parent.context)
        val width = (parent.context.resources.displayMetrics.widthPixels - dp2px(parent.context, 40)) / 5 - dp2px(parent.context, 20)
        view.layoutParams = FrameLayout.LayoutParams(width, width).apply {

        }
        return AudioEffectHolder(view)
    }

    override fun onBindViewHolder(holder: AudioEffectHolder, position: Int) {
        if (position == selectPostion) {
            holder.bind(audioEffects[position], true)
            selectHolder = holder
        } else {
            holder.bind(audioEffects[position], false)
        }
    }

    override fun getItemCount(): Int {
        return audioEffects.size
    }


    inner class AudioEffectHolder(val item: AudioEffectView) : RecyclerView.ViewHolder(item) {

        fun bind(audioEffect: AudioEffect, select: Boolean) {
            item.isSelected = select
            item.setText(audioEffect.name)
            item.setOnClickListener {
                listener.onAudioEffectSelect(audioEffect)
                selectHolder?.item?.isSelected = false
                selectHolder = this
                selectHolder?.item?.isSelected = true
            }
        }
    }

    interface AudioEffectSelectListener {
        fun onAudioEffectSelect(audioEffect: AudioEffect)
    }


    companion object {
        val audioEffects = arrayListOf(
                AudioEffect(14, "原声"),
                AudioEffect(9, "流行"),
                AudioEffect(10, "R&B"),
                AudioEffect(11, "摇滚"),
                AudioEffect(12, "嘻哈"),
                AudioEffect(13, "空灵"),
                AudioEffect(8, "迷幻"),
                AudioEffect(2, "KTV"),
                AudioEffect(6, "留声机"),
                AudioEffect(3, "录音棚"),
                AudioEffect(4, "音乐会"),
                AudioEffect(1, "电影院"),
                AudioEffect(5, "男声"),
                AudioEffect(7, "女声"),
                AudioEffect(15, "···")
        )
    }

}