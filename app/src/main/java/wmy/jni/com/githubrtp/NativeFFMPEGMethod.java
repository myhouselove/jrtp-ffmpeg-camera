package wmy.jni.com.githubrtp;

/**
 * Created by wangmingyong on 2017/9/26.
 */

public class NativeFFMPEGMethod {


    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int FFmpeg_Init();

}
