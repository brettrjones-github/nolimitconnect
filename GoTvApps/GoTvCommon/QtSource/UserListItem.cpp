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
#include "UserListItem.h"
#include "GuiUserSessionBase.h"
#include "GuiParams.h"

//============================================================================
UserListItem::UserListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    ui.m_AvatarButton->setIcon( eMyIconAvatarImage );
    ui.m_FriendshipButton->setIcon( eMyIconAnonymous );
    ui.m_MenuButton->setIcon( eMyIconMenu );

    connect( ui.m_AvatarButton,     SIGNAL(clicked()),  this, SLOT(slotAvatarButtonClicked()) );
    connect( ui.m_FriendshipButton, SIGNAL(clicked()),  this, SLOT(slotFriendshipButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(clicked()),  this, SLOT(slotMenuButtonClicked()) );
}

//============================================================================
UserListItem::~UserListItem()
{
    GuiUserSessionBase * hostSession = (GuiUserSessionBase *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize UserListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( 62 * GuiParams::getGuiScale() ) );
}

//============================================================================
MyIcons& UserListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void UserListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void UserListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalUserListItemClicked( this );
}

//============================================================================
void UserListItem::setUserSession( GuiUserSessionBase* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiUserSessionBase * UserListItem::getUserSession( void )
{
    return (GuiUserSessionBase *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void UserListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserListItem::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserListItem::slotMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void UserListItem::updateWidgetFromInfo( void )
{
    GuiUserSessionBase* hostSession = getUserSession();
    if( nullptr == hostSession )
    {
        return;
    }

    GuiUser *hostIdent = hostSession->getUserIdent();
    QString strName = hostIdent->getOnlineName();
    strName += " - ";
    QString strDesc = hostIdent->getOnlineDescription();

    // updateListEntryBackgroundColor( netIdent, item );

    ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent->getMyFriendshipToHim() ) );
    QPalette pal = ui.m_FriendshipButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent->getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_FriendshipButton->setAutoFillBackground(true);
    ui.m_FriendshipButton->setPalette(pal);
    ui.m_FriendshipButton->update();
    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
}
