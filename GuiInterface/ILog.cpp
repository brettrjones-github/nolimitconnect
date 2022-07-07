#include <QPushButton>

#include "ILog.h"
#include "IGoTv.h"
#include <CoreLib/VxDebug.h>

//============================================================================
ILog::ILog()
{
}

//============================================================================
// call these first to set all log levels
void ILog::setAllLogLevel( int level )
{
    setCoreLogLevel( level );
    setKodiLogLevel( level );
    setFfmpegLogLevel( level );
}

//============================================================================
void ILog::clearAllLogLevel()
{
    setCoreLogLevel( 0 );
    setKodiLogLevel( 0 );
    setFfmpegLogLevel( 0 );
}

//============================================================================
// adjust log levels of different modules after the "all" level is set
// this level is overall end log output.. levels of others higher than this will still be filtered out
void ILog::setCoreLogLevel( int level )
{
    m_CoreLogLevel = level;
    switch( level )
    {
    case ILOG_LEVEL_INFO:
        VxSetLogPriorityMask( LOG_STATUS | LOG_INFO | LOG_DEBUG | LOG_WARN | LOG_ERROR | LOG_ASSERT | LOG_SEVERE | LOG_FATAL );
        break;
    case ILOG_LEVEL_FATAL:
        VxSetLogPriorityMask( LOG_FATAL | LOG_INFO );
        break;
    case ILOG_LEVEL_ASSERT:
        VxSetLogPriorityMask( LOG_ASSERT | LOG_FATAL | LOG_INFO );
        break;
    case ILOG_LEVEL_SEVERE:
        VxSetLogPriorityMask( LOG_ASSERT | LOG_SEVERE | LOG_FATAL | LOG_INFO );
        break;
    case ILOG_LEVEL_ERROR:
        VxSetLogPriorityMask( LOG_ERROR | LOG_ASSERT | LOG_SEVERE | LOG_FATAL | LOG_INFO );
        break;
    case ILOG_LEVEL_WARN:
        VxSetLogPriorityMask( LOG_WARN | LOG_ERROR | LOG_ASSERT | LOG_SEVERE | LOG_FATAL | LOG_INFO );
        break;
    case ILOG_LEVEL_DEBUG:
        VxSetLogPriorityMask( LOG_DEBUG | LOG_ERROR | LOG_WARN | LOG_ERROR | LOG_ASSERT | LOG_SEVERE | LOG_FATAL );
        break;
    case ILOG_LEVEL_VERBOSE:
        VxSetLogPriorityMask( LOG_VERBOSE | LOG_STATUS | LOG_INFO | LOG_DEBUG | LOG_WARN | LOG_ERROR | LOG_ASSERT | LOG_SEVERE | LOG_FATAL );
        break;
    default:
        VxSetLogPriorityMask( 0 );
        break;
    }
}

//============================================================================
void ILog::setKodiLogLevel( int level )
{
    m_kodiLogLevel = level;
}

//============================================================================
void ILog::clearKodiLogLevel()
{
    setKodiLogLevel( 0 );
}

//============================================================================
void ILog::setFfmpegLogLevel( int level )
{
    m_ffmpegLogLevel = level;
}

//============================================================================
void ILog::clearFfmpegLogLevel()
{
    setFfmpegLogLevel( 0 );
}
