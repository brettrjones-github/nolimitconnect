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
#include <app_precompiled_hdr.h>
#include "GuiOfferSession.h"
#include "AppletPeerBase.h"
#include "ActivityMessageBox.h"
#include "IdentWidget.h"

#include "AppGlobals.h"
#include "MyIcons.h"
#include "MySndMgr.h"
#include "AppCommon.h"
#include "VidWidget.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
AppletPeerBase::AppletPeerBase(	const char * objName, AppCommon& app, QWidget * parent )
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
	connect( this, SIGNAL(signalToGuiRxedPluginOffer(GuiOfferSession *)),		this,	SLOT(slotToGuiRxedPluginOffer(GuiOfferSession *)), Qt::QueuedConnection );
	connect( this, SIGNAL(signalToGuiRxedOfferReply(GuiOfferSession *)),		this,	SLOT(slotToGuiRxedOfferReply(GuiOfferSession *)), Qt::QueuedConnection );
	connect( this, SIGNAL(signalToGuiPluginSessionEnded(GuiOfferSession *)),	this,	SLOT(slotToGuiPluginSessionEnded(GuiOfferSession *)), Qt::QueuedConnection );
	connect( this, SIGNAL(signalToGuiContactOffline(VxNetIdent *)),				this,	SLOT(slotToGuiContactOffline(VxNetIdent *)), Qt::QueuedConnection );

	m_Engine.fromGuiMuteMicrophone( false );
	m_Engine.fromGuiMuteSpeaker( false );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletPeerBase::setupBaseWidgets( IdentWidget*	friendIdentWidget, VxPushButton* permissionButton, QLabel* permissionLabel )
{
	if( friendIdentWidget )
	{
		friendIdentWidget->updateGuiFromData( m_HisIdent );
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
void AppletPeerBase::showEvent( QShowEvent * ev )
{
	AppletBase::showEvent( ev );
    if( ePluginTypeInvalid == m_ePluginType )
    {
        m_MyApp.setPluginVisible( m_ePluginType, true );
        m_MyApp.wantToGuiActivityCallbacks( this, this, true );
    }
}

//============================================================================
void AppletPeerBase::hideEvent( QHideEvent * ev )
{
    if( ePluginTypeInvalid == m_ePluginType )
    {
        m_MyApp.setPluginVisible( m_ePluginType, false );
        m_MyApp.wantToGuiActivityCallbacks( this, this, false );
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
void AppletPeerBase::toGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer )
{
	Q_UNUSED( callbackData );
	emit signalToGuiRxedPluginOffer( offer );
}

//============================================================================
void AppletPeerBase::slotToGuiRxedPluginOffer( GuiOfferSession * offer )
{
	//m_OfferSessionLogic.toGuiRxedPluginOffer( offer );
}; 

//============================================================================
void AppletPeerBase::toGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerReply )
{
	Q_UNUSED( callbackData );
	emit signalToGuiRxedOfferReply( offerReply );
} 

//============================================================================
void AppletPeerBase::slotToGuiRxedOfferReply( GuiOfferSession * offerReply )
{
	//m_OfferSessionLogic.toGuiRxedOfferReply( offerReply );
}; 

//============================================================================
void AppletPeerBase::toGuiPluginSessionEnded(void * callbackData, GuiOfferSession * offer )
{
	Q_UNUSED( callbackData );
	emit signalToGuiPluginSessionEnded( offer );
}; 

//============================================================================
void AppletPeerBase::slotToGuiPluginSessionEnded( GuiOfferSession * offer )
{
	//handleSessionEnded( offerResponse, this );
}; 

//============================================================================
void AppletPeerBase::toGuiContactOnline( void * callbackData, VxNetIdent * netIdent, bool newContact )
{
	Q_UNUSED( callbackData );
	emit signalToGuiContactOnline( netIdent, newContact );
}; 

//============================================================================
void AppletPeerBase::slotToGuiContactOffline( VxNetIdent * netIdent )
{
	m_OfferSessionLogic.toGuiContactOffline( netIdent );
}

//============================================================================
void AppletPeerBase::toGuiContactOffline( void * callbackData, VxNetIdent * friendIdent )
{
	Q_UNUSED( callbackData );
	emit signalToGuiContactOffline( friendIdent );
}; 

//============================================================================
void AppletPeerBase::toGuiClientPlayVideoFrame(	void *			userData, 
														VxGUID&			onlineId, 
														uint8_t *			pu8Jpg, 
														uint32_t				u32JpgDataLen,
														int				motion0To100000 )
{
	if( m_VidCamWidget && m_VidCamWidget->isVisible() )
	{
		m_VidCamWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
	}
}

//============================================================================
void AppletPeerBase::toGuiSetGameValueVar(	void *		userData, 
													EPluginType ePluginType, 
													VxGUID&		onlineId, 
													int32_t			s32VarId, 
													int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		emit signalToGuiSetGameValueVar( s32VarId, s32VarValue );
	}
}

//============================================================================
void AppletPeerBase::toGuiSetGameActionVar(	void *		userData, 
													EPluginType ePluginType, 
													VxGUID&		onlineId, 
													int32_t			s32VarId, 
													int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		emit signalToGuiSetGameActionVar( s32VarId, s32VarValue );
	}
}

//============================================================================
QString AppletPeerBase::describePluginOffer( EPluginType ePluginType )
{
	QString strPluginOffer;
	switch( ePluginType )
	{
	case ePluginTypeRelay:	// proxy plugin
		strPluginOffer = QObject::tr(" Relay ");
		break;

	case ePluginTypeWebServer:	// web server plugin ( for profile web page )
		strPluginOffer = QObject::tr(" View Profile Page " );
		break;

	case ePluginTypeFileXfer:	// file offer plugin
		strPluginOffer = QObject::tr(" Receive A File " );
		break;

	case ePluginTypeFileServer:	// file share plugin
		strPluginOffer = QObject::tr(" View Shared Files " );
		break;

	case ePluginTypeCamServer:	// web cam broadcast plugin
		strPluginOffer = QObject::tr(" View Shared Web Cam ");
		break;

	case ePluginTypeMessenger:	// multi session chat plugin
		strPluginOffer = QObject::tr(" Join Chat Session " );
		break;

	case ePluginTypeVoicePhone:	// VOIP p2p plugin
		strPluginOffer = QObject::tr(" Voice Phone Call ");
		break;

	case ePluginTypeVideoPhone:
		strPluginOffer = QObject::tr(" Video Chat Offer ");
		break;

	case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		strPluginOffer = QObject::tr(" Play Truth Or Dare ");
		break;

	case ePluginTypeStoryboard:	// story board plugin
		strPluginOffer = QObject::tr(" View Story Board ");
		break;

	default:
		strPluginOffer = QObject::tr("Unknown Plugin Offer");
		LogMsg( LOG_ERROR, "AppletPeerBase::describePluginOffer: unrecognized plugin %d\n", ePluginType );
	}

	return strPluginOffer;
}
