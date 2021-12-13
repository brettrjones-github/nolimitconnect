//============================================================================
// Copyright (C) 2018 Brett R. Jones
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
#include "AppletKodi.h"
//#include "KodiRun.h"

#include "ActivityBrowseFiles.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppCommon.h"
#include "RenderKodiThread.h"


#include <GuiInterface/IGoTv.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

#include <QThread>

//namespace
//{

//    KodiThread *    g_KodiThread = nullptr;
//    KodiRun *       g_RunKodi = nullptr;
//    bool            g_IsKodiThreadRunning = false;

//    bool IsKodiThreadRunning()
//    {
//        return g_IsKodiThreadRunning;
//    }

//    void RunKodiFromThread( )
//    {
//        if( g_RunKodi )
//        {

//            g_KodiThread = new KodiThread();
//            g_RunKodi->moveToThread( g_KodiThread );
//            GetAppInstance().setKodiThread( g_KodiThread );
//            QObject::connect( g_KodiThread, SIGNAL( started() ), g_RunKodi, SLOT( slotStartKodiRunFromThread() ) );
//        }
//    }

//    void CleanupOldKodiThread()
//    {

//    }

//}


//============================================================================
AppletKodi::AppletKodi( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_KODI, app, parent )
//, m_ActivityCallbacksEnabled( false )
//, m_IsPlaying( false )
//, m_SliderIsPressed( false )
{
	initAppletKodi();
}

//============================================================================
void AppletKodi::initAppletKodi( void )
{
    setAppletType( eAppletKodi );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );

	ui.setupUi( getContentItemsFrame() );
    setMenuBottomVisibility( true );

    setTitleBarAppletIcon( eMyIconKodi );
 
//    startKodiModule();

    //ui.m_RenderGlWidget->setSource( new )

	//ui.m_VidWidget->showAllControls( false );
	//ui.m_VidWidget->disablePreview( true );
	//ui.m_VidWidget->disableRecordControls( true );

	//ui.m_PlayPosSlider->setRange( 0, 100000 );

	////connect( &m_MyApp,				SIGNAL(signalAssetAction(EAssetAction, VxGUID, int)), this, SLOT(slotToGuiAssetAction(EAssetAction, VxGUID, int)) );
	//connect( ui.m_PlayPosSlider, SIGNAL( sliderPressed() ), this, SLOT( slotSliderPressed() ) );
	//connect( ui.m_PlayPosSlider, SIGNAL( sliderReleased() ), this, SLOT( slotSliderReleased() ) );

	//connect( this, SIGNAL( signalPlayProgress( int ) ), this, SLOT( slotPlayProgress( int ) ) );
	//connect( this, SIGNAL( signalPlayEnd() ), this, SLOT( slotPlayEnd() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
// called just before first show of applet.. override for special initialization needs
void AppletKodi::aboutToLaunchApplet( void )
{
    //startKodiModule();
}

//============================================================================
void AppletKodi::onAppletStop( void )
{
    // should kodi be stopped or let it run simultaneously
    //stopKodiModule()

    //setReadyForCallbacks( false );
    //stopMediaIfPlaying();
}

//============================================================================
//void AppletKodi::startKodiModule( void )
//{
//    // call of doRun of kodi never returns from function until kodi is shutdown so use thread
//    IGoTv& iGoTv =  getMyApp().getGoTv();
//    if( !iGoTv.getIsAppModuleRunning( eAppModuleKodi ) && !VxIsAppShuttingDown() && !IsKodiThreadRunning() )
//    {
//        CleanupOldKodiThread();
//        g_RunKodi = new KodiRun( getMyApp(), nullptr );
//        RunKodiFromThread();
//    }
//}

////============================================================================
//void AppletKodi::stopKodiModule( void )
//{

//}

//============================================================================
void AppletKodi::onResizeBegin( QSize& newSize )
{
	//getMyApp().getGoTv().fromGuiResizeBegin( newSize.width(), newSize.height() );
}

//============================================================================
void AppletKodi::onResizeEvent( QSize& newSize )
{
	//getMyApp().getGoTv().fromGuiResizeEvent( newSize.width(), newSize.height() );
}

//============================================================================
void AppletKodi::onResizeEnd( QSize& newSize )
{
	//getMyApp().getGoTv().fromGuiResizeEnd( newSize.width(), newSize.height() );
}

//============================================================================
RenderGlWidget *  AppletKodi::getRenderConsumer( void )
{
    return ui.m_RenderWidget;
}

