#pragma once
//============================================================================
// Created by Brett R. Jones in 2018 and issued to public domain
//============================================================================

// the world of time in c/c++ is a mess with dozens of different implementations
// for different operating systems.. this is my attempt to simplify
// In most cases time should all be GMT and only local when displayed to user 
// also resolution of micro seconds is rarely useful for applications.. more of a driver and profiling tool requirement

#include "VxDefs.h"

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif
    int VxSleep( int milliSec );
#if defined(TARGET_OS_WINDOWS)
# define sleep_os(exp)		VxSleep(exp*1000)
# define msleep_os(exp)		VxSleep(exp)
# define usleep_os(exp)		VxSleep(exp/1000)
#endif // defined(TARGET_OS_WINDOWS)


// set time format to military 24hr or AM/PM
void                        SetUseMillitaryTime( bool useMilitaryTime );
bool                        GetUseMillitaryTime( );

int64_t				        GetTimeStampMs( void );			            // milli seconds since January 1, 1970 GMT time

int64_t				        GetApplicationAliveMs( void );              // milliseconds since application initialize
inline uint32_t				GetApplicationAliveSec( void )              { return ( uint32_t) (GetApplicationAliveMs() / 1000); }

int64_t				        GetGmtTimeMs( void );			            // milli seconds since January 1, 1970 GMT time
inline int64_t				GetGmtTimeSec( void )                       { return GetGmtTimeMs() / 1000; }

int64_t				        GetLocalTimeMs( void );	                    // milli seconds since January 1, 1970 local time
inline int64_t				GetLocalTimeSec( void )                     { return GetLocalTimeMs() / 1000; }

int64_t				        GmtTimeMsToLocalTimeMs( int64_t gmtTime );	  // convert GMT time to local time
inline int64_t				GmtTimeSecToLocalTimeSec( int64_t gmtTime )   { return GmtTimeMsToLocalTimeMs( gmtTime * 1000 ) / 1000; }

int64_t				        LocalTimeSecToGmtTimeMs( int64_t lclTime );	  // convert local time to GMT time
inline int64_t				LocalTimeMsToGmtTimeSec( int64_t lclTime )    { return LocalTimeSecToGmtTimeMs( lclTime * 1000 ) / 1000; }

int64_t				        LocalTimeZoneDifferenceMs( void );		      // time difference in milliseconds between GMT and local time
inline int64_t				LocalTimeZoneDifferenceSec( void )            { return LocalTimeZoneDifferenceMs() / 1000; }
inline int64_t				LocalTimeZoneDifferenceMinutes( void )        { return LocalTimeZoneDifferenceMs() / 60000; }


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>
#pragma pack(push, 1)

class TimeWithZone
{
public:
    TimeWithZone(); // will be invalid ( 0 time )
    TimeWithZone( bool initWithCurrentTime ); // will populate current time and zone
    TimeWithZone( int64_t timeGmtMs ); // will populate given GMT time and our current time zone in ms
    TimeWithZone( int64_t timeGmtMs, int32_t timeZoneMs ); 

    TimeWithZone( const TimeWithZone& timeWithZone ) = default;
    TimeWithZone( TimeWithZone&& timeWithZone ) = default;
    ~TimeWithZone() = default; // do not make virtual or inherit from so will be able to pack into network packets
    TimeWithZone& operator= ( const TimeWithZone & rhs )  = default;
    TimeWithZone& operator= ( TimeWithZone&& rhs )  = default;

    // return ms since Jan 1, 1970.. if local time add the zone offset
    int64_t                     getTimeMs( bool localTime = false )     { return localTime ? m_TimeGmtMs + m_ZoneOffsMs : m_TimeGmtMs; }

    //=== these functions use GetUseMilitaryTime() function to format as millitary 24hr time or AM/PM time ===//
    std::string			        getFileNameCompatibleDate( bool localTime = false );
    std::string			        getFileNameCompatibleDateAndTime( bool localTime = false );

    std::string			        getLocalTime( );
    std::string			        getGmtTime( );
    std::string			        getChatHourMinTimeStamp( bool localTime = false );

    std::string			        getLocalDateAndTimeWithTextMonths( bool localTime = false );
    std::string			        getLocalDateAndTimeWithNumberMonths( bool localTime = false );
    // puts http formated time in retBuf.. returns length
    int					        getHttpDateTime( char * retBuf, bool localTime = false );

    std::string			        formatTimeDiffIntoMinutesAndSeconds( TimeWithZone& otherTime, bool localTime = false );


    int64_t                     m_TimeGmtMs;        // GMT time
    int32_t                     m_ZoneOffsMs;       // Time zone offset in milliseconds
};

#pragma pack(pop)

#endif // c++ only
