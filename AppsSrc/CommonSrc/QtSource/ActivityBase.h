#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "ToGuiActivityInterface.h"
#include "ui_ActivityBase.h"

#include "AppDefs.h"
#include "SoundDefs.h"
#include "MyIconsDefs.h"

#include "OfferSessionCallbackInterface.h"

#include <CoreLib/ObjectCommon.h>
#include <CoreLib/VxGUID.h>

#include <QDialog>

class AppCommon;
class AssetBaseInfo;
class IFromGui;
class GuiUserMgr;
class GuiOfferSession;
class IdentWidget;
class MyIcons;
class P2PEngine;
class TitleBarWidget;

class QWidget;
class QLabel;
class QFrame;
class VxPushButton;

class ActivityBase : public QDialog, public ObjectCommon, public ToGuiActivityInterface, public OfferSessionCallbackInterface
{
	Q_OBJECT
public:
	ActivityBase( const char* objName, AppCommon& app, QWidget* parent, EApplet eAppletType = eAppletMessengerFrame, bool isDialog = false, bool isPopup = false, bool fullWindowSize = false );
    virtual ~ActivityBase() override = default;

	AppCommon&					getMyApp( void )					{ return m_MyApp; }
	MyIcons&					getMyIcons( void );
	P2PEngine&					getEngine( void )					{ return m_Engine; }
	IFromGui&					getFromGuiInterface( void )			{ return m_FromGui; }
    QWidget*				    getParentPageFrame( void ); // get home page frame ( Launch or Messenger Page )

    void                        setIsMaxScreenSize( bool fullScreen );
    bool                        getIsMaxScreenSize( void );

	virtual void				setAppletType( EApplet applet )		{ m_EAppletType = applet; }
    virtual EApplet				getAppletType( void )				{ return m_EAppletType; }
    void						setIsAppletLaunchWidget( bool isLaunchWidget ) { m_IsAppletLaunchWidget = isLaunchWidget; }
    bool						getIsAppletLaunchWidget( void )     { return m_IsAppletLaunchWidget; }

	bool                        isMessengerReady( void );
	bool                        isHomeFrame( void );
    bool                        isMessagerFrame( void );

    virtual TitleBarWidget *	getTitleBarWidget( void );
    virtual BottomBarWidget *	getBottomBarWidget( void );
    // get parent frame of this activity
    virtual QFrame*				getContentItemsFrame( void );
    // get content frame of launch page
    virtual QFrame*				getContentFrameOfLaunchFrame( void );
    // get content frame of messenger page
    virtual QFrame*				getContentFrameOfMessengerFrame( void );
    // get content frame of opposit page
    virtual QFrame*				getContentFrameOfOppositePageFrame( void );

    VxPushButton *		        getAppIconPushButton( void )        { return getTitleBarWidget()->getAppIconPushButton(); }
    virtual void                connectBarWidgets( void );
    virtual void				setTitleBarAppletIcon( EMyIcons appletIcon );

    // called just before first show of applet.. override for special initialization needs
    virtual void                aboutToLaunchApplet( void ) {}
 
	void						setNewParent( QWidget* parent );

	void						setupStyledDlg(	GuiUser*	    poFriend			= nullptr,
												IdentWidget *	poIdentWidget		= nullptr,
												EPluginType		ePluginType			= ePluginTypeInvalid,
												VxPushButton *	poPermissionButton	= nullptr,
												QLabel *		poPermissionLabel	= nullptr );

	void						setStatusText( QString statusMsgText );
	void						checkDiskSpace( void );

    virtual void				setPluginType( EPluginType ePluginType )			{ m_ePluginType = ePluginType; }
	virtual EPluginType			getPluginType( void )								{ return m_ePluginType; }
    virtual void				setHisIdent( GuiUser* hisIdent )				    { m_HisIdent = hisIdent; }
    virtual GuiUser*		    getHisIdent( void )									{ return m_HisIdent; }

