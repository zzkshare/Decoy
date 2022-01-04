/*
 * Created by zzk176 on 6/30/21, 9:20 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 6/30/21, 9:18 PM
 */

package top.zzkshare.decoy.utils

class JNIInterfaceManager {

    companion object {
        @JvmStatic
        external fun execJNI(str: String): String

        @JvmStatic
        external fun execJNIWithRoot(str: String): String

        @JvmStatic
        external fun getHostByName(str: String): String

        @JvmStatic
        external fun jniBase64En(str: String): String

        @JvmStatic
        external fun jniBase64De(str: String): String

        @JvmStatic
        external fun jniRc4En(key: String, content: ByteArray): String
    }
}