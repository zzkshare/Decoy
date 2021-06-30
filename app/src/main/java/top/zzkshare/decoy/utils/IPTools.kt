/*
 * Created by zzk176 on 6/30/21, 10:12 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 10:12 PM
 */

package top.zzkshare.decoy.utils

import android.widget.Toast

object IPTools {

    fun getIPByNative(): String {
        val queryIp = "ifconfig wlan0 | sed -n '/inet addr/s/^[^:]*:\\([0-9.]\\{7,15\\}\\) .*/\\1/p'"
        val result = JNIInterfaceManager.execJNIWithRoot(queryIp)
        return result
    }

    fun setHttpProxyByNative(addr: String) {
        JNIInterfaceManager.execJNIWithRoot("settings put global http_proxy ${addr}")
    }

    fun removeHttpProxyByNative() {
        setHttpProxyByNative(":0")
    }

    @Deprecated("no impl")
    fun getIPByJava(): String {
        TODO("it is not hard to impl")
    }

    fun getHostIPByNative(host: String): String {
        val ip = JNIInterfaceManager.getHostByName(host)
        if (ip == "-1") {
            LogUtil.e("get ip error, may be no internet")
            return ""
        }else{
            return ip
        }
    }

}