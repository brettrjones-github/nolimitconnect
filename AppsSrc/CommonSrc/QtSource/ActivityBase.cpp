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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityBase.h"
#include "ActivityYesNoMsgBox.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "GuiOfferSession.h"
#include "ActivityMessageBox.h"
#include "IdentWidget.h"

#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MySndMgr.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include "AppSettings.h"
#include <CoreLib/VxFileUtil.h>

#include <QRect>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QResizeEvent>

#include <stdio.h>

const int RESIZE_WINDOW_COMPLETED_TIMEOUT = 500;

//============================================================================
ActivityBase::ActivityBase( const char * objName, AppCommon& app, QWidget * parent, EApplet eAppletType, bool isDialog, bool isPopup, bool fullWindowSize )
: QDialog( parent, Qt::Widget )
, ObjectCommon( objName )
, m_MyApp( app )
, m_UserMgr( app.getUserMgr() )
, m_Engine( app.getEngine() )
, m_FromGui( m_Engine.getFromGuiInterface() )
, m_ParentWidget( parent )
, m_EAppletType( eAppletType )
, m_ePluginType( ePluginTypeInvalid )
, m_ResizingTimer( new QTimer(this) )
, m_IsDialog( isDialog )
, m_IsPopup( isPopup )
, m_FullWindowSize( fullWindowSize )
{
    vx_assert( objName );
    setObjectName( objName );
	VxAppStyle::clearFocusFrameWidget();

	if( 0xcdcdcdcdcdcdcdcd == (uint64_t)parent )
	{
		vx_assert( false );
		LogMsg( LOG_FATAL, "ActivityBase::ActivityBase: Bad Param");
	}

    m_IsDialog = isDialog 
                || ( eAppletUnknown == eAppletType ) 
                || ( eAppletActivityDialog == eAppletType ); // do not setup base class ui in the case of activity dialog because of conflict with dialog ui
    if( !m_IsDialog && !m_IsPopup )
    {
        m_WindowFlags = Qt::SubWindow;
        setWindowFlags( m_WindowFlags );
        ui.setupUi( this );
    }
    else if( m_IsPopup )
    {
        //m_WindowFlags = Qt::Popup;
        m_WindowFlags = Qt::Dialog | Qt::WindowStaysOnTopHint;
        setWindowFlags( m_WindowFlags );

        connect( &m_MyApp, SIGNAL( signalMainWindowMoved() ), this, SLOT( slotRepositionToParent() ) );

        LogMsg( LOG_DEBUG, "ActivityBase::ActivityBase: Activity Popup %s\n", objectName().toUtf8().constData() );
    }
    else if( parent )
    {
        // dialog needs to cover parent

        //m_WindowFlags = Qt::Window;
        //m_WindowFlags = Qt::SubWindow;
        //m_WindowFlags = Qt::Popup;
        m_WindowFlags = Qt::Dialog | Qt::FramelessWindowHint;
        //m_WindowFlags = Qt::Sheet;
        //m_WindowFlags = Qt::Drawer;
        //m_WindowFlags = Qt::Tool;
        //m_WindowFlags = Qt::ForeignWindow;
        //m_WindowFlags = Qt::CoverWindow;
        setWindowFlags( m_WindowFlags );
        m_MyApp.getAppTheme().applyTheme( this );
        if( !m_FullWindowSize )
        {
            m_ParentWidget = getParentPageFrame();
        }

        connect( &m_MyApp, SIGNAL( signalMainWindowMoved() ), this, SLOT( slotRepositionToParent() ) );

        LogMsg( LOG_DEBUG, "ActivityBase::ActivityBase: Activity Dialog %s\n", objectName().toUtf8().constData() );
    }

	connect( &m_MyApp,			SIGNAL( signalMainWindowResized() ),			this, SLOT( slotRepositionToParent() ) );
    connect( &m_MyApp,          SIGNAL( signalMainWindowMoved() ),              this, SLOT( slotRepositionToParent() ) );

	connect( &m_MyApp,			SIGNAL(signalUserMsg(QString)),					this, SLOT(slotStatusMsg(QString)) );
	connect( &m_MyApp,			SIGNAL(signalStatusMsg(QString)),				this, SLOT(slotStatusMsg(QString)) );
	connect( this,				SIGNAL(signalShowShouldExitMsgBox(QString)),	this, SLOT(slotShowShouldExitMsgBox(QString)), Qt::QueuedConnection );
	connect( this,				SIGNAL(finished(int)),							this, SLOT(slotActivityFinished(int)) );

	connect( m_ResizingTimer,	SIGNAL( timeout() ), this, SLOT( slotResizeWindowTimeout() ) );

	connect(	this, 
				SIGNAL(signalPlayNotifySound()), 
				&m_MyApp.getSoundMgr(), 
				SLOT(slotPlayNotifySound()) );

	connect(	this, 
				SIGNAL(signalPlayShredderSound()), 
				&m_MyApp.getSoundMgr(), 
				SLOT(slotPlayShredderSound()) );

    if( !m_IsDialog && !m_IsPopup )
    {
        // if dialog then have to wait for dialog sets up title and bottom bar widgets before connecting them
        connectBarWidgets();
    }
}

