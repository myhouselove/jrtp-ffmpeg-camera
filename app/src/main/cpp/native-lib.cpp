#include <jni.h>
#include <string>
#include <android/log.h>

#include "rtppacket.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif // WIN32


#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>

#define JNIREG_CLASS "wmy/jni/com/githubrtp/NativeFFMPEGMethod"
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "AndroidAPI", __VA_ARGS__)



static jstring JNICALL  native_stringFromJNI(JNIEnv* env,jclass obj){

    LOGI("this is jni string native!");
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


static jint JNICALL  native_FFmpeg_Init(JNIEnv* env,jclass obj){

    LOGI("this is jni ffmpeg_init native!");

    return 0;
}




// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
//        { "SetTouchRect", "(IIII)I", (void*)native_setTouchRect },
//        { "ProcessData", "([S)I", (void*)native_processData },
//        {"startAutoCalibration","(Ljava/lang/String;II)I",(void*)native_startAutoCalibration},
//        {"getCalibrationXY","()[I",(void*)native_getCalibrationXY},
//        {"getCalibrationABCD","()[F",(void*)native_getCalibrationABCD},
//
        {"FFmpeg_Init","()Ljava/lang/String;",(void*)native_FFmpeg_Init},
        {"stringFromJNI","()I",(void*)native_stringFromJNI},


};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((env)->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

int register_ndk_load(JNIEnv *env)
{
    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS,
                                 method_table, NELEM(method_table));
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;


    if ((vm)->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    register_ndk_load(env);

    // 返回jni的版本
    return JNI_VERSION_1_4;
}


