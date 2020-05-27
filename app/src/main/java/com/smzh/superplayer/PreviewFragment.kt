package com.smzh.superplayer

import android.annotation.SuppressLint
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.support.v4.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import android.widget.Toast
import kotlinx.android.synthetic.main.fragment_peview.*
import java.io.File

class PreviewFragment : Fragment(), View.OnClickListener, AudioJni.DataProcessListener, MusicSeekBar.PlayStatus
        , FilterListFragment.FilterSelectListener, SeekBar.OnSeekBarChangeListener {

    private val player by lazy { SuperPlayer.getInstance() }
    private lateinit var switchListener: SwitchFragmentListener
    private val handler = Handler(Looper.getMainLooper())
    private var playFinish = false
    private var filter = 0
    private var voclaVolume = 1
    private var musicVolume = 1
    private var musicPitch = 0f

    override fun onAttach(context: Context?) {
        super.onAttach(context)
        if (context is MainActivity) {
            switchListener = context
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_peview, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        player.setDataProcessListener(this)
        btn_save.setOnClickListener((this))
        btn_share.setOnClickListener(this)
        btn_save.isEnabled = false
        music_seek_bar.setPlayStatusListener(this)
        pitch.setOnSeekBarChangeListener(this)
        volume.setOnSeekBarChangeListener(this)
        music_volume.setOnSeekBarChangeListener(this)
        val filterFragment = FilterListFragment.newInstance()
        filterFragment.setListener(this)
        childFragmentManager.beginTransaction().add(R.id.filter_container, filterFragment).commitAllowingStateLoss()
        player.prepare(false, AudioParemeter.defaultSample, AudioParemeter.defaultMusicPath, AudioParemeter.defaultTmpPath, AudioParemeter.defaultVocalPath)
    }

    override fun onDataReady() {
        activity?.runOnUiThread {
            btn_save.isEnabled = true
            music_seek_bar.resume()
        }
        player.start()
        playFinish = false
        handler.post(timeRunnable)
    }

    override fun onCompleted() {
        activity?.runOnUiThread {
            //music_seek_bar.setCurrentMs(player.totalMs / 1000)
            music_seek_bar.pause()
        }
        handler.removeCallbacksAndMessages(null)
        playFinish = true
        player.stop()
    }

    override fun onError() {
        player.stop()
        handler.post {
            context?.let {
                Toast.makeText(it, "出现错误,请重试", Toast.LENGTH_SHORT).show()
            }
        }
    }

    override fun onClick(v: View?) {
        v?.let {
            when (it.id) {
                R.id.btn_save -> {
                    player.stop()
                    player.prepareMerge(AudioParemeter.defaultVocalPath, AudioParemeter.defaultTmpPath, AudioParemeter.outputPath)
                    merge_container.visibility = View.VISIBLE
                    player.startMerge(voclaVolume, filter, musicVolume, musicPitch)
                    handler.post(mergeRunnable)
                }
                R.id.btn_share -> {
                    val intent = Intent(Intent.ACTION_SEND)
                    val file = File(AudioParemeter.outputPath)
                    intent.putExtra(Intent.EXTRA_STREAM, Uri.parse("content://" + file.absolutePath))
                    intent.type = "audio/*"
                    startActivity(Intent.createChooser(intent, "选择分享应用"))
                }

                else -> {
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        player.stop()
        player.stopMerge()
        player.removeDataProcessListener(this)
        handler.removeCallbacksAndMessages(null)
    }

    private val timeRunnable = object : Runnable {
        @SuppressLint("SetTextI18n")
        override fun run() {
            music_seek_bar.setTotalMs(player.totalMs / 1000)
            music_seek_bar.setCurrentMs(player.currentMs / 1000)
            handler.postDelayed(this, 50)
        }
    }

    private val mergeRunnable = object : Runnable {
        @SuppressLint("SetTextI18n")
        override fun run() {
            val progress = player.mergeProgress
           // Log.d("smzh", progress.toString())
            if (progress < 0) {
                tv_merge_progress.text = "未知错误"
                mergeSuccess()
            } else {
                tv_merge_progress.text = "$progress%"
            }
            if (progress == 100) {
                context?.let {
                    tv_merge_progress.text = "$progress%"
                    mergeSuccess()
                    btn_share.visibility = View.VISIBLE
                }
            } else {
                handler.postDelayed(this, 50)
            }
        }
    }

    private fun mergeSuccess() {
        handler.postDelayed({ player.stopMerge() }, 500)
    }

    override fun resumeMusic() {
        if (playFinish) {
            music_seek_bar.setTotalMs(0)
            player.prepare(false, AudioParemeter.defaultSample, AudioParemeter.defaultMusicPath, AudioParemeter.defaultTmpPath, AudioParemeter.defaultVocalPath)
        } else {
            player.resume()
        }
    }

    override fun pauseMusic() {
        player.pause()
    }

    override fun seek(millis: Long) {
        player.seek(millis)
    }

    override fun onFilterSelect(position: Int) {
        player.setFilter(position)
        filter = position
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }


    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }


    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        seekBar?.run {
            if (id == R.id.pitch) {
                player.setPitch((progress * 2 - 100) / 10)
                musicPitch = (progress * 2 - 100) / 10f
            } else if (id == R.id.volume) {
                player.setVolume((progress * 2f / 100), 0)
                voclaVolume = (progress * 2f / 100).toInt()
            } else if (id == R.id.music_volume) {
                player.setVolume((progress * 2f / 100), 1)
                musicVolume = (progress * 2f / 100).toInt()
            }
        }
    }


}