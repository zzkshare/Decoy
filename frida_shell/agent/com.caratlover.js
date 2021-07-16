/*
com.chanson.business.login.activity.Vb  付费页
com.chanson.business.login.fragment.b   弹窗fragment
com.chanson.business.login.presenter.PhoneLoginPresenter -> loginBean.isComplete()
android intent launch_activity com.chanson.business.MainActivity
    11 -> 支付页
    9  -> 主页
    6 -> 填信息

LoginBean(userId=753897, token=457d5df3d9303544c42060310b3a3241, sex=2, isComplete=11, timSign=eJwszjHPgjAYBOD-8s5fSFughSbfwAATUQk6OBFjX7AiCKUiavzvJsD63OVyH9inuYNTpw2CDLhHyN9MWmFrdanRgIT6dh-R0GJAM6IphO8GoYClOKj61HVagaQeIUwwV7A10RVIwG38jn1VHzm9tEnU58-Da5cl09RHsbVnljyuZdpk1m6q-3XS6gZBUs58LxQh44uO8xXmEPj*AgAA--87Wzc3, isAnomaly=0, ph=null, highPrice=9.0, figure=2, appointment=2, getAlong=1)
CheckTalkBean(status=0, isVip=false, unlock=false, unlockTime=0, unlockDiamond=22, warningNotice=, noSpeakStatus=false, qq=, wx=qz53152, appointment=2, advantages=3, getAlong=2, nickname=小希, avatar=https://cdn.kela77.com/images/avatar/181202-202103071922-9s6nqi.png?x-oss-process=style/avatar_thumb, isGoddess=1, isRealpeople=1, vipLabel=0, sex=1, gpsCity=杭州市 ·西坝路)
BasicUserInfoBean(sex=2, age=31, avatar=https://cdn.kela77.com/images/public/default.png?x-oss-process=style/original, constellation=白羊座, distance=0.00km, gpsCity=杭州市, inviteType=通过系统发放的邀请码加入克拉恋人, isReal=false, isGoddess=false, isVip=false, job=, nickname=封刀令, onlineTime=online, hiddenSocial=false, birthday=639934622, annualIncome=4, iconArea=同顺街, vipLabel=0, sendUnlock=true, bubbleText=照片很漂亮，可以见面吗？, disturb=0, advantages=0, getAlong=1, appointment=2, education=0, currentState=0, await=0, figure=2, intention=0, videoUrl=, firstName=, status=0)

*/


var jclazz = null;
var jobj = null;

function getObjClassName(obj) {
    if (!jclazz) {
        var jclazz = Java.use("java.lang.Class");
    }
    if (!jobj) {
        var jobj = Java.use("java.lang.Object");
    }
    return jclazz.getName.call(jobj.getClass.call(obj));
}

function watch(obj, mtdName) {
    var listener_name = getObjClassName(obj);
    var target = Java.use(listener_name);
    if (!target || !mtdName in target) {
        return;
    }
    // send("[WatchEvent] hooking " + mtdName + ": " + listener_name);
    target[mtdName].overloads.forEach(function (overload) {
        overload.implementation = function () {
            send("[WatchEvent] " + mtdName + ": " + getObjClassName(this));
            // send("[WatchEvent] " + "stackTrace" + ": " + Java.use("android.util.Log").getStackTraceString(Java.use("java.lang.Throwable").$new()));
            return this[mtdName].apply(this, arguments);
        };
    })


}

rpc.exports = {
    onclick: function () {
        Java.perform(function () {


            Java.use("android.view.View").setOnClickListener.implementation = function (listener) {
                if (listener != null) {
                    watch(listener, 'onClick');
                }
                return this.setOnClickListener(listener);
            };


            Java.choose("android.view.View$ListenerInfo", {
                onMatch: function (instance) {
                    instance = instance.mOnClickListener.value;
                    if (instance) {
                        watch(instance, 'onClick');
                    }
                },
                onComplete: function () {
                }
            })

        })
    }
}



// 绕过付费
function modifyLoginBean() {
    Java.use("com.chanson.business.model.LoginBean").isComplete.implementation = function () {
        console.log("isComplete => "+this._isComplete)
        return 9
    }
}

function main() {
    Java.perform(function () {
        modifyLoginBean()
    })
}

main()