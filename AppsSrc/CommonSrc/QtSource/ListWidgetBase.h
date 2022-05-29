#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

class AppCommon;
class GuiUser;
class GuiUserSessionBase;
class GuiUserMgr;
class GuiThumb;
class GuiThumbMgr;
class MyIcons;
class P2PEngine;

class ListWidgetBase : public QListWidget
{
    Q_OBJECT

public:
    ListWidgetBase( QWidget* parent );

    AppCommon&                  getMyApp( void )                    { return m_MyApp; }
    MyIcons&                    getMyIcons( void )                  { return m_MyIcons; }
    GuiUserMgr&                 getUserMgr( void )                  { return m_UserMgr; }
    P2PEngine&                  getEngine( void )                   { return m_Engine; }
    GuiThumbMgr&                getThumbMgr( void )                 { return m_ThumbMgr; }

    void                        setIsHostView( bool isHost )        { m_IsHostView = isHost; }
    bool                        getIsHostView( void )               { return m_IsHostView; }
    void                        setHostType( EHostType hostType )   { m_HostType = hostType; }
    EHostType                   getHostType( void )                 { return m_HostType; }

    void                        launchChangeFriendship( GuiUser* ident );

protected:
    //=== vars ===//
    AppCommon&                  m_MyApp;
    MyIcons&                    m_MyIcons;
    GuiUserMgr&                 m_UserMgr;
    P2PEngine&                  m_Engine;
    GuiThumbMgr&                m_ThumbMgr;
    bool                        m_IsHostView{ false };
    EHostType                   m_HostType{ eHostTypeUnknown };

    VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};

