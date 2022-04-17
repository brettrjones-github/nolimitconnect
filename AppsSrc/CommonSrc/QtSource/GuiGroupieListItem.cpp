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
#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiGroupieListItem.h"
#include "GuiGroupieListSession.h"
#include "GuiParams.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>

//============================================================================
GuiGroupieListItem::GuiGroupieListItem( QWidget *parent )
: IdentWidget( parent )
{
}

//============================================================================
GuiGroupieListItem::~GuiGroupieListItem()
{
    GuiGroupieListSession * groupieSession = (GuiGroupieListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( groupieSession && !groupieSession->parent() )
    {
        delete groupieSession;
    }
}

//============================================================================
QSize GuiGroupieListItem::calculateSizeHint( void )
{
    return QSize( ( int )( GuiParams::getGuiScale() * 200 ), ( int )( GuiParams::getButtonSize( eButtonSizeMedium ).height() + 8 ) );
}

//============================================================================
MyIcons& GuiGroupieListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiGroupieListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiGroupieListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalGuiGroupieListItemClicked( this );
}

//============================================================================
void GuiGroupieListItem::setHostSession( GuiGroupieListSession* groupieSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)groupieSession) );
}

//============================================================================
GuiGroupieListSession * GuiGroupieListItem::getHostSession( void )
{
    return (GuiGroupieListSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiGroupieListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiGroupieListItem::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void GuiGroupieListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiGroupieListItem::updateWidgetFromInfo( void )
{
    GuiGroupieListSession* groupieSession = getHostSession();
    if( nullptr == groupieSession )
    {
        return;
    }

    updateGroupie( groupieSession->getGroupie() );
}

//============================================================================
void GuiGroupieListItem::updateUser( GuiUser* guiUser )
{
    updateWidgetFromInfo();
}
