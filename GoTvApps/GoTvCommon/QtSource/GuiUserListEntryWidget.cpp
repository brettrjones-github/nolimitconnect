//============================================================================
// Copyright (C) 2009 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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
#include "GuiUserListEntryWidget.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
GuiUserListEntryWidget::GuiUserListEntryWidget(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    connect( ui.m_IconButton,       SIGNAL(clicked()),  this, SLOT(slotIconButtonClicked()) );
	connect( ui.m_MenuButton,       SIGNAL(pressed()),  this, SLOT(slotMenuButtonPressed()) );
    connect( ui.m_JoinButton,		SIGNAL(pressed()),	this, SLOT(slotJoinButtonPressed()) );
}

//============================================================================
GuiUserListEntryWidget::~GuiUserListEntryWidget()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& GuiUserListEntryWidget::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void GuiUserListEntryWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void GuiUserListEntryWidget::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostListItemClicked( this );
}

//============================================================================
void GuiUserListEntryWidget::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * GuiUserListEntryWidget::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void GuiUserListEntryWidget::slotIconButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiUserListEntryWidget::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void GuiUserListEntryWidget::slotMenuButtonPressed( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void GuiUserListEntryWidget::slotMenuButtonReleased( void )
{
}

//============================================================================
void GuiUserListEntryWidget::slotJoinButtonPressed( void )
{
    emit signalJoinButtonClicked( this );
}

//============================================================================
void GuiUserListEntryWidget::updateWidgetFromInfo( void )
{
    GuiHostSession* hostSession = getHostSession();
    if( !hostSession || !hostSession->getUserIdent() )
    {
        LogMsg( LOG_ERROR, "GuiUserListEntryWidget::updateWidgetFromInfo null ident" );
        return;
    }

    GuiUser* hostIdent = hostSession->getUserIdent();
    QString strName = hostIdent->getOnlineName();
    strName += " - ";
    QString strDesc = hostIdent->getOnlineDescription();

    // updateListEntryBackgroundColor( netIdent, item );

    ui.m_IconButton->setIcon( getMyIcons().getFriendshipIcon( hostIdent->getMyFriendshipToHim() ) );
    QPalette pal = ui.m_IconButton->palette();
    pal.setColor(QPalette::Button, QColor( hostIdent->getHasTextOffers() ? Qt::yellow : Qt::white ));
    ui.m_IconButton->setAutoFillBackground(true);
    ui.m_IconButton->setPalette(pal);
    ui.m_IconButton->update();
    ui.TitlePart1->setText( strName );
    ui.TitlePart2->setText( hostIdent->describeMyFriendshipToHim() );
    ui.DescPart2->setText( strDesc );
}
