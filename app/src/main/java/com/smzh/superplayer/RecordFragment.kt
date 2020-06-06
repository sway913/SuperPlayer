package com.smzh.superplayer

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.CompoundButton
import android.widget.SeekBar
import androidx.fragment.app.Fragment
import com.smzh.superplayer.player.PlayerJni
import com.smzh.superplayer.player.SuperPlayer
import kotlinx.android.synthetic.main.fragment_record.*

class RecordFragment : Fragment()/*, View.OnClickListener, PlayerJni.PlayerStateListener, MusicSeekBar.PlayStatus
        , SeekBar.OnSeekBarChangeListener, CompoundButton.OnCheckedChangeListener, FilterListFragment.FilterSelectListener*/ {

   /* private val player by lazy { SuperPlayer.getInstance() }
    private lateinit var switchListener: SwitchFragmentListener
    private val handler = Handler(Looper.getMainLooper())
    private var isRecording = false

    override fun onAttach(context: Context?) {
        super.onAttach(context)
        if (context is MainActivity) {
            switchListener = context
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_record, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        btn_control.setOnClickListener((this))
        player.setDataProcessListener(this)
        music_seek_bar.setPlayStatusListener(this)
        pitch.setOnSeekBarChangeListener(this)
        echo.setOnCheckedChangeListener(this)
        volume.setOnSeekBarChangeListener(this)
        music_volume.setOnSeekBarChangeListener(this)
        val filterFragment = FilterListFragment.newInstance()
        filterFragment.setListener(this)
        childFragmentManager.beginTransaction().add(R.id.filter_container, filterFragment).commitAllowingStateLoss()
    }

    override fun onReady() {
        activity?.runOnUiThread {
            btn_control.text = "正在录音，点击结束"
            btn_control.isEnabled = true
        }
        player.start()
        handler.post(timeRunnable)
        music_seek_bar.resume()
    }

    override fun onCompleted() {
        activity?.runOnUiThread {
            btn_control.text = "录音完成"
            //music_seek_bar.setTotalMs(player.totalMs / 1000)
            //music_seek_bar.setCurrentMs(player.totalMs / 1000)
        }
        handler.removeCallbacksAndMessages(null)
        player.stop()
        isRecording = false
        switchListener.switchFragment(2)
    }

    override fun onError() {
        player.stop()
        activity?.runOnUiThread {
            btn_control.text = "出现错误，点击重新录音？"
            btn_control.isEnabled = true
            isRecording = false
        }
    }

    override fun onClick(v: View?) {
        v?.let {
            when (it.id) {
                R.id.btn_control -> {
                    if (!btn_control.isSelected) {
                        btn_control.isSelected = true
                        btn_control.isEnabled = false
                        isRecording = true
                        player.prepare(true, AudioParemeter.defaultSample, AudioParemeter.defaultMusicPath, AudioParemeter.defaulGuidePath, AudioParemeter.defaultVocalPath)
                    } else {
                        btn_control.isSelected = false
                        handler.removeCallbacksAndMessages(null)
                        player.stop()
                        switchListener.switchFragment(2)
                    }
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        handler.removeCallbacksAndMessages(null)
        player.stop()
        player.removeDataProcessListener(this)
    }

    private val timeRunnable = object : Runnable {
        @SuppressLint("SetTextI18n")
        override fun run() {
            music_seek_bar.setTotalMs(player.totalMs / 1000)
            music_seek_bar.setCurrentMs(player.currentMs / 1000)
            handler.postDelayed(this, 50)
        }
    }

    override fun resumeMusic() {
        if (!isRecording) {
            btn_control.isSelected = true
            btn_control.isEnabled = false
            player.prepare(true, AudioParemeter.defaultSample, AudioParemeter.defaultMusicPath, AudioParemeter.defaulGuidePath, AudioParemeter.defaultVocalPath)
            isRecording = true
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


    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        seekBar?.run {
            if (id == R.id.pitch) {
                player.setPitch((progress * 2 - 100) / 10)
            } else if (id == R.id.volume) {
                player.setVolume((progress * 2f / 100), 0)
            } else if (id == R.id.music_volume) {
                player.setVolume((progress * 2f / 100), 1)
            }
        }
    }

    override fun onCheckedChanged(buttonView: CompoundButton?, isChecked: Boolean) {
        player.setEcho(isChecked)
    }

    override fun onFilterSelect(position: Int) {
        player.setFilter(position)
    }*/
}