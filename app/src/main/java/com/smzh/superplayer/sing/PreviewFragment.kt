package com.smzh.superplayer.sing

import android.graphics.Color
import android.graphics.Rect
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.SurfaceHolder
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.smzh.superplayer.App
import com.smzh.superplayer.MainActivity.Companion.IS_VIDEO
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.WorksActivity
import com.smzh.superplayer.databinding.FragmentPreviewBinding
import com.smzh.superplayer.dp2px
import com.smzh.superplayer.player.SuperPlayer
import com.smzh.superplayer.sing.PreviewModel.Companion.STATE_PAUSE
import com.smzh.superplayer.sing.PreviewModel.Companion.STATE_PLAY
import com.smzh.superplayer.video.GLView
import com.smzh.superplayer.widget.CustomEffectView
import com.smzh.superplayer.widget.CustomSeekBar
import com.smzh.superplayer.widget.SingControlView
import kotlinx.android.synthetic.main.fragment_preview.*
import kotlinx.android.synthetic.main.fragment_preview.progress_bar

class PreviewFragment : BaseFragment(), View.OnClickListener, CustomSeekBar.SeekListener, SingControlView.SingControlListener,
        AudioFilterAdapter.AudioEffectSelectListener, CustomEffectView.CustomEffectChangedListener, GLView.SurfaceHolderListener {

    private lateinit var viewModel: PreviewModel
    private lateinit var binding: FragmentPreviewBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            val song = it.getParcelable<Song>(SONG)!!
            val isVideo = it.getBoolean(IS_VIDEO, false)
            viewModel = ViewModelProvider(this, PreviewModel.PreviewFactory(song, isVideo))[PreviewModel::class.java]
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
            layoutManager = if (!viewModel.isVideo)
                GridLayoutManager(context, 5, GridLayoutManager.VERTICAL, false)
            else {
                LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
            }
            adapter = AudioFilterAdapter(this@PreviewFragment)
            addItemDecoration(EffectItemDecoration())
        }
        viewModel.prepare()
        viewModel.mergerSuccess.observe(viewLifecycleOwner, Observer {
            if (it == true) {
                WorksActivity.start(context!!)
                activity?.finish()
            } else if (it == false) {
                Toast.makeText(App.context, "????????????", Toast.LENGTH_SHORT).show()
            }
        })
        viewModel.playState.observe(viewLifecycleOwner, Observer {
            when (it) {
                STATE_PLAY -> {
                    progress_bar.toggle(true)
                }
                STATE_PAUSE -> {
                    progress_bar.toggle(false)
                    viewModel.progressText.postValue("?????????")
                }
            }
        })
        if (viewModel.isVideo) {
            ((progress_bar.layoutParams) as ConstraintLayout.LayoutParams).run {
                bottomToTop = R.id.effect_view
                topToBottom = ConstraintLayout.LayoutParams.UNSET
            }
            ((effect_view.layoutParams) as ConstraintLayout.LayoutParams).run {
                bottomToTop = R.id.bottom_bar
                topToBottom = ConstraintLayout.LayoutParams.UNSET
                topMargin = dp2px(requireContext(), 10)
            }
            ((play_control.layoutParams) as ConstraintLayout.LayoutParams).run {
                topToBottom = ConstraintLayout.LayoutParams.UNSET
                bottomToTop = R.id.bottom_bar
            }
            play_control.visibility = View.GONE
        }
        video_preview.setSurfaceHolderListener(this)
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
                video_preview.setSurfaceHolderListener(null)
                SuperPlayer.instance.destroySurface()
                SingActivity.start(context!!, viewModel.song)
                activity?.finish()
            }
            R.id.btn_volume -> {
                btn_volume.isSelected = !btn_volume.isSelected
                play_control.visibility = if (btn_volume.isSelected) {
                    btn_volume.setTextColor(Color.GRAY)
                    effect_view.visibility = View.GONE
                    progress_bar.visibility = View.GONE
                    View.VISIBLE
                } else {
                    btn_volume.setTextColor(Color.BLACK)
                    effect_view.visibility = View.VISIBLE
                    progress_bar.visibility = View.VISIBLE
                    View.GONE
                }
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

    override fun onSurfaceCreate(holder: SurfaceHolder, w: Int, h: Int) {
        SuperPlayer.instance.createSurface(holder.surface, w, h, 1)
    }

    override fun onSurfaceDestroy() {
        SuperPlayer.instance.destroySurface()
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
        fun newInstance(song: Song, isVideo: Boolean = false) =
                PreviewFragment().apply {
                    arguments = Bundle().apply {
                        putParcelable(SONG, song)
                        putBoolean(IS_VIDEO, isVideo)
                    }
                }
    }
}