//============================================================================
void ActivityBase::connectBarWidgets( )
{
    connectTitleBarWidget( getTitleBarWidget() );
    connectBottomBarWidget( getBottomBarWidget() );
    updateExpandWindowIcon();
    if( m_IsDialog || m_IsPopup )
    {
        slotRepositionToParent();
    }
}

//============================================================================
void ActivityBase::connectTitleBarWidget( TitleBarWidget * titleBar )
{
    //=== title bar connections ====//
    connect( titleBar, SIGNAL( signalPowerButtonClicked() ), this, SLOT( slotPowerButtonClicked() ) );
    connect( titleBar, SIGNAL( signalHomeButtonClicked() ), this, SLOT( slotHomeButtonClicked() ) );
    connect( titleBar, SIGNAL( signalMuteMicButtonClicked( bool ) ), this, SLOT( slotMuteMicButtonClicked( bool ) ) );
    connect( titleBar, SIGNAL( signalMuteSpeakerButtonClicked( bool ) ), this, SLOT( slotMuteSpeakerButtonClicked( bool ) ) );
    connect( titleBar, SIGNAL( signalCameraSnapshotButtonClicked() ), this, SLOT( slotCameraSnapshotButtonClicked() ) );
    connect( titleBar, SIGNAL( signalCamPreviewClicked() ), this, SLOT( slotCamPreviewClicked() ) );
    connect( titleBar, SIGNAL( signalTrashButtonClicked() ), this, SLOT( slotTrashButtonClicked() ) );
    connect( titleBar, SIGNAL( signalShareButtonClicked() ), this, SLOT( slotShareButtonClicked() ) );
    connect( titleBar, SIGNAL( signalMenuTopButtonClicked() ), this, SLOT( slotMenuTopButtonClicked() ) );
    connect( titleBar, SIGNAL( signalBackButtonClicked() ), this, SLOT( slotBackButtonClicked() ) );
}

//============================================================================
void ActivityBase::connectBottomBarWidget( BottomBarWidget * bottomBar )
{
    //=== bottom bar signals ===// 
    connect( bottomBar, SIGNAL( signalArrowLeftButtonClicked() ), this, SLOT( slotArrowLeftButtonClicked() ) );
    connect( bottomBar, SIGNAL( signal30SecBackwardButtonClicked() ), this, SLOT( slot30SecBackwardButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalMediaPlayButtonClicked() ), this, SLOT( slotMediaPlayButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalMediaTrashButtonClicked() ), this, SLOT( slotMediaTrashButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalMediaFileShareClicked() ), this, SLOT( slotMediaFileShareClicked() ) );
    connect( bottomBar, SIGNAL( signalMediaLibraryButtonClicked() ), this, SLOT( slotMediaLibraryButtonClicked() ) );
    connect( bottomBar, SIGNAL( signal30SecForwardButtonClicked() ), this, SLOT( slot30SecForwardButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalArrowRightButtonClicked() ), this, SLOT( slotArrowRightButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalMediaRepeatButtonClicked() ), this, SLOT( slotMediaRepeatButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalMenuBottomButtonClicked() ), this, SLOT( slotMenuBottomButtonClicked() ) );
    connect( bottomBar, SIGNAL( signalExpandWindowButtonClicked() ), this, SLOT( slotExpandWindowButtonClicked() ) );
}

