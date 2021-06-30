/*
 * Created by zzk176 on 6/30/21, 9:20 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 9:20 PM
 */

package top.zzkshare.decoy.utils

import android.util.Log
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*
import top.zzkshare.decoy.App

object AdbdServiceUtil {

    fun startAdbd(port: String? = "5555"): Boolean {
        val result = JNIInterfaceManager.execJNIWithRoot("setprop service.adb.tcp.port 5555")
        if (result.isNotEmpty()) {
            LogUtil.e("start adbd fail => result")
            return false
        } else {
            JNIInterfaceManager.execJNIWithRoot("stop adbd")
            JNIInterfaceManager.execJNIWithRoot("start adbd")
            return true
        }
    }


}