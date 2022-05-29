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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletPersonalRecorder.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletPersonalRecorder::AppletPersonalRecorder( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_PERSONAL_RECORDER, app, parent )
{
    setAppletType( eAppletPersonalRecorder );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    setupMultiSessionActivity();
	m_MyApp.activityStateChange( this, true );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
AppletPersonalRecorder::~AppletPersonalRecorder()
{
    ui.m_SessionWidget->onActivityStop();
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletPersonalRecorder::setupMultiSessionActivity( void )
{
    ui.m_SessionWidget->setIsPersonalRecorder( true );
    ui.m_SessionWidget->setIdents( m_UserMgr.getMyIdent(), m_UserMgr.getMyIdent() );
    ui.m_SessionWidget->setEntryMode( eAssetTypeUnknown );
}

//============================================================================
void AppletPersonalRecorder::toGuiClientPlayVideoFrame( 
                                                        VxGUID&			onlineId,
                                                        uint8_t *		pu8Jpg,
                                                        uint32_t		u32JpgDataLen,
                                                        int				motion0To100000 )
{
    ui.m_SessionWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
}

