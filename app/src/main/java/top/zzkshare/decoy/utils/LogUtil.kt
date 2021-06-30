/*
 * Created by zzk176 on 6/30/21, 10:00 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 10:00 PM
 */

package top.zzkshare.decoy.utils

import android.util.Log

val TAG = "ZZDecoy"

object LogUtil {

    fun e(content: String) {
        Log.e(TAG, content)
    }

    fun i(content: String) {
        Log.i(TAG, content)
    }

}