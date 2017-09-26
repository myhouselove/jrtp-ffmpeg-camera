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

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
}
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "AndroidAPI", __VA_ARGS__)

void testFFMPEG() {



}

JNIEXPORT void JNICALL
Java_wmy_jni_com_githubrtp_MainActivity_FFmpeg_1Init(JNIEnv *env, jobject instance) {
    // TODO
    av_register_all();
    avdevice_register_all();
    avfilter_register_all();
    avformat_network_init();
    LOGI("ffmpeg register OK!\n");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_wmy_jni_com_githubrtp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    LOGI("1111111111111111111111\n");





    testFFMPEG();

    RTPSession sess;
    uint16_t portbase, destport;
    uint32_t destip;
    std::string ipstr;
    int status, i, num;

    // First, we'll ask for the necessary information

//	std::cout << "Enter local portbase:" << std::endl;
//	std::cin >> portbase;
//	std::cout << std::endl;
    portbase = 12348;

//	std::cout << "Enter the destination IP address" << std::endl;
//	std::cin >> ipstr;
    ipstr = "192.168.1.106";

    destip = inet_addr(ipstr.c_str());
    if (destip == INADDR_NONE) {
        //	std::cerr << "Bad IP address specified" << std::endl;
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    // The inet_addr function returns a value in network byte order, but
    // we need the IP address in host byte order, so we use a call to
    // ntohl
    destip = ntohl(destip);

//	std::cout << "Enter the destination port" << std::endl;
//	std::cin >> destport;
    destport = 10000;

    //std::cout << std::endl;
//	std::cout << "Number of packets you wish to be sent:" << std::endl;
//	std::cin >> num;
    num = 500;

    // Now, we'll create a RTP session, set the destination, send some
    // packets and poll for incoming data.


//jboolean bl = (*env)->CallBooleanMethod(env, thiz, mid, js);

    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;

    // IMPORTANT: The local timestamp unit MUST be set, otherwise
    //            RTCP Sender Report info will be calculated wrong
    // In this case, we'll be sending 10 samples each second, so we'll
    // put the timestamp unit to (1.0/10.0)
    sessparams.SetOwnTimestampUnit(1.0 / 10.0);

    sessparams.SetAcceptOwnPackets(true);
    transparams.SetPortbase(portbase);
    status = sess.Create(sessparams, &transparams);
  //  checkerror(status);

    RTPIPv4Address addr(destip, destport);

    status = sess.AddDestination(addr);
//	checkerror(status);

   // __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", "ready\n");


    for (i = 1; i <= num; i++) {
        LOGI("\nSending packet %d/%d\n", i, num);

        // send the packet
        status = sess.SendPacket((void *) "1234567890", 10, 0, false, 10);
        //checkerror(status);
        //	 jboolean bl = (*env)->CallBooleanMethod(env, thiz, regsec, jregsec);

        sess.BeginDataAccess();

        // check incoming packets
        if (sess.GotoFirstSourceWithData()) {
            do {
                RTPPacket *pack;

                while ((pack = sess.GetNextPacket()) != NULL) {
                    // You can examine the data here
                    //	printf("Got packet !\n");

                    // we don't longer need the packet, so
                    // we'll delete it
                    sess.DeletePacket(pack);
                }
            } while (sess.GotoNextSourceWithData());
        }

        sess.EndDataAccess();

#ifndef RTP_SUPPORT_THREAD
        status = sess.Poll();
//		checkerror(status);
#endif // RTP_SUPPORT_THREAD

        RTPTime::Wait(RTPTime(1, 0));
    }

    sess.BYEDestroy(RTPTime(10, 0), 0, 0);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



