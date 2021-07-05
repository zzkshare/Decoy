/*
 * Created by zzk176 on 7/1/21, 3:08 PM
 * Copyright (c) 2021 . All rights reserved.
 * Last modified 7/1/21, 3:08 PM
 */

(function(){
    let Color = {RESET: "\x1b[39;49;00m", Black: "0;01", Blue: "4;01", Cyan: "6;01", Gray: "7;11", "Green": "2;01", Purple: "5;01", Red: "1;01", Yellow: "3;01"};
    let LightColor = {RESET: "\x1b[39;49;00m", Black: "0;11", Blue: "4;11", Cyan: "6;11", Gray: "7;01", "Green": "2;11", Purple: "5;11", Red: "1;11", Yellow: "3;11"};
    var colorPrefix = '\x1b[3', colorSuffix = 'm'
    for (let c in Color){
        if (c  == "RESET") continue;
        console[c] = function(message){
            console.log(colorPrefix + Color[c] + colorSuffix + message + Color.RESET);
        }
        console["Light" + c] = function(message){
            console.log(colorPrefix + LightColor[c] + colorSuffix + message + Color.RESET);
        }
    }
})();

function hookJavaFile() {
    Java.perform(function () {
        Java.use("java.io.File").exists.implementation = function () {
            var file_name = this.path.value
            console.log("hook file ==> " + file_name)
            if (file_name.includes("supersu") || file_name.includes("/su")){
                return false
            }
            else
                return this.exists()
        }
    })
}

function hookJavaRuntimeExecute() {
    Java.perform(function () {
        Java.use("java.lang.Runtime").exec.overload('[Ljava.lang.String;', '[Ljava.lang.String;', 'java.io.File').implementation = function (cmd, env, dir) {
            console.log("hook shell =>" + cmd)
            if (cmd.includes("su") || cmd.includes("ls"))
                return this.exec(["echo","hello"], env, dir)
            else
                console.log("process => + cmd")
                return this.exec(cmd, env, dir)
        }
    })
}

function hookFanzhan() {
    Java.enumerateClassLoaders({
        onMatch: function (loader) {
            try {
                if (loader.findClass("ui.activity.WelcomeActivity")) {
                    console.Red("Successfully found loader")
                    console.Blue(loader);
                    Java.classFactory.loader = loader;
                    console.Red("Switch Classloader Successfully ! ")
                }
            }
            catch (error) {
                console.Red(" continuing :" + error)
            }
        },
        onComplete: function () {
            console.Red("EnumerateClassloader END")
        }
    })

    Java.use("ui.activity.WelcomeActivity").onResume.implementation = function () {
        console.log("WelcomeAC")
    }
}

hookJavaFile()
hookJavaRuntimeExecute()
hookFanzhan()

function printObj(obj) {
    for (var key in obj) {
        console.log(key + "=>" + obj[key])
    }
}