//============================================================================
bool ActivityBase::isMessengerReady( void )
{
	return m_MyApp.isMessengerReady();
}

//============================================================================
bool ActivityBase::isMessagerFrame( void )
{
    return GuiHelpers::isMessagerFrame( this );
}

//============================================================================
bool ActivityBase::isHomeFrame( void )
{
	return !GuiHelpers::isMessagerFrame( this );
}

//============================================================================
bool ActivityBase::getIsMaxScreenSize( void )
{
     return m_MyApp.getIsMaxScreenSize( isMessagerFrame() );
}

//============================================================================
void ActivityBase::setIsMaxScreenSize( bool fullScreen )
{
    m_MyApp.setIsMaxScreenSize( isMessagerFrame(), fullScreen );
}

//============================================================================
// only available for applets
QFrame * ActivityBase::getContentItemsFrame( void )
{
    return ui.m_ContentItemsFrame;
}

//============================================================================
// only available for applets
QFrame * ActivityBase::getContentFrameOfLaunchFrame( void )
{
    return GuiHelpers::getLaunchPageFrame( ui.m_ContentItemsFrame );
}

//============================================================================
// only available for applets
QFrame * ActivityBase::getContentFrameOfMessengerFrame( void )
{
    return GuiHelpers::getMessengerPageFrame( ui.m_ContentItemsFrame );
}

//============================================================================
// only available for applets
QFrame * ActivityBase::getContentFrameOfOppositePageFrame( void )
{
    return GuiHelpers::getOppositePageFrame( ui.m_ContentItemsFrame );
}

//============================================================================
// get home page activity ( Launch or Messenger Page )
QWidget * ActivityBase::getParentPageFrame( void )
{
    return GuiHelpers::getParentPageFrame( this );
}

//============================================================================
// overridden in dialogs
TitleBarWidget * ActivityBase::getTitleBarWidget( void )
{
	return ui.m_TitleBarWidget;
}

//============================================================================
// overridden in dialogs
BottomBarWidget * ActivityBase::getBottomBarWidget( void )
{
	return ui.m_BottomBarWidget;
}

//============================================================================
void ActivityBase::setupStyledDlg(	GuiUser *           poFriend,
									IdentWidget *		poIdentWidget,
									EPluginType			ePluginType,
									VxPushButton *		poPermissionButton,
									QLabel *			poPermissionLabel )
{
	if( poIdentWidget )
	{
		poIdentWidget->setIdentMenuButtonVisible( false );
		poIdentWidget->updateIdentity( poFriend );
	}

	if( poPermissionButton )
	{
		EPluginAccess ePluginAccess = poFriend->getMyAccessPermissionFromHim(ePluginType);

		poPermissionButton->setIcon( getMyIcons().getPluginIcon( ePluginType, ePluginAccess ) );
		QString strAction = GuiParams::describePluginAction( poFriend, ePluginType, ePluginAccess );
		poPermissionLabel->setText( strAction );
	}
}

//============================================================================
void ActivityBase::setTitleBarText( QString titleText )
{
	getTitleBarWidget()->setTitleBarText( titleText );
}

//============================================================================
void ActivityBase::setTitleStatusBarMsg( QString statusMsg )
{
    getTitleBarWidget()->setTitleStatusBarMsg( statusMsg );
}

//============================================================================
QLabel * ActivityBase::getTitleStatusBarLabel( void )
{
	return getTitleBarWidget()->getTitleStatusBarLabel();
}

//============================================================================  
void ActivityBase::setStatusText( QString statusMsgText )
{
	m_MyApp.toGuiStatusMessage( statusMsgText.toUtf8().constData() );
}

//============================================================================  
void ActivityBase::setNewParent( QWidget * parent )
{
	bool dlgIsVisible = isVisible();
	m_ParentWidget = parent;
	setParent( parent );
	if( dlgIsVisible )
	{
		show();
	}

	repositionToParent();
}

