/*
 * Created by zzk176 on 6/30/21, 9:20 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 9:18 PM
 */

package top.zzkshare.decoy

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.provider.Settings
import android.util.Log
import android.widget.Toast
import top.zzkshare.decoy.BuildConfig
import top.zzkshare.decoy.R
import kotlinx.android.synthetic.main.activity_main.*
import top.zzkshare.decoy.ui.ZZFloatWindow
import top.zzkshare.decoy.utils.AdbdServiceUtil
import top.zzkshare.decoy.utils.IPTools
import top.zzkshare.decoy.utils.JNIInterfaceManager
import top.zzkshare.decoy.utils.JNIInterfaceManager.Companion.execJNI
import top.zzkshare.decoy.utils.JNIInterfaceManager.Companion.execJNIWithRoot
import top.zzkshare.decoy.utils.JNIInterfaceManager.Companion.getHostByName
import top.zzkshare.decoy.utils.ToastUtils
import java.io.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        ("""
${execShell("whoami")}
${IPTools.getIPByNative()}
        """).also { sample_text.text = it }

        adbd_btn.setOnClickListener {
            val result = AdbdServiceUtil.startAdbd()
            if (result) {
                sample_text.text = IPTools.getIPByNative()
            }
        }

        proxy_btn.setOnClickListener {
            IPTools.setHttpProxyByNative("${BuildConfig.CHARLES_DOMAIN}:${BuildConfig.CHARLES_PORT}")
        }
        unproxy_btn.setOnClickListener {
            IPTools.removeHttpProxyByNative()
        }
        mitm_btn.setOnClickListener {
            val ip = IPTools.getHostIPByNative(BuildConfig.MIMT_DOMAIN)
            IPTools.setHttpProxyByNative("${ip}:${BuildConfig.MIMT_PROT}")
        }

        vlmcsd_btn.setOnClickListener {

        }

        float_btn.setOnClickListener {
//            ZZFloatWindow.instance.addFloatWindow(this)
            val byte = "zzkshare".toByteArray()
            Log.i("TESTTEST", JNIInterfaceManager.jniBase64En(System.currentTimeMillis().toString()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", byte))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", byte))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", byte))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", byte))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey", "5vjVZyqSTHI=".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey","zzkshare".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey","zzkshare".toByteArray()))
//            Log.i("TESTTEST", JNIInterfaceManager.jniRc4En("testkey","zzkshare".toByteArray()))
        }

    }

    private fun execShell(cmd: String): String {
        val result = execJNI(cmd)
        Log.i("native cmd", "===== $cmd =====")
        Log.i("native cmd", result)
        return result
    }

}













