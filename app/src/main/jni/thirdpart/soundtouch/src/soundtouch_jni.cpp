#include <jni.h>
#include <android/log.h>
#include <stdexcept>
#include <string>

using namespace std;

#include "../include/SoundTouch.h"
#include "../../../common/android_log.h"

//#define LOGV(...)   __android_log_print((int)ANDROID_LOG_INFO, "SOUNDTOUCH", __VA_ARGS__)
//#define LOGV(...)


// String for keeping possible c++ exception error messages. Notice that this isn't
// thread-safe but it's expected that exceptions are special situations that won't
// occur in several threads in parallel.
static string _errMsg = "";


#define BUFF_SIZE 4096


using namespace soundtouch;


// Set error message to return
static void _setErrmsg(const char *msg)
{
	_errMsg = msg;
}


#ifdef _OPENMP

#include <pthread.h>
extern pthread_key_t gomp_tls_key;
static void * _p_gomp_tls = NULL;

/// Function to initialize threading for OpenMP.
///
/// This is a workaround for bug in Android NDK v10 regarding OpenMP: OpenMP works only if
/// called from the Android App main thread because in the main thread the gomp_tls storage is
/// properly set, however, Android does not properly initialize gomp_tls storage for other threads.
/// Thus if OpenMP routines are invoked from some other thread than the main thread,
/// the OpenMP routine will crash the application due to NULL pointer access on uninitialized storage.
///
/// This workaround stores the gomp_tls storage from main thread, and copies to other threads.
/// In order this to work, the Application main thread needws to call at least "getVersionString"
/// routine.
static int _init_threading(bool warn)
{
	void *ptr = pthread_getspecific(gomp_tls_key);
	LOGV("JNI thread-specific TLS storage %ld", (long)ptr);
	if (ptr == NULL)
	{
		LOGV("JNI set missing TLS storage to %ld", (long)_p_gomp_tls);
		pthread_setspecific(gomp_tls_key, _p_gomp_tls);
	}
	else
	{
		LOGV("JNI store this TLS storage");
		_p_gomp_tls = ptr;
	}
	// Where critical, show warning if storage still not properly initialized
	if ((warn) && (_p_gomp_tls == NULL))
	{
		_setErrmsg("Error - OpenMP threading not properly initialized: Call SoundTouch.getVersionString() from the App main thread!");
		return -1;
	}
	return 0;
}

#else
static int _init_threading(bool warn)
{
	// do nothing if not OpenMP build
	return 0;
}
#endif

extern "C"
JNIEXPORT jint JNICALL
Java_com_maetimes_basic_media_audio_SoundTouch_processBuffer(JNIEnv *env, jobject instance,
        jlong handle, jint channel, jbyteArray in_, jint inSize, jbyteArray out_) {
    jbyte *in = env->GetByteArrayElements(in_, NULL);
    jbyte *out = env->GetByteArrayElements(out_, NULL);

    auto *ptr = (SoundTouch*)handle;
    ptr->putSamples((SAMPLETYPE *) in, static_cast<uint>(inSize / channel / 2));


    int nSamples = 0;
//    do {
        nSamples = ptr->receiveSamples((SAMPLETYPE *) out, static_cast<uint>(inSize / channel / 2));
//    } while (nSamples != 0);

    env->ReleaseByteArrayElements(in_, in, 0);
    env->ReleaseByteArrayElements(out_, out, 0);
    return nSamples * channel * 2;
}

extern "C" JNIEXPORT jstring JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_getVersionString(JNIEnv *env,
                                                                                                     jclass type)
{
    const char *verStr;

    LOGV("JNI call SoundTouch.getVersionString");

    // Call example SoundTouch routine
    verStr = SoundTouch::getVersionString();

    /// gomp_tls storage bug workaround - see comments in _init_threading() function!
    _init_threading(false);

    int threads = 0;
	#pragma omp parallel
    {
		#pragma omp atomic
    	threads ++;
    }
    LOGV("JNI thread count %d", threads);

    // return version as string
    return env->NewStringUTF(verStr);
}

extern "C" JNIEXPORT jlong JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_newInstance(JNIEnv *env, jclass type,
                                                                                              jint sampleRate, jint channels)
{
    auto ptr = new SoundTouch();
    ptr->setChannels(static_cast<uint>(channels));
    ptr->setSampleRate(static_cast<uint>(sampleRate));
	return (jlong)(ptr);
}

extern "C" JNIEXPORT void JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_deleteInstance(JNIEnv *env, jobject thiz, jlong handle)
{
    auto *ptr = (SoundTouch*)handle;
	delete ptr;
}

extern "C" JNIEXPORT void JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_setTempo(JNIEnv *env, jobject thiz, jlong handle, jfloat tempo)
{
    auto *ptr = (SoundTouch*)handle;
	ptr->setTempo(tempo);
}

extern "C" JNIEXPORT void JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_setPitchSemiTones(JNIEnv *env, jobject thiz, jlong handle, jfloat pitch)
{
    auto *ptr = (SoundTouch*)handle;
	ptr->setPitchSemiTones(pitch);
}

extern "C" JNIEXPORT void JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_setSpeed(JNIEnv *env, jobject thiz, jlong handle, jfloat speed)
{
    auto *ptr = (SoundTouch*)handle;
	ptr->setRate(speed);
}

extern "C" JNIEXPORT jstring JNICALL Java_com_maetimes_basic_media_audio_SoundTouch_getErrorString(JNIEnv *env,
                                                                                                   jclass type)
{
	jstring result = env->NewStringUTF(_errMsg.c_str());
	_errMsg.clear();

	return result;
}