//============================================================================  
void ActivityBase::checkDiskSpace( void )
{
	uint64_t diskFreeSpace = m_Engine.fromGuiGetDiskFreeSpace();
	if( ( 0 != diskFreeSpace ) && ( diskFreeSpace < 1000000000 ) )
	{
        m_MyApp.toGuiUserMessage( "Disk Space is low %s", GuiParams::describeFileLength( diskFreeSpace ).toUtf8().constData() );
	}
}

//============================================================================
MyIcons& ActivityBase::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void ActivityBase::slotStatusMsg( QString strMsg )
{
	//LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
	if( 0 != m_StatusMsgLabel )
	{
		m_StatusMsgLabel->setText( strMsg );
	}
}

//============================================================================
void ActivityBase::slotShowShouldExitMsgBox( QString exitReason )
{
	QMessageBox::information( this, QObject::tr("Session End Message"), exitReason, QMessageBox::Ok );
	accept();
}

//============================================================================
bool ActivityBase::playFile( QString fileName, int pos0to100000 )
{
	return GuiHelpers::playFile( m_MyApp, fileName, pos0to100000 );
}

//============================================================================
bool ActivityBase::playFile( QString fileName, VxGUID& assetId, int pos0to100000 )
{
	return GuiHelpers::playFile( m_MyApp, fileName, assetId, pos0to100000 );
}

//============================================================================
void ActivityBase::playSound( ESndDef sndDef )
{
	m_MyApp.playSound( sndDef );
}

//============================================================================
bool ActivityBase::confirmDeleteFile( QString fileName, bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ? "Confirm Shred File" : "Confirm Delete File";
		QString bodyText = "";
		if( shredFile )
		{
			bodyText = "Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?";
		}
		else
		{
			bodyText = "Are You Sure To Delete The File From The Device?";
		}

		ActivityYesNoMsgBox dlg( m_MyApp, &m_MyApp, title, bodyText );
		dlg.makeNeverShowAgainVisible( true );
		if( false == ( QDialog::Accepted == dlg.exec() ) )
		{
			acceptAction = false;
		}

		if( dlg.wasNeverShowAgainChecked() )
		{
			m_MyApp.getAppSettings().setIsConfirmDeleteDisabled( true );
		}
	}

	return acceptAction;
}

//============================================================================
void ActivityBase::okMessageBox( QString title, QString msg )
{
	QMessageBox::information( this, title, msg, QMessageBox::Ok );
}

//============================================================================
void ActivityBase::okMessageBox2( QString title, const char * msg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start( argList, msg );
	vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
	va_end( argList );

	okMessageBox( title, szBuffer );
}

