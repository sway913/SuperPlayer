package com.smzh.superplayer

import android.os.Bundle
import android.support.v4.app.Fragment
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import kotlinx.android.synthetic.main.filter_item.view.*
import kotlinx.android.synthetic.main.fragment_filter_list.*

class FilterListFragment() : Fragment() {

    private var listener: FilterSelectListener? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        if (parentFragment is RecordFragment) {
//            listener = parentFragment
//        } else if (parentFragment is PreviewFragment) {
//            listener = parentFragment
//        }
    }

    fun setListener(listener: FilterSelectListener) {
        this.listener = listener
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_filter_list, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val adapter = FilterAdapter(filterList)
        filter_list.adapter = adapter
        val linearLayoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        filter_list.layoutManager = linearLayoutManager

    }


    inner class FilterAdapter(val item: ArrayList<String>) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

        var selectPosition = 0
        var selectHolder: FilterHolder? = null

        override fun onCreateViewHolder(p0: ViewGroup, p1: Int): RecyclerView.ViewHolder {
            return FilterHolder(LayoutInflater.from(p0.context).inflate(R.layout.filter_item, p0, false))
        }

        override fun onBindViewHolder(p0: RecyclerView.ViewHolder, p1: Int) {
            if (p0 is FilterHolder) {
                if(p1 == selectPosition){
                    p0.item.filter_image.setImageResource(R.drawable.oval_red)
                    selectHolder = p0
                } else {
                    p0.item.filter_image.setImageResource(R.drawable.oval)
                }
                p0.item.filter_name.text = item[p1]
                p0.item.setOnClickListener {
                    if(selectPosition != p1){
                        listener?.onFilterSelect(p1)
                        selectHolder?.item?.filter_image?.setImageResource(R.drawable.oval)
                        selectPosition = p1
                        selectHolder = p0
                        selectHolder?.item?.filter_image?.setImageResource(R.drawable.oval_red)
                    }
                }
            }
        }

        override fun getItemCount(): Int {
            return item.size
        }
    }

    class FilterHolder(val item: View) : RecyclerView.ViewHolder(item)

    interface FilterSelectListener {
        fun onFilterSelect(position: Int)
    }


    companion object {
        fun newInstance(): FilterListFragment {
            return FilterListFragment()
        }

        val filterList by lazy {
            arrayListOf<String>(
                    "原始",
                    "电影",
                    "KTV",
                    "演播",
                    "音乐会",
                    "古典男声",
                    "录音",
                    "古典女声",
                    "山谷",
                    "POP",
                    "ROCK",
                    "HIPHOP",
                    "SPACIOUS",
                    "ORIGINAL"
            )
        }
    }

}