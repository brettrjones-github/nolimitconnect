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

#include "AppCommon.h"
#include "PersonOfferListItem.h"
#include "GuiOfferSession.h"
#include "GuiParams.h"

//============================================================================
PersonOfferListItem::PersonOfferListItem(QWidget *parent  )
: IdentLogicInterface( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    setupIdentLogic();
    ui.m_AvatarButton->setFixedSize( eButtonSizeLarge );
    ui.m_FriendshipButton->setFixedSize( eButtonSizeLarge );
    ui.m_MenuButton->setFixedSize( eButtonSizeLarge );
}

//============================================================================
PersonOfferListItem::~PersonOfferListItem()
{
    GuiOfferSession * hostSession = (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize PersonOfferListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& PersonOfferListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void PersonOfferListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void PersonOfferListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalPersonOfferListItemClicked( this );
}

//============================================================================
void PersonOfferListItem::setOfferSession( GuiOfferSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiOfferSession * PersonOfferListItem::getOfferSession( void )
{
    return (GuiOfferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void PersonOfferListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "PersonOfferListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void PersonOfferListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void PersonOfferListItem::updateWidgetFromInfo( void )
{
    GuiOfferSession* hostSession = getOfferSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "PersonOfferListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser * hostIdent = hostSession->getHisIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "PersonOfferListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    updateIdentity( hostIdent );
}
