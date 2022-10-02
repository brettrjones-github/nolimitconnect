
#include "VxJava.h"
#ifdef TARGET_OS_ANDROID

#include <CoreLib/VxThread.h>
#include <CoreLib/VxGlobals.h>

#define FROM_NATIVE_LOG_TAG "NativeFromJava:"

JavaVM *                        VxJava::m_JavaVM = nullptr;

VxMutex                         g_FromJavaAccessMutex;

namespace
{
    int							g_JniLoadCalledCnt	= 0;
    VxMutex						g_oJavaMutex;
    VxJava                      g_VxJava;
}

//============================================================================
VxJava& GetJavaEnvCache()
{
    return g_VxJava;
}

//============================================================================
JNIEnv * VxJava::getJavaEnv( void )
{
    if( GetJavaEnvCache().getJavaVM() )
    {
        g_FromJavaAccessMutex.lock();
        JNIEnv* env;
        jint res = GetJavaEnvCache().getJavaVM()->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (res == JNI_EDETACHED)
        {
            res = GetJavaEnvCache().getJavaVM()->AttachCurrentThread(&env, nullptr);
            vx_assert(res == JNI_OK);
            LogModule( eLogThread, LOG_ERROR, "Android ThreadStartCallbackFunction FAILED AttachCurrentThread" );
        }

        g_FromJavaAccessMutex.unlock();

        return env;
    }
    else
    {
        LogModule( eLogThread, LOG_ERROR, "VxJava::getJavaEnv FAILED getJavaVM" );
        return nullptr;
    }
}

//============================================================================
void ThreadStartCallbackFunction( unsigned int uiThreadId, const char* threadName )
{
//    char buf[1024];
//    sprintf(buf, "Android ThreadStartCallbackFunction Thread %d %s\n", uiThreadId, threadName);
//    __android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, buf );

    LogModule( eLogThread, LOG_DEBUG, "ThreadStartCallbackFunction Thread %d %s\n", uiThreadId, threadName );
    if( GetJavaEnvCache().getJavaVM() )
    {
        g_FromJavaAccessMutex.lock();
        JNIEnv* env;
        jint res = GetJavaEnvCache().getJavaVM()->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

        if (res == JNI_EDETACHED)
        {
            res = GetJavaEnvCache().getJavaVM()->AttachCurrentThread(&env, nullptr);
            vx_assert(res == JNI_OK);
            LogModule( eLogThread, LOG_ERROR, "Android ThreadStartCallbackFunction SUCESS AttachCurrentThread %s", threadName);
            // pthread_setspecific(threadKey, env);
        }

        g_FromJavaAccessMutex.unlock();
    }
    else
    {
        LogModule( eLogThread, LOG_ERROR, "Android ThreadStartCallbackFunction FAILED getJavaVM" );
    }
}

//============================================================================
void ThreadExitCallbackFunction( unsigned int uiThreadId, bool isExitCallbackLocked, const char* threadName )
{
    if( false == isExitCallbackLocked )
    {
        g_FromJavaAccessMutex.lock();
    }

    LogModule( eLogThread, LOG_ERROR, "Android ThreadExitCallbackFunction Thread %d %s detaching", uiThreadId, threadName );
    if( GetJavaEnvCache().getJavaVM() )
    {
        GetJavaEnvCache().getJavaVM()->DetachCurrentThread();
    }
    else
    {
        LogModule( eLogThread, LOG_ERROR, "Android ThreadExitCallbackFunction FAILED getJavaVM" );
    }


    if ( false == isExitCallbackLocked )
    {
        g_FromJavaAccessMutex.unlock();
    }
}

//============================================================================
jint JNI_OnLoad( JavaVM * vm, void * reserved )
{
    LogModule( eLogThread, LOG_DEBUG, "JNI_OnLoad start" );
    g_JniLoadCalledCnt++;
    VxJava::m_JavaVM = vm;
    JNIEnv * env;
    if( vm->GetEnv( ( void** ) &env, JNI_VERSION_1_6 ) )
    {
        LogModule( eLogThread, LOG_DEBUG, "JNI_OnLoad FAIL Get Env" );
        return -1;
    }

    g_FromJavaAccessMutex.lock();
    // set up thread exit callback so we can attach thread to java
    VxThread::setThreadStartCallback( ThreadStartCallbackFunction );

    // set up thread exit callback so we can detach thread from java
    VxThread::setThreadExitCallback( ThreadExitCallbackFunction );

    //GetNativeToJavaClass(); // force register for this thread
    //LogMsg( LOG_WARN, "JJ JNI_OnLoad called JAVA VM 0x%x Load Count %d\n", g_poJavaVM, g_JniLoadCalledCnt );

    g_FromJavaAccessMutex.unlock();

    return JNI_VERSION_1_6;
}

//============================================================================
void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    VxSetAppIsShuttingDown( true );
    //GetEngineImp().getEngine().getPeerMgr().stopListening();
    LogModule( eLogThread, LOG_DEBUG, "JNI_OnUnload" );
}

#endif // TARGET_OS_ANDROID
