package com.smzh.superplayer.sing

import android.graphics.Rect
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.smzh.superplayer.App
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.WorksActivity
import com.smzh.superplayer.databinding.FragmentPreviewBinding
import com.smzh.superplayer.dp2px
import com.smzh.superplayer.sing.PreviewModel.Companion.STATE_PAUSE
import com.smzh.superplayer.sing.PreviewModel.Companion.STATE_PLAY
import com.smzh.superplayer.widget.CustomEffectView
import com.smzh.superplayer.widget.CustomSeekBar
import com.smzh.superplayer.widget.SingControlView
import kotlinx.android.synthetic.main.fragment_preview.*

class PreviewFragment : BaseFragment(), View.OnClickListener, CustomSeekBar.SeekListener, SingControlView.SingControlListener,
        AudioFilterAdapter.AudioEffectSelectListener, CustomEffectView.CustomEffectChangedListener {

    private lateinit var viewModel: PreviewModel
    private lateinit var binding: FragmentPreviewBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            val song = it.getParcelable<Song>(SONG)!!
            viewModel = ViewModelProvider(this, PreviewModel.PreviewFactory(song))[PreviewModel::class.java]
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_preview, container, false)
        binding.vm = viewModel
        binding.listener = this
        binding.lifecycleOwner = this
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        progress_bar.setPlayStatusListener(this)
        play_control.setListener(this)
        custom_view.setCustomEffectChangeListener(this)
        effect_view.run {
            layoutManager = GridLayoutManager(context, 5, GridLayoutManager.VERTICAL, false)
            adapter = AudioFilterAdapter(this@PreviewFragment)
            addItemDecoration(EffectItemDecoration())
        }
        viewModel.prepare()
        viewModel.mergerSuccess.observe(viewLifecycleOwner, Observer {
            if (it == true) {
                WorksActivity.start(context!!)
                activity?.finish()
            } else if (it == false) {
                Toast.makeText(App.context, "保存失败", Toast.LENGTH_SHORT).show()
            }
        })
        viewModel.playState.observe(viewLifecycleOwner, Observer {
            when (it) {
                STATE_PLAY -> {
                    progress_bar.toggle(true)
                }
                STATE_PAUSE -> {
                    progress_bar.toggle(false)
                    viewModel.progressText.postValue("暂停中")
                }
            }
        })
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_save -> {
                viewModel.stop()
                viewModel.startMerger()
                merger_container.visibility = View.VISIBLE
            }
            R.id.btn_restart -> {
                viewModel.stop()
                SingActivity.start(context!!, viewModel.song)
                activity?.finish()
            }
        }
    }

    override fun play(playing: Boolean) {
        if (playing) {
            viewModel.resume()
        } else {
            viewModel.pause()
        }
    }

    override fun seek(millis: Long) {
        viewModel.seek(millis)
    }

    override fun startSeek() {
        viewModel.pause()
    }

    override fun onAccVolumeChange(volume: Float) {
        viewModel.setAccVolume(volume)
    }

    override fun onPitchChange(pitch: Float) {
        viewModel.setPitch(pitch)
    }

    override fun onVocalVolumeChange(volume: Float) {
        viewModel.setVocalVolume(volume)
    }

    override fun onAudioEffectSelect(audioEffect: AudioEffect) {
        if (audioEffect.index == 15) {
            custom_view.show()
        } else {
            val effect = SingParam.AudioEffect
            effect[effect.size - 1] = audioEffect.index.toFloat()
            viewModel.setFilter(effect)
        }
    }

    override fun onCustomEffectChanged(index: Int) {
        val effect = SingParam.AudioEffect
        effect[effect.size - 1] = index.toFloat()
        viewModel.setFilter(effect)
    }

    override fun onDestroyView() {
        super.onDestroyView()
    }

    override fun onBackPressed() {
        if (custom_view.visibility == View.VISIBLE) {
            custom_view.hide()
            return
        }
        viewModel.stop()
        super.onBackPressed()
    }

    inner class EffectItemDecoration : RecyclerView.ItemDecoration() {
        override fun getItemOffsets(outRect: Rect, view: View, parent: RecyclerView, state: RecyclerView.State) {
            outRect.left = dp2px(parent.context, 10)
            outRect.right = dp2px(parent.context, 10)
            outRect.top = dp2px(parent.context, 15)
            outRect.bottom = dp2px(parent.context, 15)
        }
    }

    companion object {

        @JvmStatic
        fun newInstance(song: Song) =
                PreviewFragment().apply {
                    arguments = Bundle().apply {
                        putParcelable(SONG, song)
                    }
                }
    }
}