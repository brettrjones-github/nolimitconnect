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
#include "HostListItem.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
HostListItem::HostListItem(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    connect( ui.m_IconButton,       SIGNAL(clicked()),  this, SLOT(slotIconButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(pressed()),  this, SLOT(slotMenuButtonPressed()) );
    connect( ui.m_JoinButton,		SIGNAL(pressed()),	this, SLOT(slotJoinButtonPressed()) );
}

//============================================================================
HostListItem::~HostListItem()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& HostListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostListItemClicked( this );
}

//============================================================================
void HostListItem::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * HostListItem::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void HostListItem::slotIconButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostListItem::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void HostListItem::slotMenuButtonPressed( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void HostListItem::slotMenuButtonReleased( void )
{
}

//============================================================================
void HostListItem::slotJoinButtonPressed( void )
{
    emit signalJoinButtonClicked( this );
}

//============================================================================
void HostListItem::updateWidgetFromInfo( void )
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

    ui.m_IconButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent.getMyFriendshipToHim() ) );
    QPalette pal = ui.m_IconButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent.getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_IconButton->setAutoFillBackground(true);
    ui.m_IconButton->setPalette(pal);
    ui.m_IconButton->update();
    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent.describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
    ui.m_MenuButton->setIcon( eMyIconMenu );
}
