#include "AppCommon.h"
#include "AppSettings.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

namespace
{
    AppCommon * g_AppCommon = nullptr;
}

AppCommon& CreateAppCommonInstance( QApplication& myQApp )
{
    static AppSettings appSettings;
    if( !g_AppCommon )
    {
        // constructor of AppCommon will set g_AppCommon
        g_AppCommon = new AppCommon( myQApp, appSettings );
    }

    return *g_AppCommon;
}

//============================================================================
AppCommon& GetAppInstance( void )
{
    return *g_AppCommon;
}

//============================================================================
void DestroyAppCommonInstance()
{
    if( g_AppCommon )
    {
        delete g_AppCommon;
        g_AppCommon = nullptr;
    }
}

//============================================================================
AppCommon::AppCommon( QApplication& myQApp, AppSettings& appSettings )
    : m_QApp( myQApp )
    , m_AppSettings( appSettings )
    , m_MyIcons( *this )
    , m_AppTheme( *this )
    , m_CamLogic( *this )
    , m_Engine( GetP2PEngineInstance() )
{
    connect( this, SIGNAL( signalEnableVideoCapture( bool ) ), this, SLOT( slotEnableVideoCapture( bool ) ) );
}

//============================================================================
void AppCommon::toGuiWantVideoCapture( bool wantVidCapture )
{
    LogMsg( LOG_INFO, "#### AppCommon::slotEnableVideoCapture %d\n", wantVidCapture );
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalEnableVideoCapture( wantVidCapture );
}

//============================================================================
void AppCommon::slotEnableVideoCapture( bool enableVidCapture )
{
    m_CamLogic.toGuiWantVideoCapture( enableVidCapture );
//    if( enableVidCapture )
//    {
//        static bool bFirstTimeVideoCaptureStarted = true;
//        if( bFirstTimeVideoCaptureStarted )
//        {
//            if( NULL == m_VidCapTimer )
//            {
//                m_VidCapTimer = new QTimer( this );
//                m_VidCapTimer->setInterval( 50 ); // we want about 20 frames per second 1000/20 = 50ms
//                connect( m_VidCapTimer, SIGNAL( timeout() ), this, SLOT( onVidCapTimer() ) );
//#ifdef TARGET_OS_WINDOWS
//                m_VidCap = VxGetVidCapInterface();
//#endif // TARGET_OS_WINDOWS
//            }
//
//            uint32_t iDevices = m_VidCap->startupVidCap();
//            if( 0 == iDevices )
//            {
//                QMessageBox::warning( this, tr( "Web Cam Video" ), tr( "No Video Capture Devices Found" ) );
//                return;
//            }
//
//            m_CamSourceId = m_AppSettings.getCamSourceId();
//            if( m_CamSourceId > iDevices )
//            {
//                m_CamSourceId = 1;
//                m_AppSettings.setCamSourceId( m_CamSourceId );
//            }
//
//            setCamCaptureRotation( m_AppSettings.getCamRotation( m_CamSourceId ) );
//
//            bFirstTimeVideoCaptureStarted = false;
//        }
//
//        if( false == m_VidCapTimer->isActive() )
//        {
//            LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture start capture\n" );
//            m_VidCapTimer->start();
//        }
//    }
//    else
//    {
//        if( m_VidCapTimer &&
//            m_VidCapTimer->isActive() )
//        {
//            LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture stopping capture\n" );
//            m_VidCapTimer->stop();
//        }
//    }
//
//    m_VidCaptureEnabled = enableVidCapture;
//    std::vector<ToGuiHardwareCtrlClient>::iterator hardwareIter;
//    for( hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
//    {
//        ToGuiHardwareCtrlClient& toGuiClient = *hardwareIter;
//        toGuiClient.m_Callback->doGuiWantVideoCapture( enableVidCapture );
//    }
}


////============================================================================
////! time to send out new video frame
//void AppCommon::onVidCapTimer( void )
//{
//#ifdef TARGET_OS_WINDOWS
//    uint32_t u32ImageLen = 0;
//    uint32_t u32Format = 0;
//    uint8_t * pu8VidBmp = m_VidCap->takeSnapShot( 0, u32ImageLen, u32Format );
//    if( pu8VidBmp )
//    {
//        BITMAPINFO * poBitmap = ( BITMAPINFO * )pu8VidBmp;
//        if( ( 0 == u32Format ) &&
//            ( 0 != poBitmap->bmiHeader.biSizeImage ) &&
//            ( poBitmap->bmiHeader.biSizeImage == ( poBitmap->bmiHeader.biWidth * poBitmap->bmiHeader.biHeight * 3 ) ) )
//        {
//            // some cams do not place format in bitmap even though data is valid ie (Microsoft LifeCam VX-3000)
//            u32Format = FOURCC_RGB;
//        }
//
//        // send out video frame as bitmap
//        m_Engine.fromGuiVideoData( u32Format, // format
//            ( uint8_t * )&pu8VidBmp[ poBitmap->bmiHeader.biSize ],
//                                   poBitmap->bmiHeader.biWidth,
//                                   poBitmap->bmiHeader.biHeight,
//                                   poBitmap->bmiHeader.biSizeImage,
//                                   m_CamCaptureRotation );
//        delete pu8VidBmp;
//    }
//#endif // TARGET_OS_WINDOWS
//}

//============================================================================
void AppCommon::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 )
{
}

//============================================================================
int AppCommon::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    int result = 0;

    return result;
}