//============================================================================
bool ActivityBase::yesNoMessageBox( QString title, QString msg )
{
	if( QMessageBox::Yes == QMessageBox::question( this, title, msg, QMessageBox::Yes | QMessageBox::No ) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool ActivityBase::yesNoMessageBox2( QString title, const char * msg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start( argList, msg );
	vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
	va_end( argList );

	return yesNoMessageBox( title, szBuffer );
}

//============================================================================
void ActivityBase::errMessageBox( QString title, QString msg )
{
	QMessageBox::warning( this, title, msg, QMessageBox::Ok );
}

//============================================================================
void ActivityBase::errMessageBox2( QString title, const char * msg, ... )
{
	if( 0 != msg )
	{
		char szBuffer[2048];
		va_list argList;
		va_start( argList, msg );
		vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
		va_end( argList );

		errMessageBox( title, szBuffer );
	}
}

//============================================================================
void ActivityBase::showEvent( QShowEvent * showEvent )
{
#if defined(TARGET_OS_ANDROID)
    // not sure why but android seems to loose the theme text color so reapply
    m_MyApp.getAppTheme().applyTheme( this );
#endif // defined(TARGET_OS_ANDROID)

    if( m_ParentWidget )
	{
		slotRepositionToParent();
	}

	QDialog::showEvent( showEvent );
#if defined(TARGET_OS_ANDROID)
    // not sure why but android seems to loose the theme text color so reapply
    m_MyApp.getAppTheme().applyTheme( this );
#endif // defined(TARGET_OS_ANDROID)

    if( !m_InitialFocusWasSet )
    {
        m_InitialFocusWasSet = true;
        if( getTitleBarWidget() )
        {
            getTitleBarWidget()->getBackButton()->setFocus();
        }
    }

	emit signalDialogWasShown();
#if defined(TARGET_OS_ANDROID)
    // not sure why but android seems to loose the theme text color so reapply
    m_MyApp.getAppTheme().applyTheme( this );
    m_MyApp.getAppTheme().selectTheme( m_MyApp.getAppSettings().getLastSelectedTheme() );
#endif // defined(TARGET_OS_ANDROID)

}

//============================================================================
void ActivityBase::resizeEvent( QResizeEvent * ev )
{
	QDialog::resizeEvent( ev );
    updateExpandWindowIcon();
	emit signalActivityBaseWasResized();
	m_ResizingWindowSize = ev->size();
	if( !m_IsResizing )
	{
		m_IsResizing = true;
		onResizeBegin( m_ResizingWindowSize );
	}

	onResizeEvent( m_ResizingWindowSize );
	m_ResizingTimer->setSingleShot( true );
	m_ResizingTimer->start( RESIZE_WINDOW_COMPLETED_TIMEOUT );
}

//============================================================================
void ActivityBase::closeEvent( QCloseEvent * ev )
{
	onCloseEvent();
	m_MyApp.activityStateChange( this, false );
	QDialog::closeEvent( ev );
}

//============================================================================
void ActivityBase::slotExpandWindowButtonClicked( void )
{
    m_MyApp.setIsMaxScreenSize( isMessagerFrame(), !m_MyApp.getIsMaxScreenSize( isMessagerFrame() ) );
    updateExpandWindowIcon();
}

//============================================================================
void ActivityBase::updateExpandWindowIcon( void )
{
    if( !m_IsDialog && !m_IsPopup )
    {
        if( m_MyApp.getIsMaxScreenSize( isMessagerFrame() ) )
        {
            ui.m_BottomBarWidget->setExpandWindowButtonIcon( eMyIconWindowShrink );
        }
        else
        {
            ui.m_BottomBarWidget->setExpandWindowButtonIcon( eMyIconWindowExpand );
        }
    }
    else if( getBottomBarWidget() )
    {
        if( m_MyApp.getIsMaxScreenSize( isMessagerFrame() ) )
        {
            getBottomBarWidget()->setExpandWindowButtonIcon( eMyIconWindowShrink );
        }
        else
        {
            getBottomBarWidget()->setExpandWindowButtonIcon( eMyIconWindowExpand );
        }
    }
}

//============================================================================
void ActivityBase::slotRepositionToParent( void )
{
	repositionToParent();
}

//============================================================================
void ActivityBase::slotActivityFinished( int finishResult )
{
	onActivityFinish();
}

//============================================================================
void ActivityBase::repositionToParent( void )
{
	if( m_ParentWidget )
		//&& ( 0 == ( m_WindowFlags & Qt::Popup ) ) )
	{
		if( 0xcdcdcdcdcdcdcdcd == (uint64_t)m_ParentWidget )
		{
			vx_assert( false );
		}

		QRect parentRect = m_ParentWidget->geometry();
		if( ( 0 >= parentRect.width() )
			|| ( 0 >= parentRect.height() ) )
		{
			// invalid window rectangle
			return;
		}

        if( m_IsPopup )
        {
            QRect parentRect( m_ParentWidget->mapToGlobal( QPoint( 0, 0 ) ), m_ParentWidget->size() );

            move( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(), parentRect ).topLeft() );
            LogMsg( LOG_DEBUG, "Popup %s size %d %d", getObjName(), parentRect.width(), parentRect.height() );
        }
        else if( m_IsDialog )
        {
            QRect parentRect( m_ParentWidget->mapToGlobal( QPoint( 0, 0 ) ), m_ParentWidget->size() );
            // LogMsg( LOG_DEBUG, "Dialog %s size %d %d", getObjName(), parentRect.width(), parentRect.height() );
            setGeometry( parentRect );
        }
        else
        {
            parentRect.setRight( parentRect.right() - parentRect.left() );
            parentRect.setLeft( 0 );
            parentRect.setBottom( parentRect.bottom() - parentRect.top() );
            parentRect.setTop( 0 );

            //LogMsg( LOG_DEBUG, "Reposition to x=%d y=%d w=%d h=%d %s parent %s\n",
            //    parentRect.left(), parentRect.top(), parentRect.width(), parentRect.height(), getObjName(), m_ParentWidget->objectName().toUtf8().constData() );
            setGeometry( parentRect );
            // LogMsg( LOG_DEBUG, "Normal %s size %d %d", getObjName(), parentRect.width(), parentRect.height() );
        }
	}
    else
    {
        LogMsg( LOG_WARNING, "Object %s has no parent page\n", objectName().toUtf8().constData() );
    }
}

//============================================================================
//=== bottom bar functions ====//
//============================================================================
void ActivityBase::setPlayProgressPlayedTime( int timeSec )
{
	ui.m_BottomBarWidget->setPlayProgressPlayedTime( timeSec );
}

//============================================================================
void ActivityBase::setPlayProgressTotalTime( int timeSec )
{
	ui.m_BottomBarWidget->setPlayProgressTotalTime( timeSec );
}

//============================================================================
//=== title bar button visiblility ====//
//============================================================================
void ActivityBase::setPowerButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setPowerButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setHomeButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setHomeButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setNetStatusVisibility( bool visible )
{
	ui.m_TitleBarWidget->setNetStatusVisibility( visible );
}

//============================================================================
void ActivityBase::setPersonsOfferListButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setPersonsOfferListButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setShareOfferListButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setShareOfferListButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setMuteSpeakerVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMuteSpeakerVisibility( visible );
}

