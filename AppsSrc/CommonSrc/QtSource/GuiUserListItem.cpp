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
#include "GuiUserListItem.h"
#include "GuiUserSessionBase.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
GuiUserListItem::GuiUserListItem(QWidget *parent  )
: IdentWidget( parent )
, m_MyApp( GetAppInstance() )
{
    setIdentWidgetSize( eButtonSizeSmall );
}

//============================================================================
GuiUserListItem::~GuiUserListItem()
{
    GuiUserSessionBase * hostSession = getUserSession();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
QSize GuiUserListItem::calculateSizeHint( void )
{
    return QSize( (int)( GuiParams::getGuiScale() * 200 ), (int)( GuiParams::getButtonSize().height() + 8 ) );
}

//============================================================================
MyIcons& GuiUserListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiUserListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiUserListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalGuiUserListItemClicked( this );
}

//============================================================================
void GuiUserListItem::setUserSession( GuiUserSessionBase* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((qulonglong)hostSession) );
}

//============================================================================
GuiUserSessionBase * GuiUserListItem::getUserSession( void )
{
    return (GuiUserSessionBase *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiUserListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiUserListItem::slotIconButtonClicked" );
	emit signalAvatarButtonClicked( this );
}

//============================================================================
void GuiUserListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiUserListItem::updateWidgetFromInfo( void )
{
    GuiUserSessionBase* hostSession = getUserSession();
    if( nullptr == hostSession )
    {
        LogMsg( LOG_DEBUG, "GuiUserListItem::updateWidgetFromInfo null user session" );
        return;
    }

    GuiUser * hostIdent = hostSession->getUserIdent();
    if( nullptr == hostIdent )
    {
        LogMsg( LOG_DEBUG, "GuiUserListItem::updateWidgetFromInfo null gui user" );
        return;
    }

    updateIdentity( hostIdent );
}

//============================================================================
void GuiUserListItem::updateThumb( GuiThumb* thumb )
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