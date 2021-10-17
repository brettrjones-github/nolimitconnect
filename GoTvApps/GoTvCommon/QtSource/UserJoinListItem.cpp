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
#include "UserJoinListItem.h"
#include "GuiUserJoinSession.h"
#include "GuiParams.h"

//============================================================================
UserJoinListItem::UserJoinListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
, m_ConnectMgr( m_MyApp.getConnectMgr() )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_UserJoinMgr( m_MyApp.getUserJoinMgr() )
{
	ui.setupUi( this );
    ui.m_AvatarButton->setFixedSize( GuiParams::getButtonSize() );
    ui.m_AvatarButton->setIcon( eMyIconAvatarImage );
    ui.m_FriendshipButton->setFixedSize( GuiParams::getButtonSize() );
    ui.m_FriendshipButton->setIcon( eMyIconAnonymous );
    ui.m_MenuButton->setFixedSize( GuiParams::getButtonSize() );
    ui.m_MenuButton->setIcon( eMyIconMenu );

    connect( ui.m_AvatarButton,     SIGNAL(clicked()),  this, SLOT(slotAvatarButtonClicked()) );
    connect( ui.m_FriendshipButton, SIGNAL(clicked()),  this, SLOT(slotFriendshipButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(clicked()),  this, SLOT(slotMenuButtonClicked()) );
}

//============================================================================
UserJoinListItem::~UserJoinListItem()
{
    GuiUserJoinSession * hostSession = (GuiUserJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize UserJoinListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& UserJoinListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void UserJoinListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void UserJoinListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalUserJoinListItemClicked( this );
}

//============================================================================
void UserJoinListItem::setUserJoinSession( GuiUserJoinSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiUserJoinSession * UserJoinListItem::getUserJoinSession( void )
{
    return (GuiUserJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void UserJoinListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserJoinListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserJoinListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserJoinListItem::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserJoinListItem::slotMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void UserJoinListItem::updateWidgetFromInfo( void )
{
    GuiUserJoinSession* hostSession = getUserJoinSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "UserJoinListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser * hostIdent = hostSession->getUserIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "UserJoinListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    QString strName = hostIdent->getOnlineName().c_str();
    strName += " - ";
    QString strDesc = hostIdent->getOnlineDescription().c_str();

    if( hostIdent->isMyself() )
    {
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator ); // eMyIconAdministrator );
        ui.m_AvatarButton->setNotifyOnlineEnabled( true );
    }
    else
    {
        ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent->getMyFriendshipToHim() ) );
        ui.m_AvatarButton->setNotifyOnlineEnabled( hostIdent->isOnline() );
    }

    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
}

//============================================================================
void UserJoinListItem::updateThumb( GuiThumb* thumb )
{
    if( thumb )
    {
        QImage thumbImage;
        if( thumb->createImage( thumbImage ) && !thumbImage.isNull() )
        {
            ui.m_AvatarButton->setIconOverrideImage( thumbImage );
            update();
        }
    }
}
