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
#include "CallListItem.h"
#include "GuiOfferSession.h"
#include "GuiParams.h"

//============================================================================
CallListItem::CallListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
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
CallListItem::~CallListItem()
{
    GuiOfferSession * hostSession = (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize CallListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& CallListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void CallListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void CallListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalCallListItemClicked( this );
}

//============================================================================
void CallListItem::setCallSession( GuiOfferSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiOfferSession * CallListItem::getCallSession( void )
{
    return (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void CallListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "CallListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void CallListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "CallListItem::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void CallListItem::slotMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void CallListItem::updateWidgetFromInfo( void )
{
    GuiOfferSession* hostSession = getCallSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "CallListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser * hostIdent = hostSession->getHisIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "CallListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    QString strName = hostIdent->getOnlineName().c_str();
    strName += " - ";
    QString strDesc = hostIdent->getOnlineDescription().c_str();

    // updateListEntryBackgroundColor( netIdent, item );

    ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent->getMyFriendshipToHim() ) );
    /*
    QPalette pal = ui.m_FriendshipButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent->getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_FriendshipButton->setAutoFillBackground(true);
    ui.m_FriendshipButton->setPalette(pal);
    ui.m_FriendshipButton->update();
    */
    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
}
