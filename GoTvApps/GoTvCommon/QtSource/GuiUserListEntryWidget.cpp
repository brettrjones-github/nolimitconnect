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

#include "AppCommon.h"
#include "GuiUserListEntryWidget.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
GuiUserListEntryWidget::GuiUserListEntryWidget(QWidget *parent  )
: IdentLogicInterface( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    setupIdentLogic();
    getIdentAvatarButton()->setFixedSize( eButtonSizeLarge );
    getIdentFriendshipButton()->setFixedSize( eButtonSizeLarge );
    getIdentMenuButton()->setFixedSize( eButtonSizeLarge );
    connect( ui.m_JoinButton, SIGNAL( clicked() ), this, SLOT( slotJoinButtonPressed() ) );
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
void GuiUserListEntryWidget::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "GuiUserListEntryWidget::slotIconButtonClicked" );
	emit signalIconButtonClicked( this );
}

//============================================================================
void GuiUserListEntryWidget::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
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
    if( hostIdent )
    {
        updateIdentity( hostIdent );
    }
}
