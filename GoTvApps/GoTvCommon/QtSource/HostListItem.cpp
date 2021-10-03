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

#include <app_precompiled_hdr.h>
#include "HostListItem.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
HostListItem::HostListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    connect( ui.m_IconButton,       SIGNAL(clicked()),  this, SLOT(slotIconButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(pressed()),  this, SLOT(slotMenuButtonPressed()) );
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
HostListItem::~HostListItem()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& HostListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostListItemClicked( this );
}

//============================================================================
void HostListItem::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * HostListItem::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void HostListItem::slotIconButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostListItem::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void HostListItem::slotMenuButtonPressed( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void HostListItem::slotMenuButtonReleased( void )
{
}

//============================================================================
void HostListItem::updateWidgetFromInfo( void )
{
    GuiHostSession* hostSession = getHostSession();
    if( nullptr == hostSession )
    {
        return;
    }

    VxNetIdent& hostIdent = hostSession->getHostIdent();
    QString strName = hostIdent.getOnlineName();
    strName += " - ";
    ui.TitlePart1->setText( strName );

    if( m_MyApp.getEngine().getMyOnlineId() == hostIdent.getMyOnlineId() )
    {
        ui.TitlePart2->setText( QObject::tr( "Hosted By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }
    else
    {
        ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent.getMyFriendshipToHim() ) );
        ui.TitlePart2->setText( hostIdent.describeMyFriendshipToHim() );
    }
    
    /*
    QPalette pal = ui.m_IconButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent.getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_IconButton->setAutoFillBackground(true);
    ui.m_IconButton->setPalette(pal);
    ui.m_IconButton->update();
    */

    if( !ui.m_IconButton->hasImage() )
    {
        VxGUID thumbId = hostSession->getHostThumbId();
        if( !thumbId.isVxGUIDValid() )
        {
            thumbId = hostIdent.getHostThumbId( hostSession->getHostType(), true );
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
        strDesc = hostIdent.getOnlineDescription();
    }

    if( !strDesc.empty() )
    {
        ui.DescPart2->setText( strDesc.c_str() );
    }
}

//============================================================================
void HostListItem::setJoinedState( EJoinState joinState )
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
void HostListItem::showConnectButton( bool isAccepted )
{
    ui.m_JoinButton->setVisible( !isAccepted );
    ui.m_JoinLabel->setVisible( !isAccepted );
    ui.m_ConnectButton->setVisible( isAccepted );
    ui.m_ConnectLabel->setVisible( isAccepted );
}

//============================================================================
void HostListItem::slotJoinButtonPressed( void )
{
    emit signalJoinButtonClicked( this );
}

//============================================================================
void HostListItem::slotConnectButtonPressed( void )
{
    emit signalConnectButtonClicked( this );
}
