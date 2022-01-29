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

#include "GuiHostedListItem.h"
#include "GuiHostedListSession.h"
#include "GuiParams.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>

//============================================================================
GuiHostedListItem::GuiHostedListItem(QWidget *parent  )
: IdentLogicInterface( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    setupIdentLogic();
    connect( ui.m_JoinButton,		SIGNAL( clicked() ),	this, SLOT( slotJoinButtonClicked() ) );
    connect( ui.m_ConnectButton,    SIGNAL( clicked() ),    this, SLOT( slotConnectButtonClicked() ) );
    connect( ui.m_KickButton,       SIGNAL( clicked() ),    this, SLOT( slotConnectButtonClicked() ) );
   
    ui.m_MenuButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeMedium ) );
    ui.m_MenuButton->setIcon( eMyIconMenu );
    ui.m_JoinButton->setIcon( eMyIconPersonAdd );
    ui.m_JoinButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_ConnectButton->setIcon( eMyIconConnect );
    ui.m_ConnectButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_KickButton->setIcon( eMyIconBoot );
    ui.m_KickButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    showConnectButton( false );
    showKickButton( false );
}

//============================================================================
GuiHostedListItem::~GuiHostedListItem()
{
    GuiHostedListSession * hostSession = (GuiHostedListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize GuiHostedListItem::calculateSizeHint( void )
{
    return QSize( ( int )( GuiParams::getGuiScale() * 200 ), ( int )( GuiParams::getButtonSize( eButtonSizeSmall ).height() + GuiParams::getButtonSize( eButtonSizeSmall ).height() ) + 16 );
}

//============================================================================
MyIcons& GuiHostedListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiHostedListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiHostedListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalGuiHostedListItemClicked( this );
}

//============================================================================
void GuiHostedListItem::setHostSession( GuiHostedListSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostedListSession * GuiHostedListItem::getHostSession( void )
{
    return (GuiHostedListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiHostedListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiHostedListItem::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void GuiHostedListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiHostedListItem::updateWidgetFromInfo( void )
{
    GuiHostedListSession* hostSession = getHostSession();
    if( nullptr == hostSession )
    {
        return;
    }

    GuiHosted* guiHosted = hostSession->getGuiHosted();
    if( nullptr == guiHosted )
    {
        return;
    }

    GuiUser* guiUser = hostSession->getGuiUser();
    //if( !guiUser )
    //{
    //    // if posible get user identity
    //    guiUser = m_MyApp.getUserMgr().getOrQueryUser( hostSession->getHostedId().getOnlineId() );
    //    if( guiUser )
    //    {
    //        hostSession->setGuiUser( guiUser );
    //    }
    //}

    if( guiUser )
    {
        updateIdentity( guiUser );
    }

    updateHosted( guiHosted );

    if( guiUser && m_MyApp.getEngine().getMyOnlineId() == hostSession->getHostedId().getOnlineId() )
    {
        ui.m_TitlePart2->setText( QObject::tr( "Hosted By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }

    if( !ui.m_IconButton->hasImage() && guiUser )
    {
        VxGUID thumbId = hostSession->getHostThumbId();
        if( !thumbId.isVxGUIDValid() )
        {
            thumbId = guiUser->getHostThumbId( hostSession->getHostType(), true );
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
    if( strDesc.empty() && guiUser )
    {
        strDesc = guiUser->getOnlineDescription();
    }

    if( !strDesc.empty() && guiUser )
    {
        ui.m_DescPart2->setText( strDesc.c_str() );
    }
}

//============================================================================
void GuiHostedListItem::setJoinedState( EJoinState joinState )
{
    // todo update join 
    switch( joinState )
    {
    case eJoinStateJoinGranted:
        showConnectButton( true );
        ui.m_ConnectButton->setIcon( eMyIconDisconnect );
        ui.m_ConnectLabel->setText( QObject::tr( "Disconnect" ) );
        ui.m_JoinButton->setIcon( eMyIconPersonAdd );
        ui.m_ConnectButton->setEnabled( true );
        showKickButton( true );
        break;
    case eJoinStateSending:
    case eJoinStateSendFail:
    case eJoinStateSendAcked:
    case eJoinStateJoinRequested:
        showConnectButton( true );
        ui.m_ConnectLabel->setText( QObject::tr( "Connect" ) );
        ui.m_ConnectButton->setIcon( eMyIconConnect );
        ui.m_JoinButton->setIcon( eMyIconPersonAdd );
        ui.m_ConnectButton->setEnabled( false );
        showKickButton( false );
        break;

    case eJoinStateJoinDenied:
        showConnectButton( false );
        ui.m_ConnectButton->setIcon( eMyIconIgnored );
        ui.m_JoinButton->setIcon( eMyIconIgnored );
        ui.m_ConnectButton->setEnabled( false );
        showKickButton( false );
        break;
    case eJoinStateNone:
    default:
        showConnectButton( false );
        ui.m_ConnectButton->setIcon( eMyIconPersonAdd );
        ui.m_JoinButton->setIcon( eMyIconPersonAdd );
        ui.m_ConnectButton->setEnabled( true );
        showKickButton( false );
        break;
    }
}

//============================================================================
void GuiHostedListItem::showConnectButton( bool isAccepted )
{
    ui.m_JoinButton->setVisible( !isAccepted );
    ui.m_JoinLabel->setVisible( !isAccepted );
    ui.m_ConnectButton->setVisible( isAccepted );
    ui.m_ConnectLabel->setVisible( isAccepted );
}

//============================================================================
void GuiHostedListItem::showKickButton( bool isVisible )
{
    ui.m_KickButton->setVisible( isVisible );
    ui.m_KickLabel->setVisible( isVisible );
}

//============================================================================
void GuiHostedListItem::slotJoinButtonClicked( void )
{
    emit signalJoinButtonClicked( this );
}

//============================================================================
void GuiHostedListItem::slotConnectButtonClicked( void )
{
    emit signalConnectButtonClicked( this );
}

//============================================================================
void GuiHostedListItem::slotKickButtonClicked( void )
{
    emit signalKickButtonClicked( this );
}
