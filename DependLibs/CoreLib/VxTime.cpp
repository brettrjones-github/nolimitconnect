//============================================================================
// Created by Brett R. Jones in 2018 and issued to public domain
//============================================================================
#include "config_corelib.h"
#include "VxTime.h"
#include "VxTimer.h"
#include "VxTimeUtil.h"
#include "VxDebug.h"

#include <sysheaders/sys/time.h>

#include <cstdlib>
#include <algorithm>

#if defined( TARGET_OS_ANDROID )
#include "VxDefs.h"

#include <time.h>
// android doesn't have millisecond sleep like most linux distributions but does have nano sleep
int VxSleep( int iMilliSec )
{
    struct timespec ts;
    ts.tv_sec = iMilliSec / 1000;
    ts.tv_nsec = ( iMilliSec % 1000 ) * 1000000;
    nanosleep( &ts, NULL );
    return 0;
};
#elif defined(TARGET_OS_WINDOWS)
int VxSleep( int milliSec ) 
{ 
    Sleep( milliSec ); 
    return 0; 
} // microsoft sleep returns void so had to make a function to return no error
#elif defined(TARGET_OS_LINUX)
#include <unistd.h>
int VxSleep( int milliSec )
{
    usleep( milliSec * 1000 );
    return 0;
}
#endif // TARGET_OS_ANDROID

namespace
{
    // initial variables set at startup to avoid constant recalculation
    // all variables are in milliseconds
    bool        g_isTimeInitialized = false;
    bool        g_UseMilitaryTime = false;

    int64_t     g_localTimezoneDifferenceMs = 0;

    int64_t     g_tickCountWhenAppInitialized = 0;
    int64_t     g_tickCountOffsetMsFromGmtTime = 0;
    int64_t     g_tickCountOffsetMsFromLocalTime = 0;
}

static void InitializeTimeIfNeeded();

//============================================================================
// set time format to military 24hr or AM/PM
void SetUseMillitaryTime( bool useMilitaryTime )
{
    g_UseMilitaryTime = useMilitaryTime;
}

//============================================================================
bool GetUseMillitaryTime()
{
    return g_UseMilitaryTime;
}

//============================================================================
int64_t	GetTimeStampMs( void )			      // milli seconds since January 1, 1970 GMT time
{
    return GetGmtTimeMs();
}

//============================================================================
// milliseconds since application initialize
int	GetApplicationAliveMs( void )
{
    InitializeTimeIfNeeded();
    int64_t tickCnt = (int64_t)GetHighResolutionTimeMs() - g_tickCountWhenAppInitialized;
    static int64_t lastTime = 0;
    if( tickCnt < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR MONOTONIC GetApplicationAliveMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, tickCnt);
    }

    lastTime = tickCnt;
    return (int)tickCnt;
}

//============================================================================
// milli seconds since January 1, 1970 GMT time
int64_t	GetGmtTimeMs( void )
{
    InitializeTimeIfNeeded();
    int64_t tickCnt = (int64_t)GetHighResolutionTimeMs() + g_tickCountOffsetMsFromGmtTime;
    static int64_t lastTime = 0;
    if( tickCnt < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR MONOTONIC GetGmtTimeMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, tickCnt);
    }

    lastTime = tickCnt;
    return tickCnt;
}

//============================================================================
// milli seconds since January 1, 1970 local time
int64_t	GetLocalTimeMs( void )
{
    InitializeTimeIfNeeded();
    return GetHighResolutionTimeMs() + g_tickCountOffsetMsFromLocalTime;
}

//============================================================================
// convert GMT time to local time
int64_t	GmtTimeMsToLocalTimeMs( int64_t gmtTime )
{
    InitializeTimeIfNeeded();
    return gmtTime + g_localTimezoneDifferenceMs;
}

//============================================================================
// time difference in milliseconds between GMT and local
int64_t	LocalTimeZoneDifferenceMs( void )
{
    return g_localTimezoneDifferenceMs;
}

//============================================================================
static void InitializeTimeIfNeeded( )
{
    if( g_isTimeInitialized )
    {
        return;
    }

    g_tickCountWhenAppInitialized = GetTickCount64();

    // get gmt time in milliseconds since since January 1, 1970 GMT time
    
    time_t rawtime;
    time( &rawtime );

    struct tm * gmtTimeVal = gmtime( &rawtime );
    int64_t gmtTimeMs = ( ( int64_t )mktime( gmtTimeVal ) * 1000 );

    struct tm * lclTimeVal = localtime( &rawtime );
    int64_t localTimeMs = ( ( int64_t )mktime( lclTimeVal ) * 1000 ); // only second resolution but does not matter as an offset

    g_localTimezoneDifferenceMs = gmtTimeMs - localTimeMs;
    g_tickCountOffsetMsFromGmtTime = gmtTimeMs - g_tickCountWhenAppInitialized;
    g_tickCountOffsetMsFromLocalTime = localTimeMs - g_tickCountWhenAppInitialized;
    g_isTimeInitialized = true;
}

