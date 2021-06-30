/*
 * Created by zzk176 on 6/30/21, 10:24 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 10:24 PM
 */

package top.zzkshare.decoy.utils

import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*
import top.zzkshare.decoy.App
import java.io.File
import java.io.FileOutputStream

@Deprecated("wait to impl just save it")
object VlmcsdUtil {
    fun setupVlmcsd() {
        val context = App.ctx.get()!!
        val ip = IPTools.getIPByNative()
        val helpKey =
            "slmgr //B -ipk W269N-WFGWX-YVC9B-4J6C9-T83GX\n" +
                    "slmgr //B /skms $ip\n" +
                    "slmgr /ato"
        val vlmcsdPath = context.cacheDir.absolutePath + "/vlmcsd"
        val result = JNIInterfaceManager.execJNIWithRoot(vlmcsdPath)
        if (result.isNotEmpty()) {
            if (setup_vlmcsd()) {
                val r = JNIInterfaceManager.execJNIWithRoot(vlmcsdPath)
            }
        }
    }

    private fun setup_vlmcsd(): Boolean {
//        val result = execShell("if [ ! -x \"/data/local/tmp/vlmcsd\" ]; then touch /data/local/tmp/vlmcsd;fi")
//        if (result.isNotEmpty()) {
//            Toast.makeText(this, result, Toast.LENGTH_LONG).show()
//        }
        val context = App.ctx.get()!!
        val assetManager = context.assets
        val vlmcsdPath = context.cacheDir.absolutePath + "/vlmcsd"
        val files = assetManager.list("")
        if (!files.isNullOrEmpty()) {
            for (file_name in files) {
                if (file_name == "vlmcsd") {
//                    LogUtil.i("find vlmcsd")
                    try {
                        var ins = assetManager.open(file_name)
                        var outFile = File(context.cacheDir, file_name)
                        var out = FileOutputStream(outFile)

                        val buffer = ByteArray(1024)
                        var read: Int
                        while (ins.read(buffer).also { read = it } != -1) {
                            out.write(buffer, 0, read)
                        }

                        ins.close()
                        out.flush()
                        out.close()
                    } catch (e: Exception) {
                        e.printStackTrace()
                        return false
                    }
                }
            }
        }
//        val result = execJNIWithRoot("if [ ! -x \"/data/local/tmp/vlmcsd\" ]; then cp ${vlmcsdPath} /data/local/tmp/;fi")
//        if (result.isNotEmpty()) {
//            sample_text.text = result
//            return false
//        }
        JNIInterfaceManager.execJNIWithRoot("chmod +x ${vlmcsdPath}")
        return true
    }
}