	// override playMedia if is applet that plays media
	virtual bool				playMedia( AssetBaseInfo& assetInfo, int pos0to100000 = 0 ) { return false; };

	virtual void				okMessageBox( QString title, QString msg );
	virtual void				okMessageBox2( QString title, const char* msg, ... );
	virtual bool				yesNoMessageBox( QString title, QString msg );
	virtual bool				yesNoMessageBox2( QString title, const char* msg, ... );
	virtual void				errMessageBox( QString title, QString msg );
	virtual void				errMessageBox2( QString title, const char* msg, ... );

	//! for derived classes to override and get in session state ( from OfferSessionCallbackInterface )
    virtual void 				onSessionStateChange( ESessionState eSessionState )	override		{}
    virtual void 				onInSession( bool isInSession )		override						{}
    virtual void 				onSessionActivityShouldExit( QString exitReason )	override		{ emit signalShowShouldExitMsgBox( exitReason ); }

	// called when activity finish.. override for exit cleanup
    virtual void				onActivityFinish( void )	{}
    // override default behavior of closing dialog when back button is clicked
    virtual void				onBackButtonClicked( void );
	// override to handle dialog closing
    virtual void				onCloseEvent( void )	{}
	// resizing window
    virtual void				onResizeBegin( QSize& newSize ){}
    virtual void				onResizeEvent( QSize& newSize ){}
    virtual void				onResizeEnd( QSize& newSize ){}

	//=== title bar functions ====//
	void						setTitleBarText( QString titleText );
	void						setTitleStatusBarMsg( QString statusMsg );
	QLabel *					getTitleStatusBarLabel( void );

	//=== bottom bar functions ====//
	virtual void				setPlayProgressPlayedTime( int timeSec );
	virtual void				setPlayProgressTotalTime( int timeSec );

	//=== title bar button visiblility ====//
	void						setPowerButtonVisibility( bool visible );
	void						setHomeButtonVisibility( bool visible );
	void						setNetStatusVisibility( bool visible );
	void						setPersonsOfferListButtonVisibility( bool visible );
	void						setShareOfferListButtonVisibility( bool visible );
	void						setMuteSpeakerVisibility( bool visible );
	void						setMuteMicrophoneVisibility( bool visible );
	void						setMicrophoneVolumeVisibility( bool visible );
	void						setCameraButtonVisibility( bool visible );
	void						setCamPreviewVisibility( bool visible );
	void						setCamViewerCountVisibility( bool visible );
	void						setTrashButtonVisibility( bool visible );
	void						setShareButtonVisibility( bool visible );
	void						setMenuTopButtonVisibility( bool visible );
	void						setMenuListButtonVisibility( bool visible );
	void						setBackButtonVisibility( bool visible );
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

	//=== title bar button icons ====//
	virtual void				setPowerButtonIcon( EMyIcons myIcon = eMyIconPowerOff );
	virtual void				setHomeButtonIcon( EMyIcons myIcon = eMyIconHome );
	virtual void				setMicrophoneIcon( EMyIcons myIcon = eMyIconMicrophoneOn );
	virtual void				setSpeakerIcon( EMyIcons myIcon = eMyIconSpeakerOn );
	virtual void				setCameraIcon( EMyIcons myIcon = eMyIconCameraNormal );
	virtual void				setTrashButtonIcon( EMyIcons myIcon = eMyIconTrash );
	virtual void				setShareButtonIcon( EMyIcons myIcon = eMyIconShare );
	virtual void				setTopMenuButtonIcon( EMyIcons myIcon = eMyIconMenu );
	virtual void				setBackButtonIcon( EMyIcons myIcon = eMyIconBack );
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

	//=== title bar button colors ====//
	virtual void				setPowerButtonColor( QColor iconColor );
	virtual void				setHomeButtonColor( QColor iconColor );
	virtual void				setMicrophoneColor( QColor iconColor );
	virtual void				setSpeakerColor( QColor iconColor );
	virtual void				setCameraColor( QColor iconColor );
	virtual void				setTrashButtonColor( QColor iconColor );
	virtual void				setShareButtonColor( QColor iconColor );
	virtual void				setTopMenuButtonColor( QColor iconColor );
	virtual void				setBackButtonColor( QColor iconColor );
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

