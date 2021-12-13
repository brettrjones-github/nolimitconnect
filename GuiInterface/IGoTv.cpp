#include <CommonHdr.h>
#include "IGoTv.h"
#include <CoreLib/VxDebug.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>

#include "ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h"
#include "AppsSrc/CommonSrc/QtSource/AppCommon.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxStringUtils.h>

#include <NetLib/VxPeerMgr.h>

#ifdef TARGET_OS_WINDOWS
# include "GuiInterface/OsWin32/IWin32.h"
#elif TARGET_OS_LINUX
# include "GuiInterface/OsLinux/ILinux.h"
#elif TARGET_OS_ANDROID
# include "GuiInterface/OsAndroid/IAndroid.h"
# include "CoreLib/VxJava.h"
#if ENABLE_KODI
# include "kodi_src/xbmc/xbmc/platform/qt/qtandroid/jni/Context.h"
#endif // ENABLE_KODI
#else 
echo traget os is not defined
#endif 

#if ENABLE_KODI
#include <kodi_src/xbmc/xbmc/utils/StringUtils.h>
#include "Application.h"
#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "filesystem/File.h"
#include "platform/Environment.h"
#include "utils/log.h"

using namespace XFILE;
#endif // ENABLE_KODI

//============================================================================
VxPeerMgr& GetVxPeerMgr( void )
{
    static VxPeerMgr g_VxPeerMgr;
    return  g_VxPeerMgr;
}

//============================================================================
P2PEngine& GetPtoPEngine()
{
    static P2PEngine g_P2PEngine( GetVxPeerMgr() );
    return g_P2PEngine;
}

//============================================================================
P2PEngine& IGoTv::getPtoP()
{
    return GetPtoPEngine();
}

//============================================================================
IGoTv& IGoTv::getIGoTv()
{
    static IGoTv g_IGoTv;
    return g_IGoTv;
}

//============================================================================
IToGui& IToGui::getToGui()
{
    return IGoTv::getIGoTv();
}

//============================================================================
IAudioRequests& IToGui::getAudioRequests()
{
    return IGoTv::getIGoTv();
}

//============================================================================
AppCommon& IGoTv::getAppCommon()
{
    return GetAppInstance();
}

//============================================================================
void ff_flush_avutil_log_buffers(void)
{
}

//============================================================================
void ff_avutil_log(void* ptr, int level, const char* format, va_list va)
{
    static VxMutex logMutex;
    logMutex.lock();

    unsigned int threadId =	VxGetCurrentThreadId();
    AVClass* avc= ptr ? *(AVClass**)ptr : NULL;

    int logLevel = level;
    int logType = 0;
    switch( level )
    {
    case AV_LOG_FATAL:
    case AV_LOG_PANIC:
        //logLevel = ILOG_LEVEL_FATAL;
        logType = LOG_FATAL;
        break;

    case AV_LOG_ERROR:
        //logLevel = ILOG_LEVEL_ERROR;
        logType = LOG_ERROR;
        break;

    case AV_LOG_WARNING:
        //logLevel = ILOG_LEVEL_WARN;
        logType = LOG_WARN;
        break;

    case AV_LOG_INFO:
        //logLevel = ILOG_LEVEL_INFO;
        logType = LOG_INFO;
        break;

    case AV_LOG_DEBUG:
        //logLevel = LOG_LEVEL_DEBUG;
        logType = LOG_DEBUG;
        break;

    case AV_LOG_TRACE:
    case AV_LOG_VERBOSE:
        //logLevel = ILOG_LEVEL_VERBOSE;
        logType = LOG_VERBOSE;
        break;

    case AV_LOG_QUIET:
    default:
        //logLevel = LOG_LEVEL_NONE;
        logMutex.unlock();
        return;
    }

    if( logLevel > GetILog().getFfmpegLogLevel() )
    {
        logMutex.unlock();
        return;
    }

    if( level > AV_LOG_DEBUG )
    {
        logMutex.unlock();
        return;
    }

    std::string message = VxStringUtils::FormatV( format, va );
    if( message.empty() )
    {
        logMutex.unlock();
        return;
    }

    static std::string buffer;
    std::string prefix = VxStringUtils::Format( "ffmpeg[%lX]: ", threadId );
    if( avc )
    {
        if( avc->item_name )
            prefix += std::string( "[" ) + avc->item_name( ptr ) + "] ";
        else if( avc->class_name )
            prefix += std::string( "[" ) + avc->class_name + "] ";
    }

    buffer += message;
    int pos, start = 0;
    while( buffer.length() && ( pos = (int)buffer.find_first_of( '\n', start ) ) >= 0 )
    {
        if( pos > start )
        {
            std::string msgStr = buffer.substr( start, pos - start ).c_str();
            if( buffer.length() && msgStr.length() )
            {
                LogMsg( logType, "%s%s", prefix.c_str(), msgStr.c_str() );
            }
        }

        start = pos + 1;
    }

    buffer.erase( 0, start );
    logMutex.unlock();
}

