package com.smzh.superplayer.sing

import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.PopupWindow
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.databinding.FragmentSingBinding
import com.smzh.superplayer.dp2px
import com.smzh.superplayer.sing.SingViewModel.Companion.STATE_PAUSE
import com.smzh.superplayer.sing.SingViewModel.Companion.STATE_SING
import com.smzh.superplayer.widget.CustomSeekBar
import com.smzh.superplayer.widget.SingControlView
import kotlinx.android.synthetic.main.fragment_sing.*

class SingFragment : BaseFragment(), View.OnClickListener, CustomSeekBar.SeekListener, SingControlView.SingControlListener {

    private lateinit var viewModel: SingViewModel
    private lateinit var binding: FragmentSingBinding
    private val controlView by lazy {
        SingControlView(context!!).apply {
            setListener(this@SingFragment)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            val song: Song = it.getParcelable(SONG)!!
            viewModel = ViewModelProvider(this, SingViewModel.SingFactory(song))[SingViewModel::class.java]
        }

    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_sing, container, false)
        binding.lifecycleOwner = this
        binding.vm = viewModel
        binding.listener = this
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        progress_bar.setPlayStatusListener(this)
        viewModel.singComplete.observe(viewLifecycleOwner, Observer {
            gotoPreview()
        })
        viewModel.singState.observe(viewLifecycleOwner, Observer {
            when (it) {
                STATE_SING -> {
                    progress_bar.toggle(true)
                }
                STATE_PAUSE -> {
                    progress_bar.toggle(false)
                    viewModel.singPercent.postValue("暂停中")
                }
            }
        })
    }

    override fun onClick(v: View?) {
        when (v?.id) {

            R.id.btn_control -> {
                showControlView()
            }

            R.id.btn_finish -> {
                gotoPreview()
            }

            R.id.btn_start -> {
                viewModel.prepare()
            }

            R.id.btn_restart -> {
                viewModel.seek(0)
            }

            R.id.btn_switch -> {

            }
        }
    }

    override fun seek(millis: Long) {
        viewModel.seek(millis)
    }

    override fun startSeek() {
        viewModel.pause()
    }

    override fun play(playing: Boolean) {
        if (playing) {
            viewModel.resume()
        } else {
            viewModel.pause()
        }
    }

    override fun onVocalVolumeChange(volume: Float) {
        viewModel.setVocalVolume(volume)
    }

    override fun onAccVolumeChange(volume: Float) {
        viewModel.setAccVolume(volume)
    }

    override fun onPitchChange(pitch: Float) {
        viewModel.setPitch(pitch)
    }

    private fun gotoPreview() {
        viewModel.stop()
        PreviewActivity.start(context!!, viewModel.song)
        activity?.finish()
    }

    override fun onBackPressed() {
        viewModel.stop()
        super.onBackPressed()
    }


    private var control: PopupWindow? = null
    private fun showControlView() {
        if (control != null) {
            control?.dismiss()
            control = null
            return
        }
        control = PopupWindow(context).apply {
            contentView = controlView
            width = ViewGroup.LayoutParams.MATCH_PARENT
            height = ViewGroup.LayoutParams.WRAP_CONTENT
            isTouchable = true
            isOutsideTouchable = true
            setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
            showAtLocation(rootView, Gravity.BOTTOM, 0, dp2px(context!!, 220))
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
    }

    companion object {

        @JvmStatic
        fun newInstance(song: Song) =
                SingFragment().apply {
                    arguments = Bundle().apply {
                        putParcelable(SONG, song)
                    }
                }
    }
}