//============================================================================
void ActivityBase::setMuteMicrophoneVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMuteMicrophoneVisibility( visible );
}

//============================================================================
void ActivityBase::setMicrophoneVolumeVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMicrophoneVolumeVisibility( visible );
}

//============================================================================
void ActivityBase::setCameraButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setCameraButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setCamPreviewVisibility( bool visible )
{
	ui.m_TitleBarWidget->setCamPreviewVisibility( visible );
}

//============================================================================
void ActivityBase::setCamViewerCountVisibility( bool visible )
{
	ui.m_TitleBarWidget->setPowerButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setTrashButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setTrashButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setShareButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setShareButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setMenuTopButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMenuTopButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setMenuListButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMenuListButtonVisibility( visible );
}

//============================================================================
void ActivityBase::setBackButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setBackButtonVisibility( visible );
}

//=== bottom bar button visibility ===// 
//============================================================================
void ActivityBase::setPlayProgressBarVisibility( bool visible )
{
	ui.m_BottomBarWidget->setPlayProgressBarVisibility( visible );
}

//============================================================================
void ActivityBase::setArrowLeftVisibility( bool visible )
{
	ui.m_BottomBarWidget->setArrowLeftVisibility( visible );
}

//============================================================================
void ActivityBase::set30SecBackwardVisibility( bool visible )
{
	ui.m_BottomBarWidget->set30SecBackwardVisibility( visible );
}

//============================================================================
void ActivityBase::setMediaPlayVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMediaPlayVisibility( visible );
}

//============================================================================
void ActivityBase::setMediaTrashVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMediaTrashVisibility( visible );
}

//============================================================================
void ActivityBase::setMediaFileShareVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMediaFileShareVisibility( visible );
}

//============================================================================
void ActivityBase::setMediaLibraryVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMediaLibraryVisibility( visible );
}

//============================================================================
void ActivityBase::set30SecForwardVisibility( bool visible )
{
	ui.m_BottomBarWidget->set30SecForwardVisibility( visible );
}

//============================================================================
void ActivityBase::setArrowRightVisibility( bool visible )
{
	ui.m_BottomBarWidget->setArrowRightVisibility( visible );
}

//============================================================================
void ActivityBase::setMediaRepeatVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMediaRepeatVisibility( visible );
}

