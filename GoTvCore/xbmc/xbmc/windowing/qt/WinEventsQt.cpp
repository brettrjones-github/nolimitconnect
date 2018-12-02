/*
 *      Copyright (C) 2010-2013 Team XBMC
 *      http://kodi.tv
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
#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "WinEventsQt.h"

#include "AppInboundProtocol.h"
#include "guilib/GUIComponent.h"
#include "guilib/GUIWindowManager.h"
#include "input/InputManager.h"
#include "input/XBMC_vkeys.h"
#include "ServiceBroker.h"
#include "utils/log.h"

#define ALMOST_ZERO 0.125f
enum {
  EVENT_STATE_TEST,
  EVENT_STATE_HOLD,
  EVENT_STATE_REPEAT
};

/************************************************************************/
/************************************************************************/
static bool different_event(XBMC_Event &curEvent, XBMC_Event &newEvent)
{
  // different type
  if (curEvent.type != newEvent.type)
    return true;

  return false;
}

/************************************************************************/
/************************************************************************/
CWinEventsQt::CWinEventsQt()
: CThread("CWinEventsQt")
{
  CLog::Log(LOGDEBUG, "CWinEventsQt::CWinEventsQt");
  Create();
}

CWinEventsQt::~CWinEventsQt()
{
  m_bStop = true;
  StopThread(true);
}

void CWinEventsQt::MessagePush(XBMC_Event *newEvent)
{
  CSingleLock lock(m_eventsCond);

  m_events.push_back(*newEvent);
}

void CWinEventsQt::MessagePushRepeat(XBMC_Event *repeatEvent)
{
  CSingleLock lock(m_eventsCond);

  std::list<XBMC_Event>::iterator itt;
  for (itt = m_events.begin(); itt != m_events.end(); ++itt)
  {
    // we have events pending, if we we just
    // repush, we might push the repeat event
    // in back of a canceling non-active event.
    // do not repush if pending are different event.
    if (different_event(*itt, *repeatEvent))
      return;
  }

  // is a repeat, push it
  m_events.push_back(*repeatEvent);
}

bool CWinEventsQt::MessagePump()
{
  bool ret = false;

  // Do not always loop, only pump the initial queued count events. else if ui keep pushing
  // events the loop won't finish then it will block xbmc main message loop.
  for (size_t pumpEventCount = GetQueueSize(); pumpEventCount > 0; --pumpEventCount)
  {
    // Pop up only one event per time since in App::OnEvent it may init modal dialog which init
    // deeper message loop and call the deeper MessagePump from there.
    XBMC_Event pumpEvent;
    {
      CSingleLock lock(m_eventsCond);
      if (m_events.empty())
        return ret;
      pumpEvent = m_events.front();
      m_events.pop_front();
    }

    std::shared_ptr<CAppInboundProtocol> appPort = CServiceBroker::GetAppPort();
    if (appPort)
      ret |= appPort->OnEvent(pumpEvent);

    if (pumpEvent.type == XBMC_MOUSEBUTTONUP)
      CServiceBroker::GetGUI()->GetWindowManager().SendMessage(GUI_MSG_UNFOCUS_ALL, 0, 0, 0, 0);
  }

  return ret;
}

size_t CWinEventsQt::GetQueueSize()
{
  CSingleLock lock(m_eventsCond);
  return m_events.size();
}

void CWinEventsQt::Process()
{
  uint32_t timeout = 10;
  uint32_t holdTimeout = 500;
  uint32_t repeatTimeout = 100;
  uint32_t repeatDuration = 0;

  XBMC_Event cur_event;
  int state = EVENT_STATE_TEST;
  while (!m_bStop)
  {
    // run a 10ms (timeout) wait cycle
    Sleep(timeout);

    CSingleLock lock(m_lasteventCond);

    switch(state)
    {
      default:
      case EVENT_STATE_TEST:
        // non-active event, eat it
        if (!m_lastevent.empty())
          m_lastevent.pop();
        break;

      case EVENT_STATE_HOLD:
        repeatDuration += timeout;
        if (!m_lastevent.empty())
        {
          if (different_event(cur_event, m_lastevent.front()))
          {
            // different event, cycle back to test
            state = EVENT_STATE_TEST;
            break;
          }

          // same event, eat it
          m_lastevent.pop();
        }

        if (repeatDuration >= holdTimeout)
        {
          CLog::Log(LOGDEBUG, "hold  ->repeat, size(%d), repeatDuration(%d)", m_lastevent.size(), repeatDuration);
          state = EVENT_STATE_REPEAT;
        }
        break;

      case EVENT_STATE_REPEAT:
        repeatDuration += timeout;
        if (!m_lastevent.empty())
        {
          if (different_event(cur_event, m_lastevent.front()))
          {
            // different event, cycle back to test
            state = EVENT_STATE_TEST;
            break;
          }

          // same event, eat it
          m_lastevent.pop();
        }

        if (repeatDuration >= holdTimeout)
        {
          // this is a repeat, push it
          MessagePushRepeat(&cur_event);
          // assuming holdTimeout > repeatTimeout,
          // just subtract the repeatTimeout
          // to get the next cycle time
          repeatDuration -= repeatTimeout;
        }
        break;
    }
  }
}
#endif // HAVE_QT_GUI