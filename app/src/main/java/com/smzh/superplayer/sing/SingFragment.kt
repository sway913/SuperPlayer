package com.smzh.superplayer.sing

import android.Manifest
import android.annotation.SuppressLint
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.smzh.superplayer.MainActivity.Companion.SONG
import com.smzh.superplayer.R
import com.smzh.superplayer.databinding.FragmentSingBinding
import com.smzh.superplayer.http.HttpManager
import com.smzh.superplayer.sing.SingViewModel.Companion.STATE_PAUSE
import com.smzh.superplayer.sing.SingViewModel.Companion.STATE_SING
import com.smzh.superplayer.widget.CustomSeekBar
import com.smzh.superplayer.widget.SingControlView
import com.smzh.superplayer.widget.SwitchButton
import com.tbruyelle.rxpermissions2.RxPermissions
import kotlinx.android.synthetic.main.fragment_sing.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.jsoup.Jsoup
import org.jsoup.nodes.Element


class SingFragment : BaseFragment(), View.OnClickListener, CustomSeekBar.SeekListener, SingControlView.SingControlListener,
        SwitchButton.OnModeChangeListener {

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
        progress_bar.setPlayStatusListener(this)
        btn_switch.setOnModeChangeListener(this)
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
                if (sing_control.visibility == View.VISIBLE) {
                    sing_control.visibility = View.GONE
                    btn_control.setTextColor(Color.BLACK)
                } else {
                    sing_control.visibility = View.VISIBLE
                    btn_control.setTextColor(Color.GRAY)
                }
            }

            R.id.btn_finish -> {
                gotoPreview()
            }

            R.id.btn_start -> {
                view?.requestFocus()
                viewModel.prepare()
            }

            R.id.btn_restart -> {
                viewModel.seek(0)
            }
            R.id.lyric_enter -> {
                if (lyric_input.text == null) {
                    return
                }
                GlobalScope.launch(Dispatchers.Main) {
                    loadLyric(lyric_input.text.toString())
                }
            }
            R.id.gl_view -> {
                hideBottomFragmentIfNeed()
            }
            R.id.tv_beauty -> {
                showBottomFragment(1)
            }
            R.id.tv_filter -> {

            }
            R.id.tv_sticker -> {

            }
            R.id.tv_switch -> {

            }
        }
    }

    private fun hideBottomFragmentIfNeed(): Boolean {
        val fragment = childFragmentManager.findFragmentByTag("bottom_fragment") ?: return false
        if (fragment.isHidden) {
            return false
        }
        childFragmentManager.beginTransaction().hide(fragment).commitAllowingStateLoss()
        return true
    }

    private fun showBottomFragment(index: Int) {
        val fragment = childFragmentManager.findFragmentByTag("bottom_fragment")
        if (fragment == null) {
            childFragmentManager.beginTransaction()
                    .add(R.id.bottom_container, BottomFragment.newInstance(), "bottom_fragment")
                    .commitAllowingStateLoss()
        } else {
            childFragmentManager.beginTransaction().show(fragment).commitAllowingStateLoss()
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

    override fun onModeChanged(isVideoMode: Boolean) {
        if (isVideoMode) {
            val disposable = RxPermissions(activity!!)
                    .request(Manifest.permission.CAMERA)
                    .subscribe { granted ->
                        if (granted) {
                            viewModel.isVideoMode.value = isVideoMode
                        } else {
                            Toast.makeText(context!!, "未获取权限", Toast.LENGTH_LONG).show()
                        }
                    }
        } else {
            viewModel.isVideoMode.value = isVideoMode
        }
    }

    override fun onBackPressed() {
        if (hideBottomFragmentIfNeed()) {
            return
        }
        viewModel.stop()
        super.onBackPressed()
    }

    @SuppressLint("SetJavaScriptEnabled")
    private suspend fun loadLyric(songName: String) {

        val finalLyric = withContext(Dispatchers.IO) {
            var lyric = "未搜索到歌词"
            try {
                val ret = HttpManager.httpApi.getLyricListHtml(songName).await()
                val dom = Jsoup.parse(ret)
                val ss: Element = dom.select("div.ss").first()
                val link = ss.selectFirst("a")
                val path = link.attr("href")

                val ret2 = HttpManager.httpApi.getLyricHtml(path).await()
                val dom2 = Jsoup.parse(ret2)
                val ss2: Element = dom2.select("div.center").first()
                val txt: Element = ss2.select("p").first()
                lyric = txt.text().replace(" ", "\n")
            } catch (e: Exception) {
                e.printStackTrace()
            }
            lyric
        }
        viewModel.lyric.postValue(finalLyric)
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