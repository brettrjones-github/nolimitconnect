#pragma once
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

#include "JNIBase.h"
#include <android/asset_manager_jni.h>
#include "BroadcastReceiver.h"

struct ANativeActivity;
class CJNIIntent;
class CJNIPackageManager;
class CJNIBroadcastReceiver;
class CJNIIntentFilter;
class CJNIClassLoader;
class CJNIApplicationInfo;
class CJNIFile;
class CJNIContentResolver;
class CJNIWindow;

class CJNIContext
{
public:
  static void createJniContext( JavaVM * jvm, JNIEnv * env );
  static CJNIContext& getJniContext();
  static void destroyJniContext();
  void initJavaContext( JavaVM * jvm, JNIEnv * env );

  const jni::jhobject& get_raw() const { return m_context; }

  static std::string CONNECTIVITY_SERVICE;

  static CJNIPackageManager GetPackageManager();
  static void startActivity(const CJNIIntent &intent);
  static jni::jhobject getSystemService(const std::string &service);
  static int checkCallingOrSelfPermission(const std::string &permission);
  static CJNIIntent registerReceiver(const CJNIBroadcastReceiver &receiver, const CJNIIntentFilter &filter);
  static CJNIIntent registerReceiver(const CJNIIntentFilter &filter);
  static void unregisterReceiver(const CJNIBroadcastReceiver &receiver);
  static void sendBroadcast(const CJNIIntent &intent);
  static CJNIIntent getIntent();
  static CJNIClassLoader getClassLoader();
  static CJNIApplicationInfo getApplicationInfo();
  static std::string getPackageName();
  static std::string getPackageResourcePath();
  static CJNIFile getCacheDir();
  static CJNIFile getDir(const std::string &path, int mode);
  static CJNIFile getExternalFilesDir(const std::string &path);
  static CJNIContentResolver getContentResolver();
  static CJNIWindow getWindow();
  static AAssetManager* getAssetManager() { return m_AssetManager; }
  static JNIEnv * getJNIEnv() { return m_JniEnv; };
  static JavaVM * getJavaVM() { return m_JniJvm; };
  int  attachThread();
  void  detachThread( int attachedState = 0 );


  CJNIContext( JavaVM * jvm, JNIEnv * env );

protected:
  ~CJNIContext() = default;

  // Main activity
  static jclass m_ActivityClass;

  static jni::jhobject m_context;

  static  AAssetManager* m_AssetManager;

  static JavaVM * m_JniJvm;
  static JNIEnv * m_JniEnv;

protected:
  CJNIContext() = default;

  void PopulateStaticFields();
  void operator=(CJNIContext const&){};
};
