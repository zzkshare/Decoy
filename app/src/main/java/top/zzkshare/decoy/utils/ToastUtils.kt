/*
 * Created by zzk176 on 6/30/21, 9:54 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 9:54 PM
 */

package top.zzkshare.decoy.utils

import android.content.Context
import android.widget.Toast
import top.zzkshare.decoy.App

object ToastUtils {
    fun makeToast(content: String) {
        Toast.makeText(App.ctx.get(), content, Toast.LENGTH_LONG).show()
    }
    fun makeToast(context: Context, content: String) {
        Toast.makeText(context, content, Toast.LENGTH_LONG).show()
    }
}