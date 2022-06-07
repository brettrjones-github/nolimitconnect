//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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
// http://www.nolimitconnect.org
//============================================================================


#include "AppCommon.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"

#include "ActivityPermissions.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::onMenuServerSelected( int iMenuId, PopupMenu *, ActivityBase * )
{
	switch( iMenuId )
	{

	case 1: //Start/Stop Web Cam Server
		switch( getEngine().fromGuiGetPluginServerState(ePluginTypeCamServer))
		{
		case ePluginServerStateStarted:	// app has started
			getEngine().setHasSharedWebCam(false);
            getEngine().fromGuiStopPluginSession( ePluginTypeCamServer, m_UserMgr.getMyOnlineId(), 0 );
			break;
		case ePluginServerStateStopped:
			startActivity( ePluginTypeCamServer, m_UserMgr.getMyIdent() );
			getEngine().setHasSharedWebCam(true);
			break;
		case ePluginServerStateDisabled:	// disabled by permissions
			{
				ActivityPermissions * poDlg = new ActivityPermissions( *this, getCentralWidget() );
				poDlg->setPluginType(ePluginTypeCamServer);
				poDlg->exec();
			}
			break;
        default:
            break;
		}

		break;

	default: 
		LogMsg( LOG_ERROR, "onMenuServerSelected: ERROR unknown menu id %d", iMenuId );
		break;
	}
}
