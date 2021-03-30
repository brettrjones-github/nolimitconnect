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
#include "UserListEntryWidget.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
UserListEntryWidget::UserListEntryWidget(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    connect( ui.m_AvatarButton,     SIGNAL(clicked()),  this, SLOT(slotAvatarButtonClicked()) );
    connect( ui.m_FriendshipButton, SIGNAL(clicked()),  this, SLOT(slotFriendshipButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(pressed()),  this, SLOT(slotMenuButtonPressed()) );
}

//============================================================================
UserListEntryWidget::~UserListEntryWidget()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& UserListEntryWidget::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void UserListEntryWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void UserListEntryWidget::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalUserListItemClicked( this );
}

//============================================================================
void UserListEntryWidget::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * UserListEntryWidget::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void UserListEntryWidget::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserListEntryWidget::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserListEntryWidget::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserListEntryWidget::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserListEntryWidget::slotMenuButtonPressed( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void UserListEntryWidget::slotMenuButtonReleased( void )
{
}

//============================================================================
void UserListEntryWidget::updateWidgetFromInfo( void )
{
    GuiHostSession* hostSession = getHostSession();
    if( nullptr == hostSession )
    {
        return;
    }

    VxNetIdent& hostIdent = hostSession->getHostIdent();
    QString strName = hostIdent.getOnlineName();
    strName += " - ";
    QString strDesc = hostIdent.getOnlineDescription();

    // updateListEntryBackgroundColor( netIdent, item );

    ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent.getMyFriendshipToHim() ) );
    QPalette pal = ui.m_FriendshipButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent.getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_FriendshipButton->setAutoFillBackground(true);
    ui.m_FriendshipButton->setPalette(pal);
    ui.m_FriendshipButton->update();
    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent.describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
}
