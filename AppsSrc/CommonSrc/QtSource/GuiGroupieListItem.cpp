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

#include "GuiGroupieListItem.h"
#include "GuiGroupieListSession.h"
#include "GuiParams.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>

//============================================================================
GuiGroupieListItem::GuiGroupieListItem(QWidget *parent  )
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
GuiGroupieListItem::~GuiGroupieListItem()
{
    GuiGroupieListSession * groupieSession = (GuiGroupieListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( groupieSession && !groupieSession->parent() )
    {
        delete groupieSession;
    }
}

//============================================================================
MyIcons& GuiGroupieListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiGroupieListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiGroupieListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalGuiGroupieListItemClicked( this );
}

//============================================================================
void GuiGroupieListItem::setHostSession( GuiGroupieListSession* groupieSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)groupieSession) );
}

//============================================================================
GuiGroupieListSession * GuiGroupieListItem::getHostSession( void )
{
    return (GuiGroupieListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiGroupieListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiGroupieListItem::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void GuiGroupieListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiGroupieListItem::updateWidgetFromInfo( void )
{
    GuiGroupieListSession* groupieSession = getHostSession();
    if( nullptr == groupieSession )
    {
        return;
    }

    GuiUser* hostIdent = groupieSession->getUserIdent();
    if( hostIdent )
    {
        updateIdentity( hostIdent );
    }

    if( m_MyApp.getEngine().getMyOnlineId() == hostIdent->getMyOnlineId() )
    {
        ui.m_TitlePart2->setText( QObject::tr( "Groupie By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }

    if( !ui.m_IconButton->hasImage() )
    {
        VxGUID thumbId = groupieSession->getHostThumbId();
        if( !thumbId.isVxGUIDValid() )
        {
            thumbId = hostIdent->getHostThumbId( groupieSession->getHostType(), true );
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
    std::string strDesc = groupieSession->getGroupieDescription();
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
void GuiGroupieListItem::setJoinedState( EJoinState joinState )
{
    // todo update join 
    switch( joinState )
    {
    case eJoinStateJoinAccepted:
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
void GuiGroupieListItem::showConnectButton( bool isAccepted )
{
    ui.m_JoinButton->setVisible( !isAccepted );
    ui.m_JoinLabel->setVisible( !isAccepted );
    ui.m_ConnectButton->setVisible( isAccepted );
    ui.m_ConnectLabel->setVisible( isAccepted );
}

//============================================================================
void GuiGroupieListItem::slotJoinButtonPressed( void )
{
    emit signalJoinButtonClicked( this );
}

//============================================================================
void GuiGroupieListItem::slotConnectButtonPressed( void )
{
    emit signalConnectButtonClicked( this );
}
