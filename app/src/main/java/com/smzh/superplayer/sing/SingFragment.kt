package com.smzh.superplayer.sing

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.R

class SingFragment : Fragment() {

    private lateinit var viewModel: SingViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        var accPath = ""
        arguments?.let {
            accPath = it.getString(ACC_PATH)!!
        }
        viewModel = ViewModelProvider(this, SingViewModel.SingFactory(accPath))[SingViewModel::class.java]
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_sing, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        viewModel.prepare()
    }

    override fun onDestroyView() {
        super.onDestroyView()
        viewModel.stop()
    }

    companion object {

        private const val ACC_PATH = "acc_path"
        private const val GUIDE_PATH = "guide_path"

        @JvmStatic
        fun newInstance(accPath: String, guidePath: String) =
                SingFragment().apply {
                    arguments = Bundle().apply {
                        putString(ACC_PATH, accPath)
                        putString(GUIDE_PATH, guidePath)
                    }
                }
    }
}