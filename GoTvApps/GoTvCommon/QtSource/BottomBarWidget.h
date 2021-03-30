#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "ToGuiOfferInterface.h"

#include "ui_BottomBarWidget.h"

class GuiUser;
class GuiOfferSession;
class QLabel;
class AppCommon;
class MyIcons;

class BottomBarWidget : public QWidget
{
	Q_OBJECT

public:
	BottomBarWidget( QWidget * parent = 0 );
	virtual ~BottomBarWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    VxMenuButton *              getMenuButton( void );

	//=== bottom bar functions ===// 
	virtual void				setPlayProgressPlayedTime( int timeSec );
	virtual void				setPlayProgressTotalTime( int timeSec );

	//=== bottom bar button visibility ===// 
	void						setPlayProgressBarVisibility( bool visible );
	void						setArrowLeftVisibility( bool visible );
	void						set30SecBackwardVisibility( bool visible );
	void						setMediaPlayVisibility( bool visible );
	void						setMediaTrashVisibility( bool visible );
	void						setMediaFileShareVisibility( bool visible );
	void						setMediaLibraryVisibility( bool visible );
	void						set30SecForwardVisibility( bool visible );
	void						setArrowRightVisibility( bool visible );
	void						setMediaRepeatVisibility( bool visible );
	void						setMenuBottomVisibility( bool visible );
	void						setExpandWindowVisibility( bool visible );

	//=== bottom bar button icon ===// 
	virtual void				setArrowLeftButtonIcon( EMyIcons myIcon = eMyIconArrowLeft );
	virtual void				set30SecBackwardButtonIcon( EMyIcons myIcon = eMyIcon30SecBackward );
	virtual void				setMediaPlayButtonIcon( EMyIcons myIcon = eMyIconPlayNormal );
	virtual void				setMediaTrashButtonIcon( EMyIcons myIcon = eMyIconTrash );
	virtual void				setMediaFileShareButtonIcon( EMyIcons myIcon = eMyIconShareFilesNormal );
	virtual void				setMediaLibraryButtonIcon( EMyIcons myIcon = eMyIconLibraryNormal );
	virtual void				set30SecForwardButtonIcon( EMyIcons myIcon = eMyIcon30SecForward );
	virtual void				setArrowRightButtonIcon( EMyIcons myIcon = eMyIconArrowRight );
	virtual void				setMediaRepeatButtonIcon( EMyIcons myIcon = eMyIconPowerOff );
	virtual void				setMenuBottomButtonIcon( EMyIcons myIcon = eMyIconMenu );
	virtual void				setExpandWindowButtonIcon( EMyIcons myIcon = eMyIconWindowExpand );

	//=== bottom bar button color ===// 
	virtual void				setPlayProgressBarColor( QColor iconColor );
	virtual void				setArrowLeftButtonColor( QColor iconColor );
	virtual void				set30SecBackwardButtonColor( QColor iconColor );
	virtual void				setMediaPlayButtonColor( QColor iconColor );
	virtual void				setMediaTrashButtonColor( QColor iconColor );
	virtual void				setMediaFileShareButtonColor( QColor iconColor );
	virtual void				setMediaLibraryButtonColor( QColor iconColor );
	virtual void				set30SecForwardButtonColor( QColor iconColor );
	virtual void				setArrowRightButtonColor( QColor iconColor );
	virtual void				setMediaRepeatButtonColor( QColor iconColor );
	virtual void				setMenuBottomButtonColor( QColor iconColor );
	virtual void				setExpandWindowButtonColor( QColor iconColor );

signals:
	//=== bottom bar signals ===// 
	void						signalArrowLeftButtonClicked( void );
	void						signal30SecBackwardButtonClicked( void );
	void						signalMediaPlayButtonClicked( void );
	void						signalMediaTrashButtonClicked( void );
	void						signalMediaFileShareClicked( void );
	void						signalMediaLibraryButtonClicked( void );
	void						signal30SecForwardButtonClicked( void );
	void						signalArrowRightButtonClicked( void );
	void						signalMediaRepeatButtonClicked( void );
	void						signalMenuBottomButtonClicked( void );
	void						signalExpandWindowButtonClicked( void );

protected slots:
	//=== bottom bar slots ===// 
	virtual void				slotArrowLeftButtonClicked( void );
	virtual void				slot30SecBackwardButtonClicked( void );
	virtual void				slotMediaPlayButtonClicked( void );
	virtual void				slotMediaTrashButtonClicked( void );
	virtual void				slot30SecForwardButtonClicked( void );
	virtual void				slotMediaFileShareClicked( void );
	virtual void				slotMediaLibraryButtonClicked( void );
	virtual void				slotArrowRightButtonClicked( void );
	virtual void				slotMediaRepeatButtonClicked( void );
	virtual void				slotMenuBottomButtonClicked( void );
	virtual void				slotExpandWindowButtonClicked( void );

protected:
	Ui::BottomBarWidgetClass	ui;
	AppCommon&					m_MyApp;
};
