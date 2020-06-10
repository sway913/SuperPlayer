package com.smzh.superplayer.sing

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.databinding.FragmentSingBinding
import kotlinx.android.synthetic.main.activity_main.view.*
import kotlinx.android.synthetic.main.layout_audio_controller.*
import kotlinx.android.synthetic.main.layout_song_item.*

class SingFragment : BaseFragment(), View.OnClickListener, SeekBar.OnSeekBarChangeListener {

    private lateinit var viewModel: SingViewModel
    private lateinit var binding: FragmentSingBinding

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
        sb_acc_vol.setOnSeekBarChangeListener(this)
        sb_vocal_vol.setOnSeekBarChangeListener(this)
        sb_pitch.setOnSeekBarChangeListener(this)
    }

    override fun onClick(v: View?) {
        when (v?.id) {

            R.id.btn_control -> {

            }

            R.id.btn_finish -> {
                viewModel.stop()
                PreviewActivity.start(context!!, viewModel.song)
                activity?.finish()
            }

            R.id.btn_start -> {
                viewModel.prepare()
            }

            R.id.btn_switch -> {

            }
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        when (seekBar?.id) {
            R.id.sb_acc_vol -> {

            }
            R.id.sb_vocal_vol -> {

            }
            R.id.sb_pitch -> {

            }
        }
    }

    override fun onBackPressed() {
        viewModel.stop()
        super.onBackPressed()
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