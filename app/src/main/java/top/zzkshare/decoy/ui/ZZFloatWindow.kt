/*
 * Created by zzk176 on 7/16/21, 5:49 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 7/16/21, 5:49 PM
 */

package top.zzkshare.decoy.ui

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.provider.Settings
import android.view.*
import android.view.ViewGroup.LayoutParams.WRAP_CONTENT
import android.view.WindowManager.LayoutParams.*
import top.zzkshare.decoy.App
import top.zzkshare.decoy.BuildConfig
import top.zzkshare.decoy.MainActivity
import top.zzkshare.decoy.R
import top.zzkshare.decoy.listener.ItemViewTouchListener

class ZZFloatWindow {
    private var floatView: View? = null
    private var lx = 0
    private var ly = 0

    private object ZZFloatWindowHolder {
        val instance = ZZFloatWindow()
    }

    companion object {
        val instance: ZZFloatWindow by lazy { ZZFloatWindowHolder.instance }
    }

    @SuppressLint("ClickableViewAccessibility")
    fun addFloatWindow(activity: Activity) {
        if (floatView != null) return
        val clazz = Settings::class.java
        val canDrawOverlays =
            clazz.getDeclaredMethod("canDrawOverlays", Context::class.java)
        val result = canDrawOverlays.invoke(null, activity) as Boolean
        if (!result) {
            val clazz = Settings::class.java
            val field = clazz.getDeclaredField("ACTION_MANAGE_OVERLAY_PERMISSION")
            val intent = Intent(field.get(null).toString())
            intent.data = Uri.parse("package:${BuildConfig.APPLICATION_ID}")
            activity.startActivityForResult(intent, 0)
            return
        }

        floatView = LayoutInflater.from(activity).inflate(R.layout.float_window_item, null)
        val layoutParam = WindowManager.LayoutParams().apply {
            width = WRAP_CONTENT
            height = WRAP_CONTENT
            flags = FLAG_NOT_TOUCH_MODAL or FLAG_NOT_FOCUSABLE // or FLAG_LAYOUT_NO_LIMITS
            type = if (Build.VERSION.SDK_INT > Build.VERSION_CODES.O) TYPE_APPLICATION_OVERLAY else TYPE_PHONE
        }
        floatView!!.setOnTouchListener { v, event ->
            when (event?.action) {
                MotionEvent.ACTION_DOWN -> {
                    lx = event.rawX.toInt()
                    ly = event.rawY.toInt()

                }
                MotionEvent.ACTION_MOVE -> {
                    val nowX = event.rawX.toInt()
                    val nowY = event.rawY.toInt()
                    val movedX = nowX - lx
                    val movedY = nowY - ly
                    lx = nowX
                    ly = nowY
                    layoutParam.apply {
                        x += movedX
                        y += movedY
                    }
                    //更新悬浮球控件位置
                    activity.windowManager.updateViewLayout(v, layoutParam)
                }
            }
            false
        }

        floatView!!.setOnClickListener {
            activity.startActivity(Intent(activity, MainActivity::class.java))
        }
        activity.windowManager.addView(floatView, layoutParam)
    }


}