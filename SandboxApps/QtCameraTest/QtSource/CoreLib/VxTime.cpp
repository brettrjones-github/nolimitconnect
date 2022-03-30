//============================================================================
// Created by Brett R. Jones in 2018 and issued to public domain
//============================================================================
#include "AppCompilerConfig.h"

#include "VxTime.h"
#include "VxTimeUtil.h"
#include "VxDebug.h"

#include <time.h>
//#include <sysheaders/sys/time.h>
#if defined(TARGET_OS_WINDOWS)
# include "windows.h"
#define ZeroMemory2(Destination,Length) memset((Destination),0,(Length))

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DEF_DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DEF_DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

const __int64 DELTA_EPOCH_IN_MICROSECS = DEF_DELTA_EPOCH_IN_MICROSECS;

// simulate gettimeofday
//struct timezone
//{
//    int  tz_minuteswest; /* minutes W of Greenwich */
//    int  tz_dsttime;     /* type of dst correction */
//};
#endif // TARGET_OS_WINDOWS

#if !defined(TARGET_OS_WINDOWS)
#include <time.h>
#include <sys/time.h>

uint64_t GetTickCount64()
{
    // if the posix os has CLOCK_MONOTONIC defined then is better to use clock_gettime
    // clock_gettime will not jump if user changes the current time. going backwards in time could cause issues
#if defined(CLOCK_MONOTONIC)
    // even if does not highlight in qt this is active for android
    struct timespec tspec;
    tspec.tv_sec = tspec.tv_nsec = 0;
    clock_gettime( CLOCK_MONOTONIC, &tspec );
# if defined(TARGET_OS_ANDROID) || defined(TARGET_OS_LINUX)
    int64_t timeNow = ( (int64_t)tspec.tv_sec * 1000LL ) + ( (int64_t)tspec.tv_nsec / 1000000L );
# else
    int64_t timeNow = ( (int64_t)tspec.tv_sec * 1000LL ) + ( (int64_t)tspec.tv_usec / 1000L );
# endif // defined(TARGET_OS_ANDROID)
    static int64_t lastTime = 0;
    if( timeNow < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR MONOTONIC GetTickCount64() Went backwards from %lld to %lld", lastTime, timeNow);
    }

    lastTime = timeNow;
    return timeNow;
#else
    struct timeval tv;
    if( gettimeofday( &tv, NULL ) != 0 )
    {
        LogMsg(LOG_ERROR, "GetTickCount64() Failed gettimeofday");
        return 0;
    }

    int64_t timeNow = ( (int64_t)tv.tv_sec * 1000LL ) + ( (int64_t)tv.tv_usec / 1000L );
    static int64_t lastTime = 0;
    if( timeNow < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR GetTickCount64() Went backwards from %lld to %lld", lastTime, timeNow);
        lastTime = timeNow;
    }

    return timeNow;
#endif // CLOCK_MONOTONIC
}
#endif // TARGET_OS_WINDOWS

GOTV_BEGIN_CDECLARES
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
GOTV_END_CDECLARES

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
int64_t	GetApplicationAliveMs( void )
{
    InitializeTimeIfNeeded();
    int64_t tickCnt = (int64_t)GetTickCount64() - g_tickCountWhenAppInitialized;
    static int64_t lastTime = 0;
    if( tickCnt < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR MONOTONIC GetApplicationAliveMs() Went backwards from %lld to %lld", lastTime, tickCnt);
    }

    lastTime = tickCnt;
    return tickCnt;
}

//============================================================================
// milli seconds since January 1, 1970 GMT time
int64_t	GetGmtTimeMs( void )
{
    InitializeTimeIfNeeded();
    int64_t tickCnt = (int64_t)GetTickCount64() + g_tickCountOffsetMsFromGmtTime;
    static int64_t lastTime = 0;
    if( tickCnt < lastTime )
    {
        LogMsg(LOG_ERROR, "ERROR MONOTONIC GetGmtTimeMs() Went backwards from %lld to %lld", lastTime, tickCnt);
    }

    lastTime = tickCnt;
    return tickCnt;
}

//============================================================================
// milli seconds since January 1, 1970 local time
int64_t	GetLocalTimeMs( void )
{
    InitializeTimeIfNeeded();
    return GetTickCount64() + g_tickCountOffsetMsFromLocalTime;
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

    g_isTimeInitialized = true;
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
