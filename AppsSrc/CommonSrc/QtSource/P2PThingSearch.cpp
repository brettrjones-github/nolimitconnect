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
// http://www.nolimitconnect.com
//============================================================================


#include "AppletPopupMenu.h"
#include "AppCommon.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "ActivityScanPeopleSearch.h"
#include "ActivityScanProfiles.h"
#include "ActivityScanWebCams.h"
#include "ActivityFileSearch.h"

/*
//============================================================================
void AppCommon::slotSearchButtonClick( void )
{
	PopupMenu oPopupMenu( *this, this );
	oPopupMenu.setTitle( QObject::tr( "Search" ) );
	oPopupMenu.addMenuItem( 1, getMyIcons().getIcon(eMyIconGuest), "Search For Person By Name");
	oPopupMenu.addMenuItem( 2, getMyIcons().getIcon(eMyIconGuest), "Search For Person By Mood Message");
	oPopupMenu.addMenuItem( 3, getMyIcons().getIcon(eMyIconProfile), "Scan Profile Pictures");
	oPopupMenu.addMenuItem( 4, getMyIcons().getIcon(eMyIconWebCamServer), "Scan Shared Web Cams");
	oPopupMenu.addMenuItem( 5, getMyIcons().getIcon(eMyIconFolder), "Search For Shared Files");
	connect( &oPopupMenu, SIGNAL(menuItemClicked(int, PopupMenu *, ActivityBase *)), this, SLOT(onMenuSearchSelected(int, PopupMenu *, ActivityBase *)));
	oPopupMenu.exec();
}
*/

//============================================================================
void AppCommon::onMenuSearchSelected( int iMenuId, PopupMenu * senderPopupMenu, ActivityBase * )
{
	Q_UNUSED( senderPopupMenu );
	switch( iMenuId )
	{
	case 1: //For Person By Name
		{
			ActivityScanPeopleSearch oDlg( *this, eScanTypePeopleSearch, this );
			oDlg.exec();
			break;
		}

	case 2: //For Person By Description
		{
			ActivityScanPeopleSearch oDlg( *this, eScanTypeMoodMsgSearch, this );
			oDlg.exec();
			break;
		}

	case 3: //Scan Profiles
		{
			ActivityScanProfiles oDlg( *this, this );
			oDlg.exec();
			break;
		}

	case 4: //Scan Web Cams
		{
			ActivityScanWebCams oDlg( *this, this );
			oDlg.exec();
			break;
		}

	case 5: //Shared Files
		{
			ActivityFileSearch oDlg( *this, this );
			oDlg.exec();
			break;
		}

	default:
		LogMsg( LOG_ERROR, "onMenuSearchSelected: ERROR unknown menu id %d\n", iMenuId );
	}
}
