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
#include "MyIconsDefs.h"

#include "ui_TitleBarWidget.h"
#include <QListWidgetItem>

class Friend;
class GuiOfferSession;
class QLabel;
class AppCommon;
class MyIcons;

class TitleBarWidget : public QWidget
{
	Q_OBJECT

public:
	TitleBarWidget( QWidget * parent = 0 );
	virtual ~TitleBarWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setTitleBarText( QString titleText );
	void						setTitleStatusBarMsg( QString statusMsg );
	QLabel *					getTitleStatusBarLabel( void );
	VxPushButton *				getAppIconPushButton( void );
    VxPushButton *				getBackButton( void ) { return ui.m_BackDlgButton; }

    void						setTitleBarAppletIcon( EMyIcons appletIcon );

	void						enableAudioControls( bool enable );
	void						enableVideoControls( bool enable );

	//=== button visiblility ====//
    void				        setBackButtonVisibility( bool visible );
	void						setCamPreviewVisibility( bool visible );
	void						setCamViewerCountVisibility( bool visible );
	void						setCameraButtonVisibility( bool visible );
    void						setHomeButtonVisibility( bool visible );
    void						setMuteSpeakerVisibility( bool visible );
    void						setMuteMicrophoneVisibility( bool visible );
    void						setMenuTopButtonVisibility( bool visible );
    void						setPowerButtonVisibility( bool visible );
	void						setShareButtonVisibility( bool visible );
    void						setTrashButtonVisibility( bool visible );

	//=== button icons ====//
    virtual void				setPowerButtonIcon( EMyIcons myIcon = eMyIconPowerOff );
	virtual void				setHomeButtonIcon( EMyIcons myIcon = eMyIconHome );
	virtual void				setMicrophoneIcon( EMyIcons myIcon = eMyIconMicrophoneOn );
	virtual void				setSpeakerIcon( EMyIcons myIcon = eMyIconSpeakerOn );
	virtual void				setCameraIcon( EMyIcons myIcon = eMyIconCameraNormal );
	virtual void				setTrashButtonIcon( EMyIcons myIcon = eMyIconTrash );
	virtual void				setShareButtonIcon( EMyIcons myIcon = eMyIconShare );
	virtual void				setTopMenuButtonIcon( EMyIcons myIcon = eMyIconMenu );
	virtual void				setBackButtonIcon( EMyIcons myIcon = eMyIconBack );

	//=== button colors ====//
	virtual void				setPowerButtonColor( QColor iconColor );
	virtual void				setHomeButtonColor( QColor iconColor );
	virtual void				setMicrophoneColor( QColor iconColor );
	virtual void				setSpeakerColor( QColor iconColor );
	virtual void				setCameraColor( QColor iconColor );
	virtual void				setTrashButtonColor( QColor iconColor );
	virtual void				setShareButtonColor( QColor iconColor );
	virtual void				setTopMenuButtonColor( QColor iconColor );
	virtual void				setBackButtonColor( QColor iconColor );

signals:
	void						signalPowerButtonClicked( void );
	void						signalHomeButtonClicked( void );
	void						signalMuteMicButtonClicked( bool muteMic );
	void						signalMuteSpeakerButtonClicked( bool muteSpeaker );
	void						signalCameraSnapshotButtonClicked( void );
	void						signalCamPreviewClicked( void );

	void						signalTrashButtonClicked( void );
	void						signalShareButtonClicked( void );
	void						signalMenuTopButtonClicked( void );
	void						signalBackButtonClicked( void );

protected slots:
	virtual void				slotPowerButtonClicked( void );
	virtual void				slotHomeButtonClicked( void );
	virtual void				slotMuteMicButtonClicked( void );
	virtual void				slotMuteSpeakerButtonClicked( void );
	virtual void				slotCameraSnapshotButtonClicked( void );
	virtual void				slotCamPreviewClicked( void );

	virtual void				slotTrashButtonClicked( void );
	virtual void				slotShareButtonClicked( void );
	virtual void				slotMenuTopButtonClicked( void );
	virtual void				slotBackButtonClicked( void );

	virtual void				slotTitleStatusBarMsg( QString msg );
	virtual void				slotToGuiPluginStatus( EPluginType ePluginType, int statusType, int statusValue );

protected:
	Ui::TitleBarWidgetClass		ui;
	AppCommon&					m_MyApp;
	bool						m_MuteMic;
	bool						m_MuteSpeaker;
	bool						m_EchoCancelEnabled;
};