//============================================================================
void ActivityBase::setMenuBottomVisibility( bool visible )
{
	ui.m_BottomBarWidget->setMenuBottomVisibility( visible );
}

//============================================================================
void ActivityBase::setExpandWindowVisibility( bool visible )
{
	ui.m_BottomBarWidget->setExpandWindowVisibility( visible );
}

//============================================================================
//=== title bar button icons ====//
//============================================================================
void ActivityBase::setPowerButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setPowerButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setHomeButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setHomeButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMicrophoneIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setMicrophoneIcon( myIcon );
}

//============================================================================
void ActivityBase::setSpeakerIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setSpeakerIcon( myIcon );
}

//============================================================================
void ActivityBase::setCameraIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setCameraIcon( myIcon );
}

//============================================================================
void ActivityBase::setTrashButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setTrashButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setShareButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setShareButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setTopMenuButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setTopMenuButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setBackButtonIcon( EMyIcons myIcon )
{
	ui.m_TitleBarWidget->setBackButtonIcon( myIcon );
}

//=== bottom bar button icon ===// 
//============================================================================
void ActivityBase::setArrowLeftButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setArrowLeftButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::set30SecBackwardButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setArrowLeftButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMediaPlayButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMediaPlayButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMediaTrashButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMediaTrashButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMediaFileShareButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMediaFileShareButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMediaLibraryButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMediaLibraryButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::set30SecForwardButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->set30SecForwardButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setArrowRightButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setArrowLeftButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMediaRepeatButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMediaRepeatButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setMenuBottomButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setMenuBottomButtonIcon( myIcon );
}

//============================================================================
void ActivityBase::setExpandWindowButtonIcon( EMyIcons myIcon )
{
	ui.m_BottomBarWidget->setExpandWindowButtonIcon( myIcon );
}

//============================================================================
//=== title bar button colors ====//
//============================================================================
void ActivityBase::setPowerButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setPowerButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setHomeButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setHomeButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMicrophoneColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setMicrophoneColor( iconColor );
}

//============================================================================
void ActivityBase::setSpeakerColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setSpeakerColor( iconColor );
}

//============================================================================
void ActivityBase::setCameraColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setCameraColor( iconColor );
}

//============================================================================
void ActivityBase::setTrashButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setTrashButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setShareButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setShareButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setTopMenuButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setTopMenuButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setBackButtonColor( QColor iconColor )
{
	ui.m_TitleBarWidget->setBackButtonColor( iconColor );
}

//============================================================================
//=== bottom bar icon color ===//
//============================================================================
void ActivityBase::setPlayProgressBarColor( QColor iconColor ) 
{
	ui.m_BottomBarWidget->setPlayProgressBarColor( iconColor );
}

//============================================================================
void ActivityBase::setArrowLeftButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setArrowLeftButtonColor( iconColor );
}

//============================================================================
void ActivityBase::set30SecBackwardButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->set30SecBackwardButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMediaPlayButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMediaPlayButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMediaTrashButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMediaTrashButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMediaFileShareButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMediaFileShareButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMediaLibraryButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMediaLibraryButtonColor( iconColor );
}

//============================================================================
void ActivityBase::set30SecForwardButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->set30SecForwardButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setArrowRightButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setArrowRightButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMediaRepeatButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMediaRepeatButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setMenuBottomButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setMenuBottomButtonColor( iconColor );
}

//============================================================================
void ActivityBase::setExpandWindowButtonColor( QColor iconColor )
{
	ui.m_BottomBarWidget->setExpandWindowButtonColor( iconColor );
}

//============================================================================
//=== title bar slots ====//
//============================================================================
void ActivityBase::slotPowerButtonClicked( void )
{
	emit signalPowerButtonClicked();
}

//============================================================================
void ActivityBase::slotHomeButtonClicked( void )
{
	emit signalHomeButtonClicked();
}

//============================================================================
void ActivityBase::slotMuteMicButtonClicked( bool muteMic )
{
    emit signalMuteMicButtonClicked( muteMic );
}

