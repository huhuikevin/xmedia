#include <sstream>

#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

#include <android/native_window.h>
#include <android/configuration.h>
#include <jni.h>

#include <android/log.h>
using namespace std;

static sp<Surface> surface; 

void* thread_run(void* obj)
{
	XBMC_Run(false);//no windows render
}

int xmeida_printf(const char *format, ...)
{
  // For use before CLog is setup by XBMC_Run()
  va_list args;
  va_start(args, format);
  int result = __android_log_vprint(ANDROID_LOG_VERBOSE, "XMeida", format, args);
  va_end(args);
  return result;
}

void set_xbmc_video_surface(sp<Surface> s)
{
	surface = s;
}

void android_start_xbmc()
{
    xmedia_printf("%s: ", __PRETTY_FUNCTION__);
    if (!m_firstrun)
    {
      android_printf("%s: Already running, ignoring request to start", __PRETTY_FUNCTION__);
      return;
    }
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&m_thread, &attr, thread_run<CXBMCApp, &CXBMCApp::run>, this);
    pthread_attr_destroy(&attr);
	
}