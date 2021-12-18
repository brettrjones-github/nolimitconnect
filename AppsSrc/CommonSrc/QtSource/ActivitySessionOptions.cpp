//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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


#include "GuiOfferSession.h"
#include "ActivitySessionOptions.h"
#include "AppGlobals.h"
#include "MyIcons.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
ActivitySessionOptions::ActivitySessionOptions(	AppCommon&			app,
												VxNetIdent *			netIdent, 
												QWidget *				parent )
: ActivityBase( OBJNAME_ACTIVITY_SESSION_OPTIONS, app, parent )
{
	Q_UNUSED( netIdent )
	setup();
	//sendPluginOffer( NULL, NULL, NULL );
}

//============================================================================
ActivitySessionOptions::ActivitySessionOptions(	AppCommon&				app,
												GuiOfferSession *		poOffer, 
												QWidget *				parent )
: ActivityBase( OBJNAME_ACTIVITY_SESSION_OPTIONS, app, parent )
{
	Q_UNUSED( poOffer )
	setup();
}

//============================================================================
void ActivitySessionOptions::setup()
{
	//setContentWidget( &m_VideoChatWidget );
}

//============================================================================
void ActivitySessionOptions::playVideoFrame( unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	//m_VideoChatWidget.playVideoFrame( pu8Jpg, u32JpgLen, motion0To100000 );
}
