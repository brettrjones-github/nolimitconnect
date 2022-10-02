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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"
#include "ShareOfferListItem.h"
#include "GuiOfferSession.h"
#include "GuiParams.h"

//============================================================================
ShareOfferListItem::ShareOfferListItem(QWidget*parent  )
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
ShareOfferListItem::~ShareOfferListItem()
{
    GuiOfferSession * hostSession = (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize ShareOfferListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& ShareOfferListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void ShareOfferListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void ShareOfferListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalShareOfferListItemClicked( this );
}

//============================================================================
void ShareOfferListItem::setOfferSession( GuiOfferSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiOfferSession * ShareOfferListItem::getOfferSession( void )
{
    return (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void ShareOfferListItem::slotAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "ShareOfferListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void ShareOfferListItem::slotFriendshipButtonClicked()
{
    LogMsg( LOG_DEBUG, "ShareOfferListItem::slotFriendshipButtonClicked" );
    emit signalAvatarButtonClicked( this );
}

//============================================================================
void ShareOfferListItem::slotMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void ShareOfferListItem::updateWidgetFromInfo( void )
{
    GuiOfferSession* hostSession = getOfferSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "ShareOfferListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser* hostIdent = hostSession->getHisIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "ShareOfferListItem::updateWidgetFromInfo null gui user" );
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
    ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim( false ) );
    ui.DescPart2->setText( strDesc );
}
