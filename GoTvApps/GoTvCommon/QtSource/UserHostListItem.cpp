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
#include "UserHostListItem.h"
#include "GuiUserHostSession.h"
#include "GuiParams.h"

//============================================================================
UserHostListItem::UserHostListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
, m_ConnectMgr( m_MyApp.getConnectMgr() )
, m_OfferMgr( m_MyApp.getOfferMgr() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_UserHostMgr( m_MyApp.getUserHostMgr() )
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
UserHostListItem::~UserHostListItem()
{
    GuiUserHostSession * hostSession = (GuiUserHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize UserHostListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& UserHostListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void UserHostListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void UserHostListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalUserHostListItemClicked( this );
}

//============================================================================
void UserHostListItem::setUserHostSession( GuiUserHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiUserHostSession * UserHostListItem::getUserHostSession( void )
{
    return (GuiUserHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void UserHostListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserHostListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserHostListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserHostListItem::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserHostListItem::slotMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void UserHostListItem::updateWidgetFromInfo( void )
{
    GuiUserHostSession* hostSession = getUserHostSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "UserHostListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser * hostIdent = hostSession->getUserIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "UserHostListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    QString strName = hostIdent->getOnlineName();
    strName += " - ";
    QString strDesc = hostIdent->getOnlineDescription();

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
void UserHostListItem::updateThumb( GuiThumb* thumb )
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
