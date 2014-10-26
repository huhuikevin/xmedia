#include <jni.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/android_view_Surface.h>
#include <gui/Surface.h>
#include <assert.h>
#include <utils/Log.h>
#include <JNIHelp.h>
#include <media/stagefright/foundation/ADebug.h>
#include <ui/GraphicBufferMapper.h>
#include <cutils/properties.h>

using namespace android;

static jboolean _nativeSetVideoSurface(JNIEnv *env, jobject thiz, jobject jsurface){
	sp<Surface> surface;
	ALOGE("[%s]%d",__FILE__,__LINE__);
	surface = android_view_Surface_getSurface(env, jsurface);
	if(android::Surface::isValid(surface)){
		ALOGE("surface is valid ");
	}else {
		ALOGE("surface is invalid ");
		return false;
	}
	set_xbmc_video_surface(surface);
	ALOGE("[%s][%d]\n",__FILE__,__LINE__);
	return true;
}

static void native_start_xbmc()
{
	android_start_xbmc();
}

static JNINativeMethod gMethods[] = {
	{"_nativeSetVideoSurface",		"(Landroid/view/Surface;)Z", 		(void *)nativeSetVideoSurface},
	{"_native_start_xbmc();, "()V", (void *)native_start_xbmc();},
};

static const char* const kClassPathName = "com/jsoft.xmedia/MainActivity";

// This function only registers the native methods
static int register_com_jsoft_xmedia(JNIEnv *env)
{
	ALOGE("[%s]%d",__FILE__,__LINE__);
    return AndroidRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	ALOGE("[%s]%d",__FILE__,__LINE__);
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);
	ALOGE("[%s]%d",__FILE__,__LINE__);
   if (register_com_jsoft_xmedia(env) < 0) {
        ALOGE("ERROR: MediaPlayer native registration failed\n");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_6;

bail:
    return result;
}