// this class forces the time variables to be initialized as soon as possible
// there is a tiny chance of threading issues while being initialized but I think it
// worth the risk to not have the overhead of mutex locking
class InitTime
{
public:
    InitTime()
    {
        InitializeTimeIfNeeded( );
    }
};

static InitTime g_InitTime;

int32_t              TimeElapsedSec( int64_t startTimeMs, int64_t endTimeMs ) { return (int32_t)( std::abs( endTimeMs - startTimeMs ) / 1000 ); };
int64_t              TimeElapsedMs( int64_t startTimeMs, int64_t endTimeMs ) { return ( std::abs( endTimeMs - startTimeMs ) ); };

int32_t              TimeElapsedGmtSec( int64_t startTimeGmtMs )   { return (int32_t)( std::abs( GetGmtTimeMs() - startTimeGmtMs ) / 1000 ); };
int64_t              TimeElapsedGmtMs( int64_t startTimeGmtMs )    { return std::abs( GetGmtTimeMs() - startTimeGmtMs ); };

//============================================================================
TimeWithZone::TimeWithZone()
: m_TimeGmtMs( 0 )
, m_ZoneOffsMs( 0 )
{
}

TimeWithZone::TimeWithZone( bool initWithCurrentTime ) // will populate current time and zone
: m_TimeGmtMs( GetGmtTimeMs() )
, m_ZoneOffsMs( LocalTimeZoneDifferenceMs() )
{
}

//============================================================================
TimeWithZone::TimeWithZone( int64_t timeGmtMs ) // will populate given GMT time and our current time zone in ms
: m_TimeGmtMs( timeGmtMs )
, m_ZoneOffsMs( LocalTimeZoneDifferenceMs() )
{
}

//============================================================================
TimeWithZone::TimeWithZone( int64_t timeGmtMs, int32_t timeZoneMs )
: m_TimeGmtMs( timeGmtMs )
, m_ZoneOffsMs( timeZoneMs )
{
}

//============================================================================
std::string TimeWithZone::getFileNameCompatibleDate( bool localTime )
{
    return VxTimeUtil::getFileNameCompatibleDate( localTime ? m_TimeGmtMs + m_ZoneOffsMs : m_ZoneOffsMs );
}

//============================================================================
std::string TimeWithZone::getFileNameCompatibleDateAndTime( bool localTime )
{
    return VxTimeUtil::getFileNameCompatibleDate( localTime ? m_TimeGmtMs + m_ZoneOffsMs : m_ZoneOffsMs );
}

//============================================================================
std::string TimeWithZone::getLocalTime(  )
{
    return VxTimeUtil::formatTimeStampIntoHoursAndMinutes( m_TimeGmtMs, GetUseMillitaryTime() );
}

//============================================================================
std::string TimeWithZone::getGmtTime()
{
    return VxTimeUtil::formatGmtTimeStampIntoHoursAndMinutes( m_TimeGmtMs, GetUseMillitaryTime() );
}

//============================================================================
std::string TimeWithZone::getChatHourMinTimeStamp( bool localTime )
{
    if( localTime )
    {
        return VxTimeUtil::formatTimeStampIntoHoursAndMinutes( m_TimeGmtMs, GetUseMillitaryTime() );
    }
    else
    {
        return VxTimeUtil::formatGmtTimeStampIntoHoursAndMinutes( m_TimeGmtMs + m_ZoneOffsMs, GetUseMillitaryTime() );
    }
}

//============================================================================
std::string TimeWithZone::getLocalDateAndTimeWithTextMonths( bool localTime )
{
    return VxTimeUtil::formatTimeStampIntoDateAndTimeWithTextMonths( localTime ? m_TimeGmtMs + LocalTimeZoneDifferenceMs() : m_TimeGmtMs + m_ZoneOffsMs, GetUseMillitaryTime() );
}

//============================================================================
std::string TimeWithZone::getLocalDateAndTimeWithNumberMonths( bool localTime  )
{
    return VxTimeUtil::formatTimeStampIntolDateAndTimeWithNumberMonths( localTime ? m_TimeGmtMs + LocalTimeZoneDifferenceMs() : m_TimeGmtMs + m_ZoneOffsMs, GetUseMillitaryTime() );
}

//============================================================================
// puts http formated time in retBuf.. returns length
int  TimeWithZone:: getHttpDateTime( char * retBuf, bool localTime )
{
    return VxTimeUtil::getHttpDateTime( localTime ?  m_TimeGmtMs + m_ZoneOffsMs : m_TimeGmtMs, retBuf );
}

//============================================================================
std::string TimeWithZone::formatTimeDiffIntoMinutesAndSeconds( TimeWithZone& otherTime, bool localTime )
{
    return VxTimeUtil::formatTimeDiffIntoMinutesAndSeconds( getTimeMs( localTime ) - otherTime.getTimeMs( localTime ) );
}
