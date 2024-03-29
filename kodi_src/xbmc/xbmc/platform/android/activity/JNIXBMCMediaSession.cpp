/*
 *  Copyright (C) 2017 Christian Browet
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */
#include "config_kodi.h"
#if !defined(HAVE_QT_GUI)

#include "JNIXBMCMediaSession.h"

#include <android/jni/jutils-details.hpp>
#include <android/jni/Context.h>

#include "CompileInfo.h"
#include "XBMCApp.h"
#include "Application.h"
#include "messaging/ApplicationMessenger.h"
#include "input/Key.h"

using namespace jni;

static std::string s_className = std::string(CCompileInfo::GetClass()) + "/XBMCMediaSession";

CJNIXBMCMediaSession::CJNIXBMCMediaSession()
  : CJNIBase(s_className)
  , m_isActive(false)
{
  m_object = new_object(CJNIContext::getClassLoader().loadClass(GetDotClassName(s_className)));
  m_object.setGlobal();

  add_instance(m_object, this);
}

CJNIXBMCMediaSession::CJNIXBMCMediaSession(const CJNIXBMCMediaSession& other)
  : CJNIBase(other)
{
  add_instance(m_object, this);
}

CJNIXBMCMediaSession::~CJNIXBMCMediaSession()
{
  remove_instance(this);
}

void CJNIXBMCMediaSession::RegisterNatives(JNIEnv* env)
{
  jclass cClass = env->FindClass(s_className.c_str());
  if(cClass)
  {
    JNINativeMethod methods[] =
    {
      {"_onPlayRequested", "()V", (void*)&CJNIXBMCMediaSession::_onPlayRequested},
      {"_onPauseRequested", "()V", (void*)&CJNIXBMCMediaSession::_onPauseRequested},
      {"_onNextRequested", "()V", (void*)&CJNIXBMCMediaSession::_onNextRequested},
      {"_onPreviousRequested", "()V", (void*)&CJNIXBMCMediaSession::_onPreviousRequested},
      {"_onForwardRequested", "()V", (void*)&CJNIXBMCMediaSession::_onForwardRequested},
      {"_onRewindRequested", "()V", (void*)&CJNIXBMCMediaSession::_onRewindRequested},
      {"_onStopRequested", "()V", (void*)&CJNIXBMCMediaSession::_onStopRequested},
      {"_onSeekRequested", "(J)V", (void*)&CJNIXBMCMediaSession::_onSeekRequested},
    };

    env->RegisterNatives(cClass, methods, sizeof(methods)/sizeof(methods[0]));
  }
}

void CJNIXBMCMediaSession::activate(bool state)
{
  if (state == m_isActive)
    return;

  call_method<void>(m_object,
                    "activate", "(Z)V",
                    (jboolean)state);
  m_isActive = state;
}

void CJNIXBMCMediaSession::updatePlaybackState(const CJNIPlaybackState& state)
{
  call_method<void>(m_object,
                    "updatePlaybackState", "(Landroid/media/session/PlaybackState;)V",
                    state.get_raw());
}

void CJNIXBMCMediaSession::updateMetadata(const CJNIMediaMetadata& myData)
{
  call_method<void>(m_object,
                    "updateMetadata", "(Landroid/media/MediaMetadata;)V",
                    myData.get_raw());
}

void CJNIXBMCMediaSession::updateIntent(const CJNIIntent& intent)
{
  call_method<void>(m_object,
                    "updateIntent", "(Landroid/content/Intent;)V",
                    intent.get_raw());
}

void CJNIXBMCMediaSession::OnPlayRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
  {
    if (g_application.GetAppPlayer().IsPaused())
      KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_PAUSE)));
  }
}

void CJNIXBMCMediaSession::OnPauseRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
  {
    if (!g_application.GetAppPlayer().IsPaused())
      KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_PAUSE)));
  }
}

void CJNIXBMCMediaSession::OnNextRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
    KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_NEXT_ITEM)));
}

void CJNIXBMCMediaSession::OnPreviousRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
    KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_PREV_ITEM)));
}

void CJNIXBMCMediaSession::OnForwardRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
  {
    if (!g_application.GetAppPlayer().IsPaused())
      KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_PLAYER_FORWARD)));
  }
}

void CJNIXBMCMediaSession::OnRewindRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
  {
    if (!g_application.GetAppPlayer().IsPaused())
      KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_PLAYER_REWIND)));
  }
}

void CJNIXBMCMediaSession::OnStopRequested()
{
  if (g_application.GetAppPlayer().IsPlaying())
    KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_GUI_ACTION, WINDOW_INVALID, -1, static_cast<void*>(new CAction(ACTION_STOP)));
}

void CJNIXBMCMediaSession::OnSeekRequested(int64_t pos)
{
  g_application.SeekTime(pos / 1000.0);
}

bool CJNIXBMCMediaSession::isActive() const
{
  return m_isActive;
}

/**********************/

void CJNIXBMCMediaSession::_onPlayRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnPlayRequested();
}

void CJNIXBMCMediaSession::_onPauseRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnPauseRequested();
}

void CJNIXBMCMediaSession::_onNextRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnNextRequested();
}

void CJNIXBMCMediaSession::_onPreviousRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnPreviousRequested();
}

void CJNIXBMCMediaSession::_onForwardRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnForwardRequested();
}

void CJNIXBMCMediaSession::_onRewindRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnRewindRequested();
}

void CJNIXBMCMediaSession::_onStopRequested(JNIEnv* env, jobject thiz)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnStopRequested();
}

void CJNIXBMCMediaSession::_onSeekRequested(JNIEnv* env, jobject thiz, jlong pos)
{
  (void)env;

  CJNIXBMCMediaSession *inst = find_instance(thiz);
  if (inst)
    inst->OnSeekRequested(pos);
}
#endif // !defined(HAVE_QT_GUI)