//============================================================================
void AppletKodi::setupBottomMenu( VxMenuButton * menuButton )
{
    if( menuButton )
    {
        menuButton->setMenuId( 1 );
        menuButton->addMenuItem( eMenuItemShowRecent );
        menuButton->addMenuItem( eMenuItemShowWatched );
        menuButton->addMenuItem( eMenuItemShowLibrary );
        menuButton->addMenuItem( eMenuItemBrowse );
    }

    connect( menuButton, SIGNAL( signalMenuItemSelected( int, EMenuItemType ) ), 
                this, SLOT( slotMenuItemSelected( int, EMenuItemType ) ) );
}


//============================================================================
void AppletKodi::slotMenuItemSelected( int menuId, EMenuItemType menuItemType )
{
    ActivityBrowseFiles * dlgBrowse;
    switch( menuItemType )
    {
    case eMenuItemBrowse:
        dlgBrowse = new ActivityBrowseFiles( m_MyApp, eFileFilterVideoOnly, this, true );
        dlgBrowse->setAppletType( getAppletType() );
        dlgBrowse->exec();
        if( dlgBrowse->getWasFileSelected() )
        {
            QString fileName = dlgBrowse->getSelectedFileName();
            playFile( fileName );
        }

        break;

    default:
        break;
    }
}


//============================================================================
void AppletKodi::showEvent( QShowEvent * showEvent )
{
    AppletBase::showEvent( showEvent );
    if( ( false == VxIsAppShuttingDown() )
        && !m_IsPlaying )
    {
        m_MyApp.wantToGuiActivityCallbacks( this, this, true );
        setReadyForCallbacks( true );
        if( m_AssetInfo.isValid() )
        {
            m_Engine.fromGuiAssetAction( eAssetActionPlayOneFrame, m_AssetInfo, 0 );
        }
    }

//    static bool g_firstShow = false;
//    if( !g_firstShow )
//    {
//        g_firstShow = true;
//        //QString videoFile = "F:/TestMedia/video_test/test_Tarzan2016_01_18_01-22-44.avi";
//        QString videoFile = "F:/TestMedia/video_test/Agents.of.S.H.I.E.L.D.S05E09/Marvels.Agents.of.S.H.I.E.L.D.S05E09.WEBRip.x264-ION10.mp4";
//        //QString videoFile = "F:/TestMedia/video_test/Test.avi";
//        //QString videoFile = "F:/TestMedia/video_test/PleaseStandByAC3-EVO.avi";
//        playFile( videoFile );
//    }
}

//============================================================================
void AppletKodi::hideEvent( QHideEvent * hideEvent )
{
    AppletBase::hideEvent( hideEvent );
    setReadyForCallbacks( false );
    m_MyApp.wantToGuiActivityCallbacks( this, this, false );
}

//============================================================================
void AppletKodi::resizeEvent( QResizeEvent * ev )
{
    AppletBase::resizeEvent( ev );
    if( ( false == VxIsAppShuttingDown() )
        && m_AssetInfo.isValid()
        && !m_IsPlaying
        && isVisible() )
    {
        setReadyForCallbacks( true );
        //m_Engine.fromGuiAssetAction( eAssetActionPlayOneFrame, m_AssetInfo, 0 );
    }
}

//============================================================================
void AppletKodi::setReadyForCallbacks( bool isReady )
{
	if( m_ActivityCallbacksEnabled != isReady )
	{
		m_ActivityCallbacksEnabled = isReady;
		//m_MyApp.wantToGuiActivityCallbacks( this, this, isReady );
	}
}



#if 0

//============================================================================
void AppletKodi::setAssetInfo( AssetInfo& assetInfo )
{
	AppletBase::setAssetInfo( assetInfo );
	//ui.m_FileNameLabel->setText( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_ShredButton->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_LeftAvatarBar->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_RightAvatarBar->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );

	ui.m_VidWidget->setVideoFeedId( m_AssetInfo.getAssetUniqueId() );
	//if( ui.m_TagLabel->text().isEmpty() )
	//{
	//	ui.m_TagLabel->setVisible( false );
	//	ui.m_TagTitleLabel->setVisible( false );
	//	this->setSizeHint( QSize( 100, 224 - 16 ) );
	//}
	//else
	//{
	//	ui.m_TagLabel->setVisible( true );
	//	ui.m_TagTitleLabel->setVisible( true );
	//	this->setSizeHint( QSize( 100, 224 ) );
	//}

	//if( assetInfo.isMine() )
	//{
	//	ui.m_LeftAvatarBar->setTime( m_AssetInfo.getCreationTime() );
	//}
	//else
	//{
	//	ui.m_RightAvatarBar->setTime( m_AssetInfo.getCreationTime() );
	//}

	//if( assetInfo.isFileAsset() )
	//{
	//	ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
	//	ui.m_RightAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
	//}
	//else
	//{
	//	ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconTrash );
	//	ui.m_RightAvatarBar->setShredButtonIcon( eMyIconTrash );
	//}

	//updateFromAssetInfo();
}

