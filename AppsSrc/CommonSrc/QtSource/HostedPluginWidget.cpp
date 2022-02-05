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

#include "HostedPluginWidget.h"
#include "GuiHostSession.h"
#include "GuiParams.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>

//============================================================================
HostedPluginWidget::HostedPluginWidget( QWidget *parent )
: IdentLogicInterface( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    setupIdentLogic();
    connect( ui.m_JoinButton,		SIGNAL(pressed()),	this, SLOT(slotJoinButtonPressed()) );
    connect( ui.m_ConnectButton,    SIGNAL( pressed() ), this, SLOT( slotConnectButtonPressed() ) );
   
    ui.m_MenuButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeMedium ) );
    ui.m_MenuButton->setIcon( eMyIconMenu );
    ui.m_JoinButton->setIcon( eMyIconPersonAdd );
    ui.m_MenuButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_ConnectButton->setIcon( eMyIconConnect );
    ui.m_ConnectButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    showConnectButton( false );
}

//============================================================================
HostedPluginWidget::~HostedPluginWidget()
{
}

//============================================================================
MyIcons& HostedPluginWidget::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostedPluginWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostedPluginWidget::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
}

//============================================================================
void HostedPluginWidget::setHostSession( GuiHostSession* hostSession )
{
    m_HostSession = hostSession;
}

//============================================================================
GuiHostSession * HostedPluginWidget::getHostSession( void )
{
    return m_HostSession;
}

//============================================================================
void HostedPluginWidget::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostedPluginWidget::slotIconButtonClicked" );
}

//============================================================================
void HostedPluginWidget::onIdentMenuButtonClicked( void )
{
}

//============================================================================
void HostedPluginWidget::updateWidgetFromInfo( void )
{
    GuiHostSession* hostSession = getHostSession();
    if( nullptr == hostSession )
    {
        return;
    }

    GuiUser* hostIdent = hostSession->getUserIdent();
    if( hostIdent )
    {
        updateIdentity( hostIdent );
    }

    if( m_MyApp.getEngine().getMyOnlineId() == hostIdent->getMyOnlineId() )
    {
        ui.m_TitlePart2->setText( QObject::tr( " Hosted By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }

    if( !ui.m_IconButton->hasImage() )
    {
        VxGUID thumbId = hostSession->getHostThumbId();
        if( !thumbId.isVxGUIDValid() )
        {
            thumbId = hostIdent->getHostThumbId( hostSession->getHostType(), true );
        }
       
        if( thumbId.isVxGUIDValid() )
        {
            QImage thumbImage;
            if( m_MyApp.getThumbMgr().getThumbImage( thumbId, thumbImage ) )
            {
                ui.m_IconButton->setIconOverrideImage( thumbImage );
            }
        }
    }

    // set text of line 2
    std::string strDesc = hostSession->getHostDescription();
    if( strDesc.empty() )
    {
        strDesc = hostIdent->getOnlineDescription();
    }

    if( !strDesc.empty() )
    {
        ui.m_DescPart2->setText( strDesc.c_str() );
    }
}

//============================================================================
void HostedPluginWidget::setJoinedState( EJoinState joinState )
{
    // todo update join 
    switch( joinState )
    {
    case eJoinStateJoinWasGranted:
        ui.m_ConnectButton->setIcon( eMyIconConnect );
        showConnectButton( true );
        break;
    case eJoinStateJoinIsGranted:
        ui.m_ConnectButton->setIcon( eMyIconDisconnect );
        showConnectButton( true );
        break;
    case eJoinStateSending:
    case eJoinStateSendFail:
    case eJoinStateSendAcked:
    case eJoinStateJoinRequested:
    case eJoinStateJoinDenied:
    case eJoinStateNone:
    default:
        showConnectButton( false );
        break;
    }
}

//============================================================================
void HostedPluginWidget::showConnectButton( bool isAccepted )
{
    ui.m_JoinButton->setVisible( !isAccepted );
    ui.m_JoinLabel->setVisible( !isAccepted );
    ui.m_ConnectButton->setVisible( isAccepted );
    ui.m_ConnectLabel->setVisible( isAccepted );
}

//============================================================================
void HostedPluginWidget::slotJoinButtonPressed( void )
{
    LogMsg( LOG_DEBUG, "HostedPluginWidget::slotJoinButtonPressed" );
}

//============================================================================
void HostedPluginWidget::slotConnectButtonPressed( void )
{
    LogMsg( LOG_DEBUG, "HostedPluginWidget::slotConnectButtonPressed" );
}
