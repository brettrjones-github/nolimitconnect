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
// http://www.nolimitconnect.org
//============================================================================

#include "GuiOfferSession.h"
#include "AppletPeerBase.h"
#include "ActivityMessageBox.h"
#include "IdentWidget.h"

#include "AppGlobals.h"
#include "MyIcons.h"
#include "SoundMgr.h"
#include "AppCommon.h"
#include "VidWidget.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
AppletPeerBase::AppletPeerBase(	const char* objName, AppCommon& app, QWidget* parent )
: AppletBase( objName, app, parent )
, m_OfferSessionLogic( this, this, app )
{
	setupAppletPeerBase();
}

//============================================================================
AppletPeerBase::~AppletPeerBase()
{
}

//============================================================================
void AppletPeerBase::onActivityFinish( void )
{
	m_OfferSessionLogic.onStop();
}

//============================================================================
void AppletPeerBase::setupAppletPeerBase( void )
{
    connect( &m_UserMgr, SIGNAL(signalUserAdded(GuiUser*)),	                this, SLOT(slotUserAdded(GuiUser*)) );
    connect( &m_UserMgr, SIGNAL(signalUserRemoved(VxGUID)),	                this, SLOT(slotUserRemoved(VxGUID)) );
    connect( &m_UserMgr, SIGNAL(signalUserUpdated(GuiUser*)),	            this, SLOT(slotUserUpdated(GuiUser*)) );
    connect( &m_UserMgr, SIGNAL(signalUserOnlineStatus(GuiUser*)),			this, SLOT(slotUserOnlineStatus(GuiUser*)) );

	m_Engine.fromGuiMuteMicrophone( false );
	m_Engine.fromGuiMuteSpeaker( false );
	m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletPeerBase::slotUserAdded( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "AppletPeerBase::slotUserAdded" );
}

//============================================================================
void AppletPeerBase::slotUserRemoved( VxGUID onlineId )
{
    LogMsg( LOG_DEBUG, "AppletPeerBase::slotUserRemoved" );
}

//============================================================================
void AppletPeerBase::slotUserUpdated( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "AppletPeerBase::slotUserUpdated" );
}

//============================================================================
void AppletPeerBase::slotUserOnlineStatus( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "AppletPeerBase::slotUserOnlineStatus" );
    if( !user->isOnline() )
    {
        m_OfferSessionLogic.toGuiContactOffline( user );
    }
}

//============================================================================
void AppletPeerBase::setupBaseWidgets( IdentWidget*	friendIdentWidget, VxPushButton* permissionButton, QLabel* permissionLabel )
{
	if( friendIdentWidget )
	{
		friendIdentWidget->updateIdentity( m_HisIdent );
	}

	if( 0 != permissionButton && m_HisIdent )
	{
		EPluginAccess ePluginAccess = m_HisIdent->getMyAccessPermissionFromHim( m_ePluginType );

		permissionButton->setIcon( getMyIcons().getPluginIcon( m_ePluginType, ePluginAccess ) );
		QString strAction = DescribePluginType( m_ePluginType );
		if( 0 != permissionLabel )
		{
			permissionLabel->setText( strAction );	
		}
	}
}

//============================================================================
void AppletPeerBase::showEvent( QShowEvent* ev )
{
	AppletBase::showEvent( ev );
    if( ePluginTypeInvalid == m_ePluginType )
    {
        m_MyApp.getPluginMgr().setPluginVisible( m_ePluginType, true );
        m_MyApp.wantToGuiActivityCallbacks( this, true );
    }
}

//============================================================================
void AppletPeerBase::hideEvent( QHideEvent* ev )
{
    if( ePluginTypeInvalid == m_ePluginType )
    {
        m_MyApp.getPluginMgr().setPluginVisible( m_ePluginType, false );
        m_MyApp.wantToGuiActivityCallbacks( this, false );
    }

    AppletBase::hideEvent( ev );
}

//============================================================================   
void AppletPeerBase::closeEvent( QCloseEvent * ev )
{
	m_OfferSessionLogic.onStop();
    AppletBase::closeEvent( ev );
}

//============================================================================
bool AppletPeerBase::fromGuiSetGameValueVar(	int32_t			s32VarId, 
													int32_t			s32VarValue )
{
	bool bResult = m_FromGui.fromGuiSetGameValueVar(	m_ePluginType, 
														m_HisIdent->getMyOnlineId(),
														s32VarId,
														s32VarValue );
	if( false == bResult )
	{
		m_OfferSessionLogic.handleUserWentOffline();
	}

	return bResult;
}

//============================================================================
bool AppletPeerBase::fromGuiSetGameActionVar(	int32_t			s32VarId, 
													int32_t			s32VarValue )
{
	bool bResult = m_FromGui.fromGuiSetGameActionVar(	m_ePluginType, 
														m_HisIdent->getMyOnlineId(),
														s32VarId,
														s32VarValue );
	if( false == bResult )
	{
		m_OfferSessionLogic.handleUserWentOffline();
	}

	return bResult;
}

//============================================================================
void AppletPeerBase::toGuiRxedPluginOffer(GuiOfferSession * offer )
{
	//m_OfferSessionLogic.toGuiRxedPluginOffer( offer );
}; 

//============================================================================
void AppletPeerBase::toGuiRxedOfferReply( GuiOfferSession * offerReply )
{
	//m_OfferSessionLogic.toGuiRxedOfferReply( offerReply );
}; 

//============================================================================
void AppletPeerBase::toGuiPluginSessionEnded( GuiOfferSession * offer )
{
	//handleSessionEnded( offerResponse, this );
}; 

//============================================================================
void AppletPeerBase::callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 )
{
	if( m_VidCamWidget && m_VidCamWidget->isVisible() )
	{
		m_VidCamWidget->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
	}
}

//============================================================================
void AppletPeerBase::toGuiSetGameValueVar(	EPluginType     ePluginType, 
											VxGUID&		    onlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		//emit signalToGuiSetGameValueVar( s32VarId, s32VarValue );
	}
}

//============================================================================
void AppletPeerBase::toGuiSetGameActionVar(	EPluginType     ePluginType, 
											VxGUID&		    onlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		//emit signalToGuiSetGameActionVar( s32VarId, s32VarValue );
	}
}
