//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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


#include "BottomBarWidget.h"
#include "MyIcons.h"
#include "AppCommon.h"

//============================================================================
BottomBarWidget::BottomBarWidget( QWidget * parent )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
	setFixedHeight( GuiParams::getButtonSize(eButtonSizeSmall).height() + 6 );
	ui.m_ArrowLeftButton->setFixedSize( eButtonSizeSmall );
	ui.m_30SecBackButton->setFixedSize( eButtonSizeSmall );
	ui.m_MediaPlayButton->setFixedSize( eButtonSizeSmall );
	ui.m_MediaTrashButton->setFixedSize( eButtonSizeSmall );
	ui.m_MediaFileShareButton->setFixedSize( eButtonSizeSmall );
	ui.m_MediaLibraryButton->setFixedSize( eButtonSizeSmall );
	ui.m_30SecForwardButton->setFixedSize( eButtonSizeSmall );
	ui.m_ArrowRightButton->setFixedSize( eButtonSizeSmall );
	ui.m_MediaRepeatButton->setFixedSize( eButtonSizeSmall );
	ui.m_MenuBottomButton->setFixedSize( eButtonSizeSmall );
	ui.m_ExpandButton->setFixedSize( eButtonSizeSmall );

	setArrowLeftButtonIcon();
	set30SecBackwardButtonIcon();
	setMediaPlayButtonIcon();
	setMediaTrashButtonIcon();
	setMediaFileShareButtonIcon();
	setMediaLibraryButtonIcon();
	set30SecForwardButtonIcon();
	setArrowRightButtonIcon();
	setMediaRepeatButtonIcon();
	setMenuBottomButtonIcon();
	setExpandWindowButtonIcon();

	setPlayProgressBarVisibility( false );
	setArrowLeftVisibility( false );
	set30SecBackwardVisibility( false );
	setMediaPlayVisibility( false );
	setMediaTrashVisibility( false );
	setMediaFileShareVisibility( false );
	setMediaLibraryVisibility( false );
	set30SecForwardVisibility( false );
	setArrowRightVisibility( false );
	setMediaRepeatVisibility( false );
	setMenuBottomVisibility( false );
	setExpandWindowVisibility( true );

	connect( ui.m_ArrowLeftButton,			SIGNAL(clicked()),		this, SLOT(slotArrowLeftButtonClicked()) );
	connect( ui.m_30SecBackButton,			SIGNAL( clicked() ),	this, SLOT( slot30SecBackwardButtonClicked() ) );
	connect( ui.m_MediaPlayButton,			SIGNAL( clicked() ),	this, SLOT( slotMediaPlayButtonClicked() ) );
	connect( ui.m_MediaTrashButton,			SIGNAL(clicked()),		this, SLOT(slotMediaTrashButtonClicked()) );
	connect( ui.m_MediaFileShareButton,		SIGNAL(clicked()),		this, SLOT(slotMediaFileShareClicked()) );
	connect( ui.m_MediaLibraryButton,		SIGNAL(clicked()),		this, SLOT(slotMediaLibraryButtonClicked()) );
	connect( ui.m_30SecForwardButton,		SIGNAL(clicked()),		this, SLOT(slot30SecForwardButtonClicked()) );
	connect( ui.m_ArrowRightButton,			SIGNAL( clicked() ), this, SLOT( slotArrowRightButtonClicked() ) );
	connect( ui.m_MediaRepeatButton,		SIGNAL( clicked() ), this, SLOT( slotMediaRepeatButtonClicked() ) );
	connect( ui.m_MenuBottomButton,			SIGNAL( clicked() ), this, SLOT( slotMenuBottomButtonClicked() ) );
	connect( ui.m_ExpandButton,				SIGNAL( clicked() ), this, SLOT( slotExpandWindowButtonClicked() ) );
}

//============================================================================
BottomBarWidget::~BottomBarWidget()
{
}
//============================================================================
//=== bottom bar functions ===// 
//============================================================================
void BottomBarWidget::setPlayProgressPlayedTime( int timeSec )
{
	ui.m_PlayProgressBar->setValue( timeSec );
}

//============================================================================
void BottomBarWidget::setPlayProgressTotalTime( int timeSec )
{
	ui.m_PlayProgressBar->setRange( 0, timeSec );
}

//============================================================================
//=== bottom bar button visibility ===// 
//============================================================================
void BottomBarWidget::setPlayProgressBarVisibility( bool visible ) { ui.m_PlayProgressFrame->setVisible( visible ); }
void BottomBarWidget::setArrowLeftVisibility( bool visible ){ ui.m_ArrowLeftButton->setVisible( visible ); }
void BottomBarWidget::set30SecBackwardVisibility( bool visible ){ ui.m_30SecBackButton->setVisible( visible ); }
void BottomBarWidget::setMediaPlayVisibility( bool visible ){ ui.m_MediaPlayButton->setVisible( visible ); }
void BottomBarWidget::setMediaTrashVisibility( bool visible ){ ui.m_MediaTrashButton->setVisible( visible ); }
void BottomBarWidget::setMediaFileShareVisibility( bool visible ){ ui.m_MediaFileShareButton->setVisible( visible ); }
void BottomBarWidget::setMediaLibraryVisibility( bool visible ){ ui.m_MediaLibraryButton->setVisible( visible ); }
void BottomBarWidget::set30SecForwardVisibility( bool visible ){ ui.m_30SecForwardButton->setVisible( visible ); }
void BottomBarWidget::setArrowRightVisibility( bool visible ){ ui.m_ArrowRightButton->setVisible( visible ); }
void BottomBarWidget::setMediaRepeatVisibility( bool visible ){ ui.m_MediaRepeatButton->setVisible( visible ); }
void BottomBarWidget::setMenuBottomVisibility( bool visible ){ ui.m_MenuBottomButton->setVisible( visible ); }
void BottomBarWidget::setExpandWindowVisibility( bool visible ){ ui.m_ExpandButton->setVisible( visible ); }

