package com.smzh.superplayer

import android.view.View
import androidx.databinding.BindingAdapter

@BindingAdapter("visible")
fun setViewVisible(view: View, visible: Boolean) {
    view.visibility = if (visible) View.VISIBLE else View.GONE
}