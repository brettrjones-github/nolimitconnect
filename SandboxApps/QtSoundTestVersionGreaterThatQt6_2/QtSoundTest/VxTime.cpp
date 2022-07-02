#include "VxTime.h"
#include "VxTimeUtil.h"
#include <time.h>
#ifdef TARGET_OS_LINUX
# include <sys/time.h>
#endif // TARGET_OS_LINUX
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
struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

//============================================================================
int gettimeofday( struct timeval *tv/*in*/, struct timezone *tz/*in*/ )
{
    FILETIME ft;
    __int64 tmpres = 0;
    TIME_ZONE_INFORMATION tz_winapi;
    int rez = 0;
    ZeroMemory2( &ft, sizeof( ft ) );
    ZeroMemory2( &tz_winapi, sizeof( tz_winapi ) );

    GetSystemTimeAsFileTime( &ft );

    tmpres = ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (__int32)( tmpres*0.000001 );
    tv->tv_usec = ( tmpres % 1000000 );


    //_tzset(),don't work properly, so we use GetTimeZoneInformation
    if( tz ) // it is normal for tz to be passed as null so do check for null
    {
        rez = GetTimeZoneInformation( &tz_winapi );
        tz->tz_dsttime = ( rez == 2 ) ? true : false;
        tz->tz_minuteswest = tz_winapi.Bias + ( ( rez == 2 ) ? tz_winapi.DaylightBias : 0 );
    }

    return 0;
    }

#endif // defined(TARGET_OS_WINDOWS)

#if !defined(TARGET_OS_WINDOWS)

uint64_t GetTickCount64()
{
    // if the posix os has CLOCK_MONOTONIC defined then is better to use clock_gettime
    // clock_gettime will not jump if user changes the current time. going backwards in time could cause issues
#ifdef CLOCK_MONOTONIC
    struct timespec now;
    clock_gettime( CLOCK_MONOTONIC, &now );
    return ( (int64_t)now.tv_sec * 1000L ) + ( now.tv_nsec / 1000 );
#else
    struct timeval tv;
    if( gettimeofday( &tv, NULL ) != 0 )
        return 0;

    return ( (int64_t)tv.tv_sec * 1000 ) + ( tv.tv_usec / 1000 );
#endif // CLOCK_MONOTONIC
}

#endif

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


// set time format to military 24hr or AM/PM
void SetUseMillitaryTime( bool useMilitaryTime )
{
    g_UseMilitaryTime = useMilitaryTime;
}

bool GetUseMillitaryTime()
{
    return g_UseMilitaryTime;
}

int64_t	GetTimeStampMs( void )			      // milli seconds since January 1, 1970 GMT time
{
    InitializeTimeIfNeeded();
    return GetGmtTimeMs();
}

// milliseconds since application initialize
int64_t	GetApplicationAliveMs( void )
{
    InitializeTimeIfNeeded();
    return GetTickCount64() - g_tickCountWhenAppInitialized;
}

// milli seconds since January 1, 1970 GMT time
int64_t	GetGmtTimeMs( void )
{
    InitializeTimeIfNeeded();
    return GetTickCount64() + g_tickCountOffsetMsFromGmtTime;
}

// milli seconds since January 1, 1970 local time
int64_t	GetLocalTimeMs( void )
{
    InitializeTimeIfNeeded();
    return GetTickCount64() + g_tickCountOffsetMsFromLocalTime;
}

// convert GMT time to local time
int64_t	GmtTimeMsToLocalTimeMs( int64_t gmtTime )
{
    InitializeTimeIfNeeded();
    return gmtTime + g_localTimezoneDifferenceMs;
}

// convert local time to GMT time
int64_t	LocalTimeMsToGmtTimeMs( int64_t lclTime )
{
    InitializeTimeIfNeeded();
    return lclTime - g_localTimezoneDifferenceMs;
}

// time difference in milliseconds between GMT and local 
int64_t	LocalTimeZoneDifferenceMs( void )
{
    return g_localTimezoneDifferenceMs;
}


static void InitializeTimeIfNeeded( )
{
    if( g_isTimeInitialized )
    {
        return;
    }

    g_isTimeInitialized = true;
    g_tickCountWhenAppInitialized = GetTickCount64();

    // get gmt time in milliseconds since since January 1, 1970 GMT time
    struct timeval gmtTimeVal;
    gettimeofday ( &gmtTimeVal, NULL );
    int64_t gmtTimeMs =  ( gmtTimeVal.tv_sec * 1000 ) + ( gmtTimeVal.tv_usec / 1000 );

    int64_t localTimeMs = 0;
#ifdef TARGET_OS_WINDOWS
    // FILETIME is 100 nanoseconds since JAN 1, 1601
    FILETIME ft;
    GetSystemTimeAsFileTime( &ft );
    // takes the last modified date
    LARGE_INTEGER date, adjust;
    date.HighPart = ft.dwHighDateTime;
    date.LowPart = ft.dwLowDateTime;
    // 100-nanoseconds = milliseconds * 10000
    adjust.QuadPart = 11644473600000 * 10000;
    // removes the difference between 1970 and 1601
    date.QuadPart -= adjust.QuadPart;
    // convert from 100-nanoseconds to milliseconds
    localTimeMs = date.QuadPart / 10000;
#else
    localTimeMs = time( NULL ) * 1000; // only second resolution but does not matter as an offset
#endif // TARGET_OS_WINDOWS

    g_localTimezoneDifferenceMs = localTimeMs - gmtTimeMs;
    g_tickCountOffsetMsFromGmtTime = gmtTimeMs - g_tickCountWhenAppInitialized;
    g_tickCountOffsetMsFromLocalTime = g_tickCountOffsetMsFromGmtTime + g_localTimezoneDifferenceMs;
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

TimeWithZone::TimeWithZone( int64_t timeGmtMs ) // will populate given GMT time and our current time zone in ms
: m_TimeGmtMs( timeGmtMs )
, m_ZoneOffsMs( LocalTimeZoneDifferenceMs() )
{
}

TimeWithZone::TimeWithZone( int64_t timeGmtMs, int32_t timeZoneMs )
: m_TimeGmtMs( timeGmtMs )
, m_ZoneOffsMs( timeZoneMs )
{
}

std::string TimeWithZone::getFileNameCompatibleDate( bool localTime )
{
    return VxTimeUtil::getFileNameCompatibleDate( localTime ? m_TimeGmtMs + m_ZoneOffsMs : m_ZoneOffsMs );
}

std::string TimeWithZone::getFileNameCompatibleDateAndTime( bool localTime )
{
    return VxTimeUtil::getFileNameCompatibleDate( localTime ? m_TimeGmtMs + m_ZoneOffsMs : m_ZoneOffsMs );
}

std::string TimeWithZone::getLocalTime(  )
{
    return VxTimeUtil::formatTimeStampIntoHoursAndMinutes( m_TimeGmtMs, GetUseMillitaryTime() );
}

std::string TimeWithZone::getGmtTime()
{
    return VxTimeUtil::formatGmtTimeStampIntoHoursAndMinutes( m_TimeGmtMs, GetUseMillitaryTime() );
}

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

std::string TimeWithZone::getLocalDateAndTimeWithTextMonths( bool localTime )
{
    return VxTimeUtil::formatTimeStampIntoDateAndTimeWithTextMonths( localTime ? m_TimeGmtMs + LocalTimeZoneDifferenceMs() : m_TimeGmtMs + m_ZoneOffsMs, GetUseMillitaryTime() );
}

std::string TimeWithZone::getLocalDateAndTimeWithNumberMonths( bool localTime  )
{
    return VxTimeUtil::formatTimeStampIntolDateAndTimeWithNumberMonths( localTime ? m_TimeGmtMs + LocalTimeZoneDifferenceMs() : m_TimeGmtMs + m_ZoneOffsMs, GetUseMillitaryTime() );
}

// puts http formated time in retBuf.. returns length
int  TimeWithZone:: getHttpDateTime( char * retBuf, bool localTime )
{
    return VxTimeUtil::getHttpDateTime( localTime ?  m_TimeGmtMs + m_ZoneOffsMs : m_TimeGmtMs, retBuf );
}

std::string TimeWithZone::formatTimeDiffIntoMinutesAndSeconds( TimeWithZone& otherTime, bool localTime )
{
    return VxTimeUtil::formatTimeDiffIntoMinutesAndSeconds( getTimeMs( localTime ) - otherTime.getTimeMs( localTime ) );
}
