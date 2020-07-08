package com.smzh.superplayer.sing

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import com.smzh.superplayer.R
import com.smzh.superplayer.database.Filter
import com.smzh.superplayer.database.FilterDataManager
import com.smzh.superplayer.loadBitmapSync
import com.smzh.superplayer.player.VideoEffect
import kotlinx.android.synthetic.main.fragment_bottom.*
import java.io.ByteArrayOutputStream

class BottomFragment : Fragment(), FilterListAdapter.FilterApplyListener, SeekBar.OnSeekBarChangeListener {

    private var index: Int = 1
    private var listener: onFilterChangeListener? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (parentFragment is SingFragment) {
            listener = parentFragment as SingFragment
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_bottom, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        arguments?.let {
            setIndex(it.getInt("INDEX"))
        }
        filter_rv.run {
            layoutManager = LinearLayoutManager(context!!, LinearLayoutManager.HORIZONTAL, false)
            adapter = FilterListAdapter(FilterDataManager.getFilterList(), this@BottomFragment)
        }
        sb_smooth.setOnSeekBarChangeListener(this)
        sb_white.setOnSeekBarChangeListener(this)
    }

    override fun onFilterApply(filter: Filter) {
        SingParam.videoEffect.setLookupTablePath(SingParam.lookupPicPath + filter.name + ".png")
        listener?.onFilterChange(SingParam.videoEffect)
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
        when (seekBar?.id) {
            R.id.sb_white -> {
                SingParam.videoEffect.setWhiteLevel(progress / 100f)
                listener?.onFilterChange(SingParam.videoEffect)
            }
            R.id.sb_smooth -> {
                SingParam.videoEffect.setSmoothLevel(progress / 100f)
                listener?.onFilterChange(SingParam.videoEffect)
            }
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {

    }

    fun setIndex(index: Int) {
        if (rootView == null) {
            return
        }
        this.index = index
        if (index == 1) {
            beauty_container.visibility = View.VISIBLE
            filter_rv.visibility = View.GONE
        }
        if (index == 2) {
            beauty_container.visibility = View.GONE
            filter_rv.visibility = View.VISIBLE
        }
    }

    interface onFilterChangeListener {

        fun onFilterChange(videoEffect: VideoEffect)
    }


    companion object {

        fun newInstance(index: Int): BottomFragment {
            return BottomFragment().apply {
                arguments = Bundle().apply {
                    putInt("INDEX", index)
                }
            }
        }
    }
}