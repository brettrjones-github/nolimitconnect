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
#include "UserJoinListItem.h"
#include "GuiUserJoinSession.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
UserJoinListItem::UserJoinListItem(QWidget *parent  )
: IdentWidget( parent )
, m_MyApp( GetAppInstance() )
, m_ConnectMgr( m_MyApp.getConnectMgr() )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_UserJoinMgr( m_MyApp.getUserJoinMgr() )
{
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
    // emit signalUserJoinListItemClicked( this );
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
void UserJoinListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "UserJoinListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void UserJoinListItem::onIdentMenuButtonClicked( void )
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

    updateIdentity( hostIdent );
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
