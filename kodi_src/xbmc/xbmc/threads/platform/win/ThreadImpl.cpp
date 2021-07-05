/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include <windows.h>
#include <process.h>
#include "platform/win32/WIN32Util.h"
#include "utils/log.h"

void CThread::SpawnThread(unsigned stacksize)
{
  // Create in the suspended state, so that no matter the thread priorities and scheduled order, the handle will be assigned
  // before the new thread exits.
  unsigned threadId;
  m_ThreadOpaque.handle = (HANDLE)_beginthreadex(NULL, stacksize, &staticThread, this, CREATE_SUSPENDED, &threadId);
  if (m_ThreadOpaque.handle == NULL)
  {
    CLog::Log(LOGERROR, "%s - fatal error %d creating thread", __FUNCTION__, GetLastError());
    return;
  }
  m_ThreadId = threadId;

  if (ResumeThread(m_ThreadOpaque.handle) == -1)
    CLog::Log(LOGERROR, "%s - fatal error %d resuming thread", __FUNCTION__, GetLastError());

}

void CThread::TermHandler()
{
  CloseHandle(m_ThreadOpaque.handle);
  m_ThreadOpaque.handle = NULL;
}

void CThread::SetThreadInfo()
{
  const unsigned int MS_VC_EXCEPTION = 0x406d1388;

#pragma pack(push,8)
  struct THREADNAME_INFO
  {
    DWORD dwType; // must be 0x1000
    LPCSTR szName; // pointer to name (in same addr space)
    DWORD dwThreadID; // thread ID (-1 caller thread)
    DWORD dwFlags; // reserved for future use, most be zero
  } info;
#pragma pack(pop)

  info.dwType = 0x1000;
  info.szName = m_ThreadName.c_str();
  info.dwThreadID = m_ThreadId;
  info.dwFlags = 0;

  __try
  {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR *)&info);
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
  }

  CWIN32Util::SetThreadLocalLocale(true); // avoid crashing with setlocale(), see https://connect.microsoft.com/VisualStudio/feedback/details/794122
}

ThreadIdentifier CThread::GetCurrentThreadId()
{
  return ::GetCurrentThreadId();
}

ThreadIdentifier CThread::GetDisplayThreadId(const ThreadIdentifier tid)
{
  return tid;
}

bool CThread::IsCurrentThread(const ThreadIdentifier tid)
{
  return (::GetCurrentThreadId() == tid);
}

int CThread::GetMinPriority(void)
{
  return(THREAD_PRIORITY_IDLE);
}

int CThread::GetMaxPriority(void)
{
  return(THREAD_PRIORITY_HIGHEST);
}

int CThread::GetNormalPriority(void)
{
  return(THREAD_PRIORITY_NORMAL);
}

int CThread::GetSchedRRPriority(void)
{
  return GetNormalPriority();
}

bool CThread::SetPriority(const int iPriority)
{
  bool bReturn = false;

  CSingleLock lock(m_CriticalSection);
  if (m_ThreadOpaque.handle)
  {
    bReturn = SetThreadPriority(m_ThreadOpaque.handle, iPriority) == TRUE;
  }

  return bReturn;
}

int CThread::GetPriority()
{
  CSingleLock lock(m_CriticalSection);

  int iReturn = THREAD_PRIORITY_NORMAL;
  if (m_ThreadOpaque.handle)
  {
    iReturn = GetThreadPriority(m_ThreadOpaque.handle);
  }
  return iReturn;
}

bool CThread::WaitForThreadExit(unsigned int milliseconds)
{
  bool bReturn = true;

  CSingleLock lock(m_CriticalSection);
  if (m_ThreadId && m_ThreadOpaque.handle != NULL)
  {
    // boost priority of thread we are waiting on to same as caller
    int callee = GetThreadPriority(m_ThreadOpaque.handle);
    int caller = GetThreadPriority(::GetCurrentThread());
    if(caller != THREAD_PRIORITY_ERROR_RETURN && caller > callee)
      SetThreadPriority(m_ThreadOpaque.handle, caller);

    lock.Leave();
    bReturn = m_TermEvent.WaitMSec(milliseconds);
    lock.Enter();

    // restore thread priority if thread hasn't exited
    if(callee != THREAD_PRIORITY_ERROR_RETURN && caller > callee && m_ThreadOpaque.handle)
      SetThreadPriority(m_ThreadOpaque.handle, callee);
  }
  return bReturn;
}

int64_t CThread::GetAbsoluteUsage()
{
#ifdef TARGET_WINDOWS_STORE
  // GetThreadTimes is available since 10.0.15063 only
  return 0;
#else
  CSingleLock lock(m_CriticalSection);

  if (!m_ThreadOpaque.handle)
    return 0;

  uint64_t time = 0;
  FILETIME CreationTime, ExitTime, UserTime, KernelTime;
  if( GetThreadTimes(m_ThreadOpaque.handle, &CreationTime, &ExitTime, &KernelTime, &UserTime ) )
  {
    time = (((uint64_t)UserTime.dwHighDateTime) << 32) + ((uint64_t)UserTime.dwLowDateTime);
    time += (((uint64_t)KernelTime.dwHighDateTime) << 32) + ((uint64_t)KernelTime.dwLowDateTime);
  }
  return time;
#endif
}

float CThread::GetRelativeUsage()
{
  unsigned int iTime = XbmcThreads::SystemClockMillis();
  iTime *= 10000; // convert into 100ns tics

  // only update every 1 second
  if( iTime < m_iLastTime + 1000*10000 ) return m_fLastUsage;

  int64_t iUsage = GetAbsoluteUsage();

  if (m_iLastUsage > 0 && m_iLastTime > 0)
    m_fLastUsage = (float)( iUsage - m_iLastUsage ) / (float)( iTime - m_iLastTime );

  m_iLastUsage = iUsage;
  m_iLastTime = iTime;

  return m_fLastUsage;
}

void CThread::SetSignalHandlers()
{
}