//============================================================================
void ActivityBase::slotMuteSpeakerButtonClicked( bool muteSpeaker )
{
	emit signalMuteSpeakerButtonClicked(muteSpeaker);
}

//============================================================================
void ActivityBase::slotCameraSnapshotButtonClicked( void )
{
	emit signalCameraSnapshotButtonClicked();
}

//============================================================================
void ActivityBase::slotCamPreviewClicked( void )
{
	emit signalCamPreviewClicked();
}

//============================================================================
void ActivityBase::slotTrashButtonClicked( void )
{
	emit signalTrashButtonClicked();
}

//============================================================================
void ActivityBase::slotShareButtonClicked( void )
{
	emit signalShareButtonClicked();
}

//============================================================================
void ActivityBase::slotMenuTopButtonClicked( void )
{
	emit signalMenuTopButtonClicked();
}

//============================================================================
void ActivityBase::slotBackButtonClicked( void )
{
	onBackButtonClicked();
}

// override default behavior of closing dialog when back button is clicked
//============================================================================
void ActivityBase::onBackButtonClicked( void )
{
    emit signalBackButtonClicked();
    m_MyApp.activityStateChange( this, false );
    close();
}

//=== bottom bar slots ====//
//============================================================================
void ActivityBase::slotArrowLeftButtonClicked( void )
{
	emit signalArrowLeftButtonClicked();
}

//============================================================================
void ActivityBase::slot30SecBackwardButtonClicked( void )
{
	emit signal30SecBackwardButtonClicked();
}

//============================================================================
void ActivityBase::slotMediaPlayButtonClicked( void )
{
	emit signalMediaPlayButtonClicked();
}

//============================================================================
void ActivityBase::slotMediaTrashButtonClicked( void )
{
	emit signalMediaTrashButtonClicked();
}

//============================================================================
void ActivityBase::slot30SecForwardButtonClicked( void )
{
	emit signal30SecForwardButtonClicked();
}

//============================================================================
void ActivityBase::slotMediaFileShareClicked( void )
{
	emit signalMediaFileShareClicked();
}

//============================================================================
void ActivityBase::slotMediaLibraryButtonClicked( void )
{
	emit signalMediaLibraryButtonClicked();
}

//============================================================================
void ActivityBase::slotArrowRightButtonClicked( void )
{
	emit signalArrowRightButtonClicked();
}

//============================================================================
void ActivityBase::slotMediaRepeatButtonClicked( void )
{
	emit signalMediaRepeatButtonClicked();
}

//============================================================================
void ActivityBase::slotMenuBottomButtonClicked( void )
{
	emit signalMenuBottomButtonClicked();
}

//============================================================================
void ActivityBase::slotResizeWindowTimeout()
{
	if( m_IsResizing )
	{
		m_IsResizing = false;
		onResizeEnd( m_ResizingWindowSize );
	}
}

//============================================================================
void ActivityBase::fillMyNodeUrl( QLabel * myUrlLabel )
{
    if( myUrlLabel )
    {
        std::string url;
        m_MyApp.getEngine().fromGuiGetNodeUrl( url );
        if( !url.empty() )
        {
            myUrlLabel->setText( QString( url.c_str() ) );
        }
    }
}

//============================================================================
void ActivityBase::setTitleBarAppletIcon( EMyIcons appletIcon )
{
    TitleBarWidget * titleBar = getTitleBarWidget();
    if( titleBar )
    {
        VxPushButton * titleButton = titleBar->getAppIconPushButton();
        if( titleButton )
        {
            titleButton->setAppIcon( appletIcon, this );
            disconnect( titleButton, SIGNAL( signalAppIconSpecialClick() ), this, SLOT( slotAppIconSpecialClick() ) );
            connect( titleButton, SIGNAL( signalAppIconSpecialClick() ), this, SLOT( slotAppIconSpecialClick() ) );
        }
    }
}

//============================================================================
void ActivityBase::slotAppIconSpecialClick( void )
{
    onAppIconSpecialClick( this );
}

//============================================================================
void ActivityBase::onAppIconSpecialClick( ActivityBase * activityBase )
{
    LogMsg( LOG_DEBUG, "onAppIconSpecialClick" );
}
