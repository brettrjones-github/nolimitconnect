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
// http://www.nolimitconnect.com
//============================================================================

#include "ListWidgetBase.h"
#include "AppletPeerChangeFriendship.h"
#include "AppCommon.h"

//============================================================================
ListWidgetBase::ListWidgetBase( QWidget* parent )
    : QListWidget( parent )
    , m_MyApp( GetAppInstance() )
    , m_MyIcons( m_MyApp.getMyIcons() )
    , m_UserMgr( m_MyApp.getUserMgr() )
    , m_Engine( m_MyApp.getEngine() )
    , m_ThumbMgr( m_MyApp.getThumbMgr() )
{
    setFocusPolicy( Qt::NoFocus );
}

//============================================================================
void ListWidgetBase::launchChangeFriendship( GuiUser* guiUser )
{
    AppletPeerChangeFriendship* applet = dynamic_cast<AppletPeerChangeFriendship*>(m_MyApp.launchApplet( eAppletPeerChangeFriendship, dynamic_cast<QWidget*>(this->parent()) ));
    if( applet )
    {
        applet->setFriend( guiUser );
    }
}