//============================================================================
IGoTv::IGoTv()
: m_ILog()
#ifdef TARGET_OS_WINDOWS
, m_OsInterface( *new IWin32(*this) )
#elif TARGET_OS_LINUX
, m_OsInterface( *new ILinux( *this ) )
#elif TARGET_OS_ANDROID
, m_OsInterface( *new IAndroid( *this ) )
#endif 
, m_VxPeerMgr( GetVxPeerMgr() )
#if ENABLE_KODI
, m_Kodi( GetKodiInstance() )
#endif // ENABLE_KODI
{
    memset( m_IsRunning, 0, sizeof( m_IsRunning  ) );
}

//============================================================================
IGoTv::~IGoTv()
{
    delete &m_OsInterface;
}

//=== utilities ===//
//============================================================================
bool IGoTv::initDirectories()
{
    return m_OsInterface.initDirectories();
}

//============================================================================
void IGoTv::createUserDirs() const
{
#if ENABLE_KODI
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::createUserDirs");
    CDirectory::Create( "special://home/" );
    CDirectory::Create( "special://home/addons" );
    CDirectory::Create( "special://home/addons/packages" );
    CDirectory::Create( "special://home/addons/temp" );
    CDirectory::Create( "special://home/media" );
    CDirectory::Create( "special://home/system" );
    CDirectory::Create( "special://masterprofile/" );
    CDirectory::Create( "special://temp/" );
    CDirectory::Create( "special://logpath" );
    CDirectory::Create( "special://temp/temp" ); // temp directory for python and dllGetTempPathA


      //Let's clear our archive cache before starting up anything more
    auto archiveCachePath = CSpecialProtocol::TranslatePath( "special://temp/archive_cache/" );
    if( CDirectory::Exists( archiveCachePath ) )
        if( !CDirectory::RemoveRecursive( archiveCachePath ) )
            CLog::Log( LOGWARNING, "Failed to remove the archive cache at %s", archiveCachePath.c_str() );
    CDirectory::Create( archiveCachePath );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::playGoTvMedia( AssetBaseInfo * assetInfo )
{ 
};

//=== ffmpeg ===//
//============================================================================
void IGoTv::startupFfmpeg()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::startupFfmpeg");
    static bool ffmpegStarted = false;
    if( !ffmpegStarted )
    {
        ffmpegStarted = true;
        // register ffmpeg lockmanager callback
    //    av_lockmgr_register( &ffmpeg_lockmgr_cb );

        // set avutil callback
        av_log_set_callback( ff_avutil_log );
        // register avcodec
        avcodec_register_all();
#if CONFIG_AVDEVICE
        avdevice_register_all();
#endif
        // register avformat
        av_register_all();
        // register avfilter
        avfilter_register_all();
        // initialize network protocols
        avformat_network_init();

    }
}

//============================================================================
void IGoTv::shutdownFfmpeg()
{

}

//=== open ssl ===//
//============================================================================
void IGoTv::setSslCertFile( std::string certFile )
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::setSslCertFile (%s)", certFile.c_str());
    m_SslCertFile = certFile;
    if( !m_SslCertFile.empty() )
    {
        if( VxFileUtil::fileExists( certFile.c_str() ) )
        {
            setenv( "SSL_CERT_FILE", certFile.c_str(), 1 );
        }

        // TODO: anything special for linux?
    }
}

