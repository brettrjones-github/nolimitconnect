/*
 *      Copyright (C) 2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "Context.h"
#include "PackageManager.h"
#include <qtandroid/log.h>
#include "Intent.h"
#include "IntentFilter.h"
#include "ClassLoader.h"
#include "jutils/jutils-details.hpp"
#include "BroadcastReceiver.h"
#include "JNIThreading.h"
#include "ApplicationInfo.h"
#include "JNIFile.h"
#include "ContentResolver.h"
#include "BaseColumns.h"
#include "MediaStore.h"
#include "PowerManager.h"
#include "Cursor.h"
#include "ConnectivityManager.h"
#include "AudioFormat.h"
#include "AudioAttributes.h"
#include "AudioManager.h"
#include "AudioTrack.h"
#include "Surface.h"
#include "MediaCodec.h"
#include "MediaCodecInfo.h"
#include "MediaFormat.h"
#include "Window.h"
#include "View.h"
#include "Build.h"
#include "DisplayMetrics.h"
#include "Intent.h"
#include "KeyEvent.h"
#include "Settings.h"
#include "JNIEnvironment.h"
#include "Document.h"
#include "RecognizerIntent.h"
#include "AudioDeviceInfo.h"
#include "MediaSync.h"

#if 0
#include <qtandroid/native_activity.h>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>

#include <CoreLib/VxJava.h>

using namespace jni;

jhobject CJNIContext::m_context(0);

CJNIContext * g_JniContext = nullptr;
AAssetManager* CJNIContext::m_AssetManager = nullptr;
JavaVM * CJNIContext::m_JniJvm = nullptr;
JNIEnv * CJNIContext::m_JniEnv = nullptr;

std::string CJNIContext::CONNECTIVITY_SERVICE;

// must be called before main
void CJNIContext::createJniContext( JavaVM * jvm, JNIEnv * env )
{
    if( !g_JniContext )
    {
        g_JniContext = new CJNIContext( jvm, env );
    }
}

CJNIContext& CJNIContext::getJniContext()
{
    return *g_JniContext;
}

void CJNIContext::destroyJniContext()
{
    if( g_JniContext )
    {
        delete g_JniContext;
        g_JniContext = nullptr;
    }
}

CJNIContext::CJNIContext( JavaVM * jvm, JNIEnv * env )
{
    initJavaContext( jvm, env );
}

void CJNIContext::initJavaContext( JavaVM * jvm, JNIEnv * env )
{
  xbmc_jni_on_load( jvm, env );

  m_JniJvm = jvm;
  m_JniEnv = env;

  // attach thread may be needed if created in different thread
  //m_JniJvm->AttachCurrentThread(&m_JniEnv, NULL);

  QAndroidJniObject mainActivity = QtAndroid::androidActivity();
  if ( mainActivity.isValid() )
  {
      jobject activityObj = mainActivity.object();
      if( activityObj )
      {
          jclass clazzObj = env->GetObjectClass(activityObj);
          if ( clazzObj )
          {
              jmethodID idNativeActivity_getAppContext = env->GetMethodID(clazzObj, "getApplicationContext", "()Landroid/content/Context;");
              if( idNativeActivity_getAppContext )
              {
                  jobject appContextObj = env->CallObjectMethod(activityObj, idNativeActivity_getAppContext, "()Landroid/content/Context;");
                  if( appContextObj )
                  {
                       m_context.reset( appContextObj );

                       CJNIBase::SetSDKVersion( 21 );
                       PopulateStaticFields();

                       jclass clazzContextObj = env->GetObjectClass(appContextObj);
                       jmethodID idGetAssets = env->GetMethodID(clazzContextObj, "getAssets", "()Landroid/content/res/AssetManager;");
                       m_AssetManager = AAssetManager_fromJava(env, env->CallObjectMethod(appContextObj, idGetAssets));
                       if( !m_AssetManager )
                       {
                           qWarning() << "m_AssetManager is null";
                       }
                   }
                  else
                  {
                      qWarning() << "getApplicationContext object is invalid";
                  }
              }
              else
              {
                  qWarning() << "getApplicationContext method id is invalid";
              }
          }
          else
          {
              qWarning() << "clazz obj is null";
          }
      }
      else
      {
          qWarning() << "activity object is invalid";
      }
  }
  else
  {
      qWarning()<<"main activity is invalid";
  }
}

int CJNIContext::attachThread()
{
    JNIEnv * jniEnv = getJNIEnv();
    JavaVM * javaVM = getJavaVM();
    int attachedState  = javaVM->AttachCurrentThread( &jniEnv, NULL );
    if( attachedState )
    {
        qWarning() << "Failed to attach java thread";
    }
    else if ( jniEnv )
    {
        if( jniEnv->ExceptionCheck() )
        {
            jniEnv->ExceptionDescribe();
        }
        else
        {
            initJavaContext( javaVM, jniEnv );
        }
    }

    return attachedState;
 }

void  CJNIContext::detachThread( int attachedState )
{
    getJavaVM()->DetachCurrentThread();
}


void CJNIContext::PopulateStaticFields()
{
  jhclass clazz = find_class("android/content/Context");
  CONNECTIVITY_SERVICE = jcast<std::string>(get_static_field<jhstring>(clazz,"CONNECTIVITY_SERVICE"));

//  CJNIBaseColumns::PopulateStaticFields();
//  CJNIMediaStoreMediaColumns::PopulateStaticFields();
//  CJNIPowerManager::PopulateStaticFields();
  CJNIPackageManager::PopulateStaticFields();
//  CJNIMediaStoreMediaColumns::PopulateStaticFields();
//  CJNICursor::PopulateStaticFields();
//  CJNIContentResolver::PopulateStaticFields();
//  CJNIConnectivityManager::PopulateStaticFields();
//  CJNIAudioFormat::PopulateStaticFields();
//  CJNIAudioAttributes::PopulateStaticFields();
//  CJNIAudioManager::PopulateStaticFields();
//  CJNIAudioTrack::PopulateStaticFields();
//  CJNISurface::PopulateStaticFields();
//  CJNIMediaCodec::PopulateStaticFields();
//  CJNIMediaCodecInfoCodecProfileLevel::PopulateStaticFields();
//  CJNIMediaCodecInfoCodecCapabilities::PopulateStaticFields();
//  CJNIMediaFormat::PopulateStaticFields();
//  CJNIView::PopulateStaticFields();
  CJNIBuild::PopulateStaticFields();
  CJNIDisplayMetrics::PopulateStaticFields();
//  CJNIIntent::PopulateStaticFields();
//  CJNIKeyEvent::PopulateStaticFields();
//  CJNISettings::PopulateStaticFields();
//  CJNIEnvironment::PopulateStaticFields();
//  CJNIDocument::PopulateStaticFields();
//  CJNIRecognizerIntent::PopulateStaticFields();
//  CJNIAudioDeviceInfo::PopulateStaticFields();
//  CJNIViewInputDevice::PopulateStaticFields();
//  CJNIMediaSync::PopulateStaticFields();
}

CJNIPackageManager CJNIContext::GetPackageManager()
{
  return call_method<jhobject>(m_context,
    "getPackageManager", "()Landroid/content/pm/PackageManager;");
}

void CJNIContext::startActivity(const CJNIIntent &intent)
{
  call_method<void>(jhobject(m_context),
    "startActivity", "(Landroid/content/Intent;)V",
    intent.get_raw());
}

int CJNIContext::checkCallingOrSelfPermission(const std::string &permission)
{
  return call_method<int>(m_context,
    "checkCallingOrSelfPermission", "(Ljava/lang/String;)I",
    jcast<jhstring>(permission));
}

jhobject CJNIContext::getSystemService(const std::string &service)
{
  return call_method<jhobject>(m_context,
    "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",
    jcast<jhstring>(service));
}

CJNIIntent CJNIContext::registerReceiver(const CJNIBroadcastReceiver &receiver, const CJNIIntentFilter &filter)
{
  return call_method<jhobject>(m_context,
    "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;",
    receiver.get_raw(), filter.get_raw());
}

CJNIIntent CJNIContext::registerReceiver(const CJNIIntentFilter &filter)
{
  return call_method<jhobject>(m_context,
    "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;",
    (jobject)NULL, filter.get_raw());
}

void CJNIContext::unregisterReceiver(const CJNIBroadcastReceiver &receiver)
{
  call_method<void>(m_context,
    "unregisterReceiver", "(Landroid/content/BroadcastReceiver;)V",
    receiver.get_raw());
}

void CJNIContext::sendBroadcast(const CJNIIntent &intent)
{
  call_method<void>(m_context,
    "sendBroadcast", "(Landroid/content/Intent;)V",
    intent.get_raw());
}

CJNIIntent CJNIContext::getIntent()
{
  return call_method<jhobject>(m_context,
    "getIntent", "()Landroid/content/Intent;");
}

CJNIClassLoader CJNIContext::getClassLoader()
{
  return call_method<jhobject>(m_context,
    "getClassLoader", "()Ljava/lang/ClassLoader;");
}

CJNIApplicationInfo CJNIContext::getApplicationInfo()
{
  return call_method<jhobject>(m_context,
    "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
}

std::string CJNIContext::getPackageName()
{
  return jcast<std::string>(call_method<jhstring>(m_context,
    "getPackageName", "()Ljava/lang/String;"));
}

std::string CJNIContext::getPackageResourcePath()
{
  return jcast<std::string>(call_method<jhstring>(m_context,
    "getPackageResourcePath", "()Ljava/lang/String;"));
}

CJNIFile CJNIContext::getCacheDir()
{
  return call_method<jhobject>(m_context,
    "getCacheDir", "()Ljava/io/File;");
}

CJNIFile CJNIContext::getDir(const std::string &path, int mode)
{
  return call_method<jhobject>(m_context,
    "getDir", "(Ljava/lang/String;I)Ljava/io/File;",
    jcast<jhstring>(path), mode);
}

CJNIFile CJNIContext::getExternalFilesDir(const std::string &path)
{
  return call_method<jhobject>(m_context,
    "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;",
    jcast<jhstring>(path));
}

CJNIContentResolver CJNIContext::getContentResolver()
{
  return call_method<jhobject>(m_context,
    "getContentResolver", "()Landroid/content/ContentResolver;");
}

CJNIWindow CJNIContext::getWindow()
{
  return call_method<jhobject>(m_context,
    "getWindow", "()Landroid/view/Window;");
}
#endif
