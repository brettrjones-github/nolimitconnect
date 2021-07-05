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

#pragma once
#include "config_kodi.h"
#ifdef HAVE_QT_GUI


#include <list>
#include <queue>
#include <vector>
#include <string>

#include "threads/Event.h"
#include "threads/Thread.h"
#include "threads/CriticalSection.h"
#include "windowing/WinEvents.h"

class CWinEventsQt : public IWinEvents, public CThread
{
public:
    CWinEventsQt();
    ~CWinEventsQt();
 
    void                        MessagePush( XBMC_Event *newEvent );
    void                        MessagePushRepeat( XBMC_Event *repeatEvent );
    bool                        MessagePump();

private:
    size_t                      GetQueueSize();

    // for CThread
    virtual void                Process();

    CCriticalSection            m_eventsCond;
    std::list<XBMC_Event>       m_events;

    CCriticalSection            m_lasteventCond;
    std::queue<XBMC_Event>      m_lastevent;
};

#endif // HAVE_QT_GUI