////============================================================================
//void AppletKodi::slotToGuiAssetAction( EAssetAction assetAction, int pos0to100000 )
//{
//	AppletPlayerBase::slotToGuiAssetAction( assetAction, pos0to100000 );
//	switch( assetAction )
//	{
//	case eAssetActionPlayProgress:
//		if( false == m_SliderIsPressed )
//		{
//			updateGuiPlayControls( true );
//			ui.m_PlayPosSlider->setValue( pos0to100000 );
//		}
//
//		break;
//
//	case eAssetActionPlayEnd:
//		if( false == m_SliderIsPressed )
//		{
//			updateGuiPlayControls( false );
//		}
//
//		break;
//
//	default:
//		break;
//	}
//}
//
////============================================================================
//void AppletKodi::slotSliderPressed( void )
//{
//	m_SliderIsPressed = true;
//}
//
////============================================================================
//void AppletKodi::slotSliderReleased( void )
//{
//	m_SliderIsPressed = false;
//	int posVal = ui.m_PlayPosSlider->value();
//	startMediaPlay( posVal );
//}
//
////============================================================================
//void AppletKodi::slotPlayButtonClicked( void )
//{
//	if( m_IsPlaying )
//	{
//		stopMediaIfPlaying();
//	}
//	else
//	{
//		startMediaPlay( 0 );
//	}
//}
//
////========================================================================
//void AppletKodi::startMediaPlay( int startPos )
//{
//	bool playStarted = m_Engine.fromGuiAssetAction( eAssetActionPlayBegin, m_AssetInfo, startPos );
//	updateGuiPlayControls( playStarted );
//	if( false == playStarted )
//	{
//		m_MyApp.toGuiStatusMessage( "Video Play FAILED TO Begin" );
//	}
//}
//
////========================================================================
//void AppletKodi::updateGuiPlayControls( bool isPlaying )
//{
//	if( m_IsPlaying != isPlaying )
//	{
//		m_IsPlaying = isPlaying;
//		if( m_IsPlaying )
//		{
//			// start playing
//			//ui.m_PlayPauseButton->setIcons( eMyIconPauseNormal );
//			setReadyForCallbacks( true );
//		}
//		else
//		{
//			// stop playing
//			//ui.m_PlayPauseButton->setIcons( eMyIconPlayNormal );
//			ui.m_PlayPosSlider->setValue( 0 );
//		}
//	}
//}

////============================================================================
//void AppletKodi::stopMediaIfPlaying( void )
//{
//	if( m_IsPlaying )
//	{
//		m_MyApp.toGuiStatusMessage( "" );
//		m_Engine.fromGuiAssetAction( eAssetActionPlayEnd, m_AssetInfo, 0 );
//	}
//
//	updateGuiPlayControls( false );
//}
//
////============================================================================
//void AppletKodi::slotShredAsset( void )
//{
//	onAppletStop();
//	//emit signalShreddingAsset( this );
//}
//
////============================================================================
//void AppletKodi::toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgLen, int motion0To100000 )
//{
//	Q_UNUSED( userData );
//	ui.m_VidWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgLen, motion0To100000 );
//}
//
////============================================================================
//int AppletKodi::toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
//{
//    Q_UNUSED( userData );
//    return ui.m_VidWidget->playVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
//}
//
////============================================================================
//void AppletKodi::slotPlayProgress( int pos0to100000 )
//{
//	if( m_IsPlaying && ( false == m_SliderIsPressed ) )
//	{
//		ui.m_PlayPosSlider->setValue( pos0to100000 );
//	}
//}
//
////============================================================================
//void AppletKodi::slotPlayEnd( void )
//{
//	//updateGuiPlayControls( false );
//}
//
////============================================================================
//void AppletKodi::showShredder( bool show )
//{
//	//if( m_AssetInfo.isMine() )
//	//{
//	//	ui.m_LeftAvatarBar->showShredder( show );
//	//}
//	//else
//	//{
//	//	ui.m_RightAvatarBar->showShredder( show );
//	//}
//}
//
////============================================================================
//void AppletKodi::showXferProgress( bool show )
//{
//	//if( m_AssetInfo.isMine() )
//	//{
//	//	ui.m_LeftAvatarBar->showXferProgress( show );
//	//}
//	//else
//	//{
//	//	ui.m_RightAvatarBar->showXferProgress( show );
//	//}
//}
//
////============================================================================
//void AppletKodi::setXferProgress( int xferProgress )
//{
//	//if( m_AssetInfo.isMine() )
//	//{
//	//	ui.m_LeftAvatarBar->setXferProgress( xferProgress );
//	//}
//	//else
//	//{
//	//	ui.m_RightAvatarBar->setXferProgress( xferProgress );
//	//}
//}

#endif // 0