//============================================================================
MyIcons&  BottomBarWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
VxMenuButton * BottomBarWidget::getMenuButton( void )
{
    return ui.m_MenuBottomButton;
}

//============================================================================
void BottomBarWidget::setArrowLeftButtonIcon( EMyIcons myIcon )
{
	ui.m_ArrowLeftButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::set30SecBackwardButtonIcon( EMyIcons myIcon )
{
	ui.m_30SecBackButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setMediaPlayButtonIcon( EMyIcons myIcon )
{
	ui.m_MediaPlayButton->setIcon( myIcon );
}
//============================================================================
void BottomBarWidget::setMediaTrashButtonIcon( EMyIcons myIcon )
{
	ui.m_MediaTrashButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setMediaFileShareButtonIcon( EMyIcons myIcon )
{
	ui.m_MediaFileShareButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setMediaLibraryButtonIcon( EMyIcons myIcon )
{
	ui.m_MediaLibraryButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::set30SecForwardButtonIcon( EMyIcons myIcon )
{
	ui.m_30SecForwardButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setArrowRightButtonIcon( EMyIcons myIcon )
{
	ui.m_ArrowRightButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setMediaRepeatButtonIcon( EMyIcons myIcon )
{
	ui.m_MediaRepeatButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setMenuBottomButtonIcon( EMyIcons myIcon )
{
	ui.m_MenuBottomButton->setIcon( myIcon );
}

//============================================================================
void BottomBarWidget::setExpandWindowButtonIcon( EMyIcons myIcon )
{
	ui.m_ExpandButton->setIcon( myIcon );
}

//=== bottom bar button color ===// 
//============================================================================
void BottomBarWidget::setPlayProgressBarColor( QColor iconColor )
{
	//ui.m_ArrowLeftButton->setIconColor( iconColor );
}

//============================================================================
void BottomBarWidget::setArrowLeftButtonColor( QColor iconColor )
{
	ui.m_ArrowLeftButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::set30SecBackwardButtonColor( QColor iconColor )
{
	ui.m_30SecBackButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setMediaPlayButtonColor( QColor iconColor )
{
	ui.m_MediaPlayButton->setIconOverrideColor( iconColor );
}
//============================================================================
void BottomBarWidget::setMediaTrashButtonColor( QColor iconColor )
{
	ui.m_MediaTrashButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setMediaFileShareButtonColor( QColor iconColor )
{
	ui.m_MediaFileShareButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setMediaLibraryButtonColor( QColor iconColor )
{
	ui.m_MediaLibraryButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::set30SecForwardButtonColor( QColor iconColor )
{
	ui.m_30SecForwardButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setArrowRightButtonColor( QColor iconColor )
{
	ui.m_ArrowRightButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setMediaRepeatButtonColor( QColor iconColor )
{
	ui.m_MediaRepeatButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setMenuBottomButtonColor( QColor iconColor )
{
	ui.m_MenuBottomButton->setIconOverrideColor( iconColor );
}

//============================================================================
void BottomBarWidget::setExpandWindowButtonColor( QColor iconColor )
{
	ui.m_ExpandButton->setIconOverrideColor( iconColor );
}

//=== bottom bar button clicked ===// 
//============================================================================
void BottomBarWidget::slotArrowLeftButtonClicked( void )
{
	emit signalArrowLeftButtonClicked();
}

//============================================================================
void BottomBarWidget::slot30SecBackwardButtonClicked( void )
{
	emit signal30SecBackwardButtonClicked();
}

//============================================================================
void BottomBarWidget::slotMediaPlayButtonClicked( void )
{
	emit signalMediaPlayButtonClicked();
}

//============================================================================
void BottomBarWidget::slotMediaTrashButtonClicked( void )
{
    emit signalMediaTrashButtonClicked();
}

//============================================================================
void BottomBarWidget::slot30SecForwardButtonClicked( void )
{
    emit signal30SecForwardButtonClicked();
}

//============================================================================
void BottomBarWidget::slotMediaFileShareClicked( void )
{
	emit signalMediaFileShareClicked();
}

//============================================================================
void BottomBarWidget::slotMediaLibraryButtonClicked( void )
{
	emit signalMediaLibraryButtonClicked();
}

//============================================================================
void BottomBarWidget::slotArrowRightButtonClicked( void )
{
	emit signalArrowRightButtonClicked();
}

//============================================================================
void BottomBarWidget::slotMediaRepeatButtonClicked( void )
{
	emit signalMediaRepeatButtonClicked();
}

//============================================================================
void BottomBarWidget::slotMenuBottomButtonClicked( void )
{
	emit signalMenuBottomButtonClicked();
}

//============================================================================
void BottomBarWidget::slotExpandWindowButtonClicked( void )
{
	emit signalExpandWindowButtonClicked();
}
