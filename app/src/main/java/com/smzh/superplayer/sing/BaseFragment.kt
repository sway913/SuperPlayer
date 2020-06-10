package com.smzh.superplayer.sing

import android.os.Bundle
import android.view.KeyEvent
import android.view.View
import androidx.fragment.app.Fragment

open class BaseFragment : Fragment(), View.OnKeyListener {

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        getView()?.run {
            isFocusableInTouchMode = true
            requestFocus()
            setOnKeyListener(this@BaseFragment)
        }
    }

    override fun onKey(v: View?, keyCode: Int, event: KeyEvent?): Boolean {
        event?.let {
            if (keyCode == KeyEvent.KEYCODE_BACK && it.action == KeyEvent.ACTION_UP) {
                onBackPressed()
                return true
            }
        }
        return false
    }

    open fun onBackPressed() {
        activity?.onBackPressed()
    }

}