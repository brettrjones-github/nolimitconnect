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
#include "HostListEntryWidget.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

//============================================================================
HostListEntryWidget::HostListEntryWidget(QWidget *parent  )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
	connect( ui.m_IconButton, SIGNAL(pressed()), this, SLOT(iconButtonPressed()) );
	connect( ui.m_IconButton, SIGNAL(released()), this, SLOT(iconButtonReleased()) );
	connect( ui.m_MenuButton, SIGNAL(pressed()), this, SLOT(slotMenuButtonPressed()) );
	connect( ui.m_MenuButton, SIGNAL(released()), this, SLOT(slotMenuButtonReleased()) );
    /*
    connect( ui.m_FileIconButton,		SIGNAL(clicked()),						this, SLOT(slotFileIconButtonClicked()) );
    connect( ui.m_FileCancelButton,		SIGNAL(clicked()),						this, SLOT(slotCancelButtonClicked()) );
    connect( ui.m_FileActionBar,		SIGNAL(signalPlayButtonClicked()),		this, SLOT(slotPlayButtonClicked()) );
    connect( ui.m_FileActionBar,		SIGNAL(signalLibraryButtonClicked()),	this, SLOT(slotLibraryButtonClicked()) );
    connect( ui.m_FileActionBar,		SIGNAL(signalFileShareButtonClicked()), this, SLOT(slotFileShareButtonClicked()) );
    connect( ui.m_FileActionBar,		SIGNAL(signalShredButtonClicked()),		this, SLOT(slotShredButtonClicked()) );
    */
}

//============================================================================
HostListEntryWidget::~HostListEntryWidget()
{
    GuiHostSession * hostSession = (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& HostListEntryWidget::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostListEntryWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostListEntryWidget::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostListItemClicked( this );
}

//============================================================================
void HostListEntryWidget::setHostSession( GuiHostSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostSession * HostListEntryWidget::getHostSession( void )
{
    return (GuiHostSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void HostListEntryWidget::iconButtonPressed()
{
	//SubWidget->setDown(true);
	emit listButtonClicked( this );
}

//============================================================================
void HostListEntryWidget::iconButtonReleased()
{
	//SubWidget->setDown(false);
}

//============================================================================
void HostListEntryWidget::slotMenuButtonPressed()
{
	//SubWidget->setDown(true);
	emit signalMenuButtonClicked( this );
}

//============================================================================
void HostListEntryWidget::slotMenuButtonReleased()
{
	//SubWidget->setDown(false);
}

//============================================================================
void HostListEntryWidget::listButtonPressed()
{
	emit listButtonClicked( this );
}

//============================================================================
void HostListEntryWidget::listButtonReleased()
{

}

//============================================================================
void HostListEntryWidget::updateWidgetFromInfo( void )
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

}
