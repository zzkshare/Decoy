/*
 * Created by zzk176 on 6/30/21, 9:20 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 9:18 PM
 */

package top.zzkshare.decoy

import android.app.Application
import android.content.Context
import java.lang.ref.WeakReference


class App: Application() {
    companion object {
        init {
             System.loadLibrary("native-lib")
        }
        lateinit var ctx: WeakReference<Context>

    }

    override fun onCreate() {
        super.onCreate()
        ctx = WeakReference(applicationContext)
    }

}