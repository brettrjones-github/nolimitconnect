//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#include <app_precompiled_hdr.h>
#include "AppletChatRoomJoin.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletChatRoomJoin::AppletChatRoomJoin( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_CHAT_ROOM_JOIN, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletChatRoomJoin );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );

	m_MyApp.activityStateChange( this, true );
}


//============================================================================
AppletChatRoomJoin::~AppletChatRoomJoin()
{
    m_MyApp.activityStateChange( this, false );
}