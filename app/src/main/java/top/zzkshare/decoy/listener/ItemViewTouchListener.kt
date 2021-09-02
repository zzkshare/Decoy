/*
 * Created by zzk176 on 7/16/21, 7:01 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 7/16/21, 7:01 PM
 */
package top.zzkshare.decoy.listener

import android.view.MotionEvent
import android.view.View
import android.view.WindowManager

class ItemViewTouchListener(private val windowManager: WindowManager, private val layoutParams:WindowManager.LayoutParams) : View.OnTouchListener {
    private var x = 0
    private var y = 0
    override fun onTouch(v: View?, event: MotionEvent?): Boolean {
        when (event?.action) {
            MotionEvent.ACTION_DOWN -> {
                x = event.rawX.toInt()
                y = event.rawY.toInt()

            }
            MotionEvent.ACTION_MOVE -> {
                val nowX = event.rawX.toInt()
                val nowY = event.rawY.toInt()
                val movedX = nowX - x
                val movedY = nowY - y
                x = nowX
                y = nowY
                layoutParams.apply {
                    x += movedX
                    y += movedY
                }
                //更新悬浮球控件位置
                windowManager.updateViewLayout(v, layoutParams)
            }
        }
        return false
    }

}