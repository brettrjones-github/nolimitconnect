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
// http://www.nolimitconnect.org
//============================================================================

#include "ToGuiOfferInterface.h"
#include "ToGuiActivityInterface.h"
#include "ToGuiHardwareControlInterface.h"
#include "MyIconsDefs.h"

#include "ui_TitleBarWidget.h"
#include <QListWidgetItem>
#include <QTimer>

#include <CoreLib/VxGUID.h>

class AppCommon;
class GuiOfferClientMgr;
class GuiOfferSession;
class GuiUser;
class MyIcons;
class QLabel;

class TitleBarWidget : public QWidget, public ToGuiActivityInterface, public ToGuiHardwareControlInterface
{
	Q_OBJECT

public:
	TitleBarWidget( QWidget * parent = 0 );
	virtual ~TitleBarWidget() = default;

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setTitleBarText( QString titleText );
	void						setTitleStatusBarMsg( QString statusMsg );
	QLabel *					getTitleStatusBarLabel( void );
	VxPushButton *				getAppIconPushButton( void );
    VxPushButton *				getBackButton( void ) { return ui.m_BackDlgButton; }

	void						enableAudioControls( bool enable );
	void						enableVideoControls( bool enable );

	//=== button visiblility ====//
	void						setPopupVisibility( void );// for popup dialogs

    void				        setBackButtonVisibility( bool visible );
	void						setCamPreviewVisibility( bool visible );
	void						setCamViewerCountVisibility( bool visible );
	void						setCameraButtonVisibility( bool visible );
    void						setHomeButtonVisibility( bool visible );
	void						setNetStatusVisibility( bool visible );
    void						setMuteSpeakerVisibility( bool visible );
    void						setMuteMicrophoneVisibility( bool visible );
	void						setMicrophoneVolumeVisibility( bool visible );
    void						setMenuTopButtonVisibility( bool visible );
	void						setMenuListButtonVisibility( bool visible );
    void						setPowerButtonVisibility( bool visible );
	void						setPersonsOfferListButtonVisibility( bool visible );
	void						setShareOfferListButtonVisibility( bool visible );
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
    void                        signalCamPlaying( bool isPlaying );

public slots:
	virtual void				updateTitleBar( void );
	virtual void				slotSystemReady( bool isReady );

	virtual void				slotApplicationIconClicked( void );
	virtual void				slotPowerButtonClicked( void );
	virtual void				slotHomeButtonClicked( void );
    virtual void				slotMicrophonePeak( int peekVal0to32768 );
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
    virtual void				slotToGuiNetAvailStatus( ENetAvailStatus eNetAvailStatus );
    virtual void                slotCamPlaying( bool isPlaying );
    virtual void				slotCamTimeout( void );
    virtual void				slotSignalHelpClick( void );

    virtual void				slotPersonsOfferListButtonClicked( void );
    virtual void				slotShareOfferListButtonClicked( void );

    virtual void				slotShareOfferListCount( int activeCnt );
    virtual void				slotHostJoinRequestCount( int activeCnt );

	virtual void				slotTitleBarMenuButtonClicked( void );

	virtual void				slotMicrophonePeekTimeout( void );

protected:
	QWidget*					getTitleBarParentFrame( void );
	QWidget*					getTitleBarParentPage( void );
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;
	void						resizeEvent( QResizeEvent* ev ) override;

    virtual void				toGuiClientPlayVideoFrame( VxGUID&          onlineId,
                                                           uint8_t *		pu8Jpg,
                                                           uint32_t		    u32JpgDataLen,
                                                           int				motion0To100000 ) override;
	void						checkTitleBarIconsFit( void );

	virtual void 				callbackToGuiWantMicrophoneRecording( bool wantMicInput ) override;
	virtual void 				callbackToGuiWantSpeakerOutput( bool wantSpeakerOutput ) override;
	virtual void				callbackToGuiWantVideoCapture( bool wantVideoCapture ) override;

	Ui::TitleBarWidgetClass		ui;
	AppCommon&					m_MyApp;
    GuiOfferClientMgr&          m_OfferClientMgr;
	bool						m_MuteMic{ false };
	bool						m_MuteSpeaker{ false };
	bool						m_EchoCancelEnabled{ false };
    VxGUID                      m_MyOnlineId;
	bool                        m_CallbacksRequested{ false };

    QTimer*                     m_CamTimer;
    bool                        m_CamPlaying{ false };
    uint64_t                    m_LastCamFrameTimeMs{ 0 };

	QTimer*						m_MicrophonePeekTimer;
	bool                        m_MicrophonePlaying{ false };
	uint64_t                    m_LastMicrophonePeekTimeMs{ 0 };
};
