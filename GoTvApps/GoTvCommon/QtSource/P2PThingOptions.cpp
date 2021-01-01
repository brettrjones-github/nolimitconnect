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

#include <app_precompiled_hdr.h>

#include "PopupMenu.h"
#include "ActivitySoundOptions.h"
#include "ActivityPermissions.h"
#include "ActivityPermissionsRelay.h"
#include "ActivityNetworkSettings.h"
#include "ActivityNetServiceTest.h"
#include "ActivityVideoOptions.h"
#include "ActivityToFriendMultiSession.h"
#include "ActivitySelectFileToSend.h"
#include "GuiHelpers.h"

#include <GoTvInterface/IFromGui.h>

#include <CoreLib/VxGlobals.h>

/*
//============================================================================
void AppCommon::slotOptionsButtonClick( void )
{
	PopupMenu popupMenu( *this, this );
	popupMenu.setTitle( "Options");
	popupMenu.addMenuItem( 1, getMyIcons().getIcon( eMyIconPermissions),  QObject::tr( "Edit Plugin Permissions" ) );
	popupMenu.addMenuItem( 2, getMyIcons().getIcon( eMyIconSpeakerOn ),  QObject::tr( "Sound Options" ) );
	//popupMenu.addMenuItem( 6, getMyIcons().getIcon( eMyIconDebug ),  QObject::tr( "Debug Only" ) );
	//popupMenu.addMenuItem( 8, getMyIcons().getIcon( eMyIconFolder ), "Setup file share" );
	//popupMenu.addMenuItem( 12, getMyIcons().getIcon( eMyIconDebug ), "Debug Settings" );

	connect( &popupMenu, SIGNAL(menuItemClicked(int, PopupMenu *, ActivityBase *)), this, SLOT(onMenuOptionsSelected(int, PopupMenu *, ActivityBase *)));
	popupMenu.exec();
}*/

//============================================================================
void AppCommon::addPermissionMenuEntry(	PopupMenu *		poPopupMenu, 
											EPluginType		ePluginType, 
											EMyIcons		eIconType, 
											const char *	pText )
{
	int iPluginPermission = m_Engine.getPluginPermission( (int)ePluginType );
	
	char as8Buf[255];
	sprintf(as8Buf, pText, GuiHelpers::describePluginPermission( (EFriendState) iPluginPermission ) );
	poPopupMenu->addMenuItem( ePluginType, getMyIcons().getIcon(eIconType), as8Buf );
}

//============================================================================
void AppCommon::onMenuOptionsSelected( int iMenuId, PopupMenu * senderPopupMenu, ActivityBase * activityBase )
{
	std::string strTmp;
	PopupMenu popupMenu( *this, activityBase );
	switch( iMenuId )
	{

	case 1: // edit permissions
		{
			popupMenu.setTitle( "Edit Access Permissions");
			// file share plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeFileServer, eMyIconShareFilesNormal, "(%s) Who can access shared files" );
			// file offer plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeFileXfer, eMyIconSendFileNormal, "(%s) Who can offer to send a file" );
			// web cam broadcast plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeCamServer, eMyIconWebCamServer, "(%s) Who can access web cam broadcast" );
			// instant message p2p plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeMessenger, eMyIconMultiSession, "(%s) Who can offer chat session" );
			// VOIP p2p plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeVoicePhone, eMyIconVoicePhoneNormal, "(%s) Who can offer voice phone call" );
			// Video phone p2p plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeVideoPhone, eMyIconVideoPhoneNormal, "(%s) Who can offer video chat" );
			// Web Cam Truth Or Dare game p2p plugin
			addPermissionMenuEntry( &popupMenu, ePluginTypeTruthOrDare, eMyIconTruthOrDareNormal, "(%s) Who can offer to play truth or dare" );
			// Relay
			addPermissionMenuEntry( &popupMenu, ePluginTypeRelay, eMyIconRelay, "(%s) Who can use my device as a relay" );
			// Profile 
			addPermissionMenuEntry( &popupMenu, ePluginTypeWebServer, eMyIconProfile, "(%s) Who can view my profile" );
			// Storyboard
			addPermissionMenuEntry( &popupMenu, ePluginTypeStoryboard, eMyIconStoryBoardNormal, "(%s) Who can view my storyboard" );
            connect( &popupMenu, SIGNAL(menuItemClicked(int, PopupMenu *, ActivityBase *)), this, SLOT(onEditPermissionsSelected(int, PopupMenu *, ActivityBase *)));

			popupMenu.exec();
			break;
		}

	case 2: //Sound Options
		{
			ActivitySoundOptions oDlg( *this, this );
			oDlg.exec();
			break;
		}

	//case 7: //eTestParamSoundDelayTest
	//	{
	//		m_Engine.fromGuiTestCmd( IFromGui::eTestParamSoundDelayTest );
	//	}
	//	break;

	//case 12: //Debug settings
	//	{
	//		//m_DebugSettingsDlg->show();
	//		senderPopupMenu->setVisible( false );
	//		ActivitySelectFileToSend selectFileDlg(	*this,
	//												GetAppGlobals()->getUserIdent(), 
	//												getCentralWidget() );
	//		selectFileDlg.exec();
	//	}
	//	break;

	default: 
		LogMsg( LOG_INFO, "onMenuServerSelected: unknown id %d\n", iMenuId );
	}
}

//============================================================================
//! user selected permission to edit
void AppCommon::onEditPermissionsSelected( int iMenuId, PopupMenu * senderPopupMenu, ActivityBase * )
{
	Q_UNUSED( senderPopupMenu );
	if( iMenuId > 0 )
	{
		if( ePluginTypeRelay == iMenuId )
		{
			ActivityPermissionsRelay oDlg( *this, this );
			oDlg.setPluginType( (EPluginType) iMenuId );
			oDlg.exec();
		}
		else
		{
			ActivityPermissions oDlg( *this, this );
			oDlg.setPluginType( (EPluginType) iMenuId );
			oDlg.exec();
		}
	}
}
