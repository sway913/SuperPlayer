package com.smzh.superplayer.sing

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.databinding.FragmentPreviewBinding
import kotlinx.android.synthetic.main.fragment_preview.*
import kotlinx.android.synthetic.main.layout_song_item.*

class PreviewFragment : BaseFragment(), View.OnClickListener {

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
        viewModel.prepare()
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_save -> {
                viewModel.stop()
                viewModel.startMerger()
                merger_container.visibility = View.VISIBLE
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
    }

    override fun onBackPressed() {
        viewModel.stop()
        super.onBackPressed()
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