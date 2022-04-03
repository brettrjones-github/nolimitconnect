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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppCommon.h"
#include "GuiUserJoinListItem.h"
#include "GuiUserJoinSession.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
GuiUserJoinListItem::GuiUserJoinListItem(QWidget *parent  )
: IdentWidget( parent )
, m_MyApp( GetAppInstance() )
, m_ConnectIdListMgr( m_MyApp.getConnectIdListMgr() )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_UserJoinMgr( m_MyApp.getUserJoinMgr() )
{
}

//============================================================================
GuiUserJoinListItem::~GuiUserJoinListItem()
{
    GuiUserJoinSession * hostSession = (GuiUserJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize GuiUserJoinListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& GuiUserJoinListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiUserJoinListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiUserJoinListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    // emit signalUserJoinListItemClicked( this );
}

//============================================================================
void GuiUserJoinListItem::setUserJoinSession( GuiUserJoinSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiUserJoinSession * GuiUserJoinListItem::getUserJoinSession( void )
{
    return (GuiUserJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiUserJoinListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiUserJoinListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void GuiUserJoinListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiUserJoinListItem::updateWidgetFromInfo( void )
{
    GuiUserJoinSession* hostSession = getUserJoinSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "GuiUserJoinListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser* guiUser = hostSession->getGuiUser();
    if( nullptr == guiUser )
    {
        LogMsg( LOG_DEBUG, "GuiUserJoinListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    updateIdentity( guiUser );
}

//============================================================================
void GuiUserJoinListItem::updateThumb( GuiThumb* thumb )
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

//============================================================================
void GuiUserJoinListItem::updateUser( GuiUser* guiUser )
{
    updateWidgetFromInfo();
}
