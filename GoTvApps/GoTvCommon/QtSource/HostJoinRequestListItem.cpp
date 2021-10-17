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
#include "HostJoinRequestListItem.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
HostJoinRequestListItem::HostJoinRequestListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    connect( ui.m_AvatarButton,       SIGNAL(clicked()),  this, SLOT(slotAvatarButtonClicked()) );
    connect( ui.m_FriendshipButton,   SIGNAL( clicked() ), this, SLOT( slotFriendshipButtonClicked() ) );
	connect( ui.m_MenuButton,         SIGNAL(pressed()),  this, SLOT(slotMenuButtonPressed()) );
    connect( ui.m_AcceptButton,		  SIGNAL(pressed()),	this, SLOT(slotAcceptButtonPressed()) );
    connect( ui.m_RejectButton,       SIGNAL( pressed() ), this, SLOT( slotRejectButtonPressed() ) );
   
    ui.m_AvatarButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeMedium ) );
    ui.m_AvatarButton->setIcon( eMyIconAvatarImage );
    ui.m_FriendshipButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeMedium ) );
    ui.m_FriendshipButton->setIcon( eMyIconAnonymous );
    ui.m_MenuButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeMedium ) );
    ui.m_MenuButton->setIcon( eMyIconMenu );

    ui.m_AcceptButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_AcceptButton->setIcon( eMyIconAcceptNormal );
    ui.m_RejectButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_RejectButton->setIcon( eMyIconCancelNormal );
}

//============================================================================
HostJoinRequestListItem::~HostJoinRequestListItem()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& HostJoinRequestListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostJoinRequestListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostJoinRequestListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostJoinRequestListItemClicked( this );
}

//============================================================================
void HostJoinRequestListItem::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * HostJoinRequestListItem::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void HostJoinRequestListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListItem::slotAvatarButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListItem::slotAvatarButtonClicked" );
    emit signalFriendshipButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::slotMenuButtonPressed( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::slotMenuButtonReleased( void )
{
}

//============================================================================
void HostJoinRequestListItem::updateWidgetFromInfo( void )
{
    GuiHostSession* hostSession = getHostSession();
    if( !hostSession || !hostSession->getUserIdent() )
    {
        LogMsg( LOG_ERROR, "HostJoinRequestListItem::updateWidgetFromInfo null ident" );
        return;
    }

    GuiUser* hostIdent = hostSession->getUserIdent();
    QString strName = hostIdent->getOnlineName().c_str();
    strName += " - ";
    ui.TitlePart1->setText( strName );

    if( m_MyApp.getEngine().getMyOnlineId() == hostIdent->getMyOnlineId() )
    {
        ui.TitlePart2->setText( QObject::tr( "Hosted By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }
    else
    {
        ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent->getMyFriendshipToHim() ) );
        ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim() );
    }
    
    /*
    QPalette pal = ui.m_AvatarButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent.getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_AvatarButton->setAutoFillBackground(true);
    ui.m_AvatarButton->setPalette(pal);
    ui.m_AvatarButton->update();
    */

    if( !ui.m_AvatarButton->hasImage() )
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
                ui.m_AvatarButton->setIconOverrideImage( thumbImage );
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
        ui.DescPart2->setText( strDesc.c_str() );
    }
}

//============================================================================
void HostJoinRequestListItem::setJoinedState( EJoinState joinState )
{
    // todo update join 
    switch( joinState )
    {
    case eJoinStateJoinAccepted:
        showAcceptButton( false );
        showRejectButton( true );
        break;
    case eJoinStateSending:
    case eJoinStateSendFail:
    case eJoinStateSendAcked:
    case eJoinStateJoinRequested:
        showAcceptButton( true );
        showRejectButton( false );
        break;
    case eJoinStateJoinDenied:
    case eJoinStateNone:
    default:
        break;
    }
}

//============================================================================
void HostJoinRequestListItem::showAcceptButton( bool makeVisible )
{
    ui.m_AcceptButton->setVisible( makeVisible );
    ui.m_AcceptLabel->setVisible( makeVisible );

}

//============================================================================
void HostJoinRequestListItem::showRejectButton( bool makeVisible )
{
    ui.m_RejectButton->setVisible( makeVisible );
    ui.m_RejectLabel->setVisible( makeVisible );
}

//============================================================================
void HostJoinRequestListItem::slotAcceptButtonPressed( void )
{
    emit signalAcceptButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::slotRejectButtonPressed( void )
{
    emit signalRejectButtonClicked( this );
}