	virtual bool				playFile( uint8_t fileType, QString& fullFileName, VxGUID& assetId ) { return false; }
	bool						confirmDeleteFile( QString fileName, bool shredFile );
	bool						confirmDeleteFile( AssetBaseInfo& assetInfo, bool shredFile = false );

signals:
	//=== title bar signals ====//
	void						signalPowerButtonClicked( void );
	void						signalHomeButtonClicked( void );
	void						signalCameraSnapshotButtonClicked( void );
	void						signalCamPreviewClicked( void );
	void						signalTrashButtonClicked( void );
	void						signalShareButtonClicked( void );
	void						signalMenuTopButtonClicked( void );
	void						signalBackButtonClicked( void );
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

	//==== activity signals ====//
	void						signalActivityBaseWasResized( void );
	void						signalPlayNotifySound( void );
	void						signalPlayShredderSound( void );
	void						signalDialogWasShown( void );

	void						signalShowShouldExitMsgBox( QString exitReason );

public slots:
	virtual void				closeApplet( void );

protected slots:

	//==== activity slots ====//
	virtual void				slotRepositionToParent( void );
	virtual void				slotActivityFinished( int finishResult );
	void						slotStatusMsg( QString strMsg );
	void						slotShowShouldExitMsgBox( QString exitReason );

	void						slotResizeWindowTimeout( void );

	//=== title bar slots ====//
	virtual void				slotPowerButtonClicked( void );
	virtual void				slotHomeButtonClicked( void );
	virtual void				slotCameraSnapshotButtonClicked( void );
	virtual void				slotCamPreviewClicked( void );
	virtual void				slotTrashButtonClicked( void );
	virtual void				slotShareButtonClicked( void );
	virtual void				slotMenuTopButtonClicked( void );
	virtual void				slotBackButtonClicked( void );
    virtual void				slotAppIconSpecialClick( void );

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
    virtual void				showEvent( QShowEvent* ) override;
    virtual void				closeEvent( QCloseEvent * ev ) override;
    virtual void				resizeEvent( QResizeEvent* ev ) override;

    void                        connectTitleBarWidget( TitleBarWidget * titleBar );
    void                        connectBottomBarWidget( BottomBarWidget * bottomBar );

    virtual void                updateExpandWindowIcon( void );
	virtual void				repositionToParent( void );
	void						playSound( ESndDef sndDef );
	virtual bool				playFile( QString fileName, int pos0to100000 = 0 );

    void                        fillMyNodeUrl( QLabel * myUrlLabel );

    virtual void                onAppIconSpecialClick( ActivityBase * activityBase );

	//=== vars ===//
	Ui::ActivityBaseClass		ui;

	AppCommon&					m_MyApp;
    GuiUserMgr&                 m_UserMgr;
	P2PEngine&					m_Engine;
	IFromGui&					m_FromGui;	
    Qt::WindowFlags				m_WindowFlags = Qt::Widget;
    QWidget*					m_ParentWidget{nullptr};

	EApplet						m_EAppletType = eAppletUnknown;
    EPluginType					m_ePluginType = ePluginTypeInvalid;

    bool                        m_IsAppletLaunchWidget{false};
    QLabel *					m_StatusMsgLabel{nullptr};

    GuiUser*				    m_HisIdent{nullptr};
    QTimer *					m_ResizingTimer{nullptr};
    bool						m_IsResizing{false};
	QSize						m_ResizingWindowSize;
    bool						m_IsDialog{false};
    bool						m_IsPopup{false}; // center on parent filling about half of parent
    bool						m_FullWindowSize{false};
    bool						m_InitialFocusWasSet{false};
	bool						m_HasBeenClosed{ false };
};