//=== stages of create ===//
//============================================================================
bool IGoTv::doPreStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::doPreStartup");
#ifdef TARGET_OS_ANDROID
# if ENABLE_KODI
    CJNIContext::createJniContext( GetJavaEnvCache().getJavaVM(),  GetJavaEnvCache().getJavaEnv() );
# endif // ENABLE_KODI
#endif // TARGET_OS_ANDROID

#ifdef DEBUG_KODI_ENABLE_DEBUG_LOGGING
    CLog::SetLogLevel( LOG_LEVEL_DEBUG );
#endif //   DEBUG_KODI_ENABLE_DEBUG_LOGGING
    // const int FFMPEG_LOG_LEVEL_DEBUG = 1; // shows all
    const int FFMPEG_LOG_LEVEL_NORMAL = 0; // shows notice, error, severe and fatal
#ifdef DEBUG_FFMPEG_ENABLE_LOGGING
    getILog().setFfmpegLogLevel( FFMPEG_LOG_LEVEL_DEBUG );
#else
    getILog().setFfmpegLogLevel( FFMPEG_LOG_LEVEL_NORMAL );
#endif

    bool result = m_OsInterface.doPreStartup();
	result &= m_OsInterface.initUserPaths();
	return result;
}

//============================================================================
bool IGoTv::doStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::doStartup");
    return m_OsInterface.doStartup();
}


//=== stages of run ===//
//============================================================================
bool IGoTv::initRun( const CAppParamParser& params )
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::initRun");
    return m_OsInterface.initRun( params );
}

//============================================================================
bool IGoTv::doRun( EAppModule appModule )
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::doRun");
    return m_OsInterface.doRun( appModule );
}

//=== stages of destroy ===//
//============================================================================
void IGoTv::doPreShutdown()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::doPreShutdown");
    m_OsInterface.doPreShutdown();
}

//============================================================================
void IGoTv::doShutdown()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IGoTv::doShutdown");
    m_OsInterface.doShutdown();
}

//============================================================================
//=== to gotv events ===//
//============================================================================
//============================================================================
void IGoTv::fromGuiKeyPressEvent( int moduleNum, int key, int mod )
{
#if ENABLE_KODI
    getKodi().fromGuiKeyPressEvent( moduleNum, key, mod );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiKeyReleaseEvent( int moduleNum, int key, int mod )
{
#if ENABLE_KODI
    getKodi().fromGuiKeyReleaseEvent( moduleNum, key, mod );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiMousePressEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
#if ENABLE_KODI
    getKodi().fromGuiMousePressEvent( moduleNum, mouseXPos, mouseYPos, mouseButton );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiMouseReleaseEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
#if ENABLE_KODI
    getKodi().fromGuiMouseReleaseEvent( moduleNum, mouseXPos, mouseYPos, mouseButton );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiMouseMoveEvent( int moduleNum, int mouseXPos, int mouseYPos )
{
#if ENABLE_KODI
    getKodi().fromGuiMouseMoveEvent( moduleNum, mouseXPos, mouseYPos );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiResizeBegin( int moduleNum, int winWidth, int winHeight )
{
#if ENABLE_KODI
	getKodi().fromGuiResizeBegin( moduleNum, winWidth, winHeight );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiResizeEvent( int moduleNum, int winWidth, int winHeight )
{
#if ENABLE_KODI
    getKodi().fromGuiResizeEvent( moduleNum, winWidth, winHeight );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiResizeEnd( int moduleNum, int winWidth, int winHeight )
{
#if ENABLE_KODI
	getKodi().fromGuiResizeEnd( moduleNum, winWidth, winHeight );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiCloseEvent( int moduleNum )
{
#if ENABLE_KODI
    getKodi().fromGuiCloseEvent( moduleNum );
#endif // ENABLE_KODI
}

//============================================================================
void IGoTv::fromGuiVisibleEvent( int moduleNum, bool isVisible )
{
#if ENABLE_KODI
    getKodi().fromGuiVisibleEvent( moduleNum, isVisible );
#endif // ENABLE_KODI
}
