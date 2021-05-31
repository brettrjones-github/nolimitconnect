//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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

//#include "config_corelib.h"

#include "VxTimeUtil.h"
#include "VxTime.h"
#include "VxDebug.h"

#include <string.h>
#include <stdio.h>

namespace
{
	const char * g_strDayNames="Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";
	const char * g_strMonthNames="Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec";
	const char * g_strHttpDateTimeFormat="%s, %02d %s %02d %02d:%02d:%02d GMT";
}

//============================================================================
std::string	VxTimeUtil::getFileNameCompatibleDate( int64_t timeGmtMs )
{
    time_t timeSec = (time_t)(timeGmtMs / 1000);
	struct tm * structTime;
	structTime = gmtime( &timeSec );
	char lclBuf[16];
	sprintf( lclBuf, "%04d_%02d_%02d", 
						structTime->tm_year + 1900, 
						structTime->tm_mon + 1, 
						structTime->tm_mday );
	return lclBuf;
}

//============================================================================
std::string	VxTimeUtil::getFileNameCompatibleDateAndTime( int64_t timeGmtMs )
{
    time_t timeSec = ( time_t )( timeGmtMs / 1000 );
	struct tm * structTime;
	structTime = gmtime( &timeSec );
	char lclBuf[32];
	sprintf( lclBuf, "%04d_%02d_%02d_%02d-%02d-%02d", 
		structTime->tm_year + 1900, 
		structTime->tm_mon + 1, 
		structTime->tm_mday,
		structTime->tm_hour,
		structTime->tm_min,
		structTime->tm_sec
		);
	return lclBuf;
}

//============================================================================
int VxTimeUtil::getHttpDateTime( int64_t timeGmtMs, char * retBuf )
{
    time_t timeSec = ( time_t )( timeGmtMs / 1000 );
	struct tm * structTime;
	structTime = gmtime( &timeSec );
	return sprintf(	retBuf, 
					g_strHttpDateTimeFormat,
					g_strDayNames + ( structTime->tm_wday << 2 ),
					structTime->tm_mday,
					g_strMonthNames + ( structTime->tm_mon << 2 ),
					1900 + structTime->tm_year,
					structTime->tm_hour,
					structTime->tm_min,
					structTime->tm_sec );
}

//============================================================================
std::string VxTimeUtil::getLocalTime( bool militaryTime )
{
	time_t timeNow = time( NULL );
	struct tm * structTime;
	structTime = localtime( &timeNow );
	char timeBuf[32];
	if( militaryTime )
	{
		sprintf( timeBuf, "%02d:%02d:%02d",
			structTime->tm_hour,
			structTime->tm_min,
			structTime->tm_sec );
	}
	else
	{
		if( structTime->tm_hour > 12 )
		{
			sprintf( timeBuf, "%02d:%02d:%02d PM",
				structTime->tm_hour - 12,
				structTime->tm_min,
				structTime->tm_sec );
		}
		else
		{
			sprintf( timeBuf, "%02d:%02d:%02d AM",
				structTime->tm_hour,
				structTime->tm_min,
				structTime->tm_sec );
		}
	}

	return timeBuf;
}

//============================================================================
std::string VxTimeUtil::getChatHourMinTimeStamp( bool militaryTime )
{
	time_t timeNow = time( NULL );
	struct tm * structTime;
	structTime = localtime( &timeNow );
	char timeBuf[32];
	if( militaryTime )
	{
		sprintf( timeBuf, " %02d:%02d ",
			structTime->tm_hour,
			structTime->tm_min );
	}
	else
	{
		if( structTime->tm_hour > 12 )
		{
			sprintf( timeBuf, " %02d:%02dPM ",
				structTime->tm_hour - 12,
				structTime->tm_min );
		}
		else
		{
			sprintf( timeBuf, " %02d-%02dAM ",
				structTime->tm_hour,
				structTime->tm_min );
		}
	}

	return timeBuf;
}

//============================================================================
std::string VxTimeUtil::getLocalDateAndTimeWithTextMonths( bool militaryTime )
{
	time_t timeNow = time( NULL );
	struct tm * structTime;
	structTime = localtime( &timeNow );
	char timeBuf[64];
	if( militaryTime )
	{
		sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d",
			g_strMonthNames + ( structTime->tm_mon << 2 ),
			structTime->tm_mday,
			structTime->tm_year + 1900, 
			structTime->tm_hour,
			structTime->tm_min,
			structTime->tm_sec );
	}
	else
	{
		if( structTime->tm_hour > 12 )
		{
			sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d PM",
				g_strMonthNames + ( structTime->tm_mon << 2 ),
				structTime->tm_mday,
				structTime->tm_year + 1900, 
				structTime->tm_hour - 12,
				structTime->tm_min,
				structTime->tm_sec );
		}
		else
		{
			sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d AM",
				g_strMonthNames + ( structTime->tm_mon << 2 ),
				structTime->tm_mday,
				structTime->tm_year + 1900, 
				structTime->tm_hour,
				structTime->tm_min,
				structTime->tm_sec );
		}
	}

	return timeBuf;
}

//============================================================================
std::string	 VxTimeUtil::formatTimeStampIntoDateAndTimeWithTextMonths( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
    struct tm * structTime;
    structTime = gmtime( &timeSec );
    char timeBuf[ 64 ];
    if( militaryTime )
    {
        sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d",
                 g_strMonthNames + ( structTime->tm_mon << 2 ),
                 structTime->tm_mday,
                 structTime->tm_year + 1900,
                 structTime->tm_hour,
                 structTime->tm_min,
                 structTime->tm_sec );
    }
    else
    {
        if( structTime->tm_hour > 12 )
        {
            sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d PM",
                     g_strMonthNames + ( structTime->tm_mon << 2 ),
                     structTime->tm_mday,
                     structTime->tm_year + 1900,
                     structTime->tm_hour - 12,
                     structTime->tm_min,
                     structTime->tm_sec );
        }
        else
        {
            sprintf( timeBuf, "%s %d, %d %02d:%02d:%02d AM",
                     g_strMonthNames + ( structTime->tm_mon << 2 ),
                     structTime->tm_mday,
                     structTime->tm_year + 1900,
                     structTime->tm_hour,
                     structTime->tm_min,
                     structTime->tm_sec );
        }
    }

    return timeBuf;
}

//============================================================================
std::string	 VxTimeUtil::formatTimeStampIntolDateAndTimeWithNumberMonths( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
    struct tm * structTime;
    structTime = gmtime( &timeSec );
    char timeBuf[ 64 ];
    if( militaryTime )
    {
        sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d",
                 structTime->tm_mon + 1,
                 structTime->tm_mday,
                 structTime->tm_year + 1900,
                 structTime->tm_hour,
                 structTime->tm_min,
                 structTime->tm_sec );
    }
    else
    {
        if( structTime->tm_hour > 12 )
        {
            sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d PM",
                     structTime->tm_mon + 1,
                     structTime->tm_mday,
                     structTime->tm_year + 1900,
                     structTime->tm_hour - 12,
                     structTime->tm_min,
                     structTime->tm_sec );
        }
        else
        {
            sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d AM",
                     structTime->tm_mon + 1,
                     structTime->tm_mday,
                     structTime->tm_year + 1900,
                     structTime->tm_hour,
                     structTime->tm_min,
                     structTime->tm_sec );
        }
    }

    return timeBuf;
}

//============================================================================
std::string VxTimeUtil::getLocalDateAndTimeWithNumberMonths( bool militaryTime )
{
	time_t timeNow = time( NULL );
	struct tm * structTime;
	structTime = localtime( &timeNow );
	char timeBuf[64];
	if( militaryTime )
	{
		sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d",
			structTime->tm_mon + 1, 
			structTime->tm_mday,
			structTime->tm_year + 1900, 
			structTime->tm_hour,
			structTime->tm_min,
			structTime->tm_sec );
	}
	else
	{
		if( structTime->tm_hour > 12 )
		{
			sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d PM",
				structTime->tm_mon + 1, 
				structTime->tm_mday,
				structTime->tm_year + 1900, 
				structTime->tm_hour - 12,
				structTime->tm_min,
				structTime->tm_sec );
		}
		else
		{
			sprintf( timeBuf, "%d/%d/%d %02d:%02d:%02d AM",
				structTime->tm_mon + 1, 
				structTime->tm_mday,
				structTime->tm_year + 1900, 
				structTime->tm_hour,
				structTime->tm_min,
				structTime->tm_sec );
		}
	}

	return timeBuf;
}

//============================================================================
std::string VxTimeUtil::formatTimeStampIntoHoursAndMinutes( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
	struct tm * structTime;
	structTime = localtime( &timeSec );
	char timeBuf[32];
	if( militaryTime )
	{
		sprintf( timeBuf, "%02d:%02d",
			structTime->tm_hour,
			structTime->tm_min );
	}
	else
	{
		if( structTime->tm_hour > 12 )
		{
			sprintf( timeBuf, "%02d:%02d PM",
				structTime->tm_hour - 12,
				structTime->tm_min );
		}
		else
		{
			sprintf( timeBuf, "%02d:%02d AM",
				structTime->tm_hour,
				structTime->tm_min );
		}
	}

	return timeBuf;
}

//============================================================================
std::string VxTimeUtil::formatGmtTimeStampIntoHoursAndMinutes( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
    struct tm * structTime;
    structTime = gmtime( &timeSec );
    char timeBuf[ 32 ];
    if( militaryTime )
    {
        sprintf( timeBuf, "%02d:%02d",
                 structTime->tm_hour,
                 structTime->tm_min );
    }
    else
    {
        if( structTime->tm_hour > 12 )
        {
            sprintf( timeBuf, "%02d:%02d PM",
                     structTime->tm_hour - 12,
                     structTime->tm_min );
        }
        else
        {
            sprintf( timeBuf, "%02d:%02d AM",
                     structTime->tm_hour,
                     structTime->tm_min );
        }
    }

    return timeBuf;
}

//============================================================================
std::string VxTimeUtil::formatTimeStampIntoHoursAndMinutesAndSeconds( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
	struct tm * structTime;
	structTime = localtime( &timeSec );
    char timeBuf[ 32 ];

    if( structTime )
    {
	    if( militaryTime )
	    {
		    sprintf( timeBuf, "%02d:%02d:%02d",
			    structTime->tm_hour,
			    structTime->tm_min,
			    structTime->tm_sec );
	    }
	    else
	    {
		    if( structTime->tm_hour > 12 )
		    {
			    sprintf( timeBuf, "%02d:%02d:%02d PM",
				    structTime->tm_hour - 12,
				    structTime->tm_min,
				    structTime->tm_sec );
		    }
		    else
		    {
			    sprintf( timeBuf, "%02d:%02d:%02d AM",
				    structTime->tm_hour,
				    structTime->tm_min,
				    structTime->tm_sec );
		    }
	    }
    }
    else
    {
        LogMsg( LOG_ERROR, "Invalid Time %d", timeSec );
        strcpy( timeBuf, "00:00:00" );
    }

	return timeBuf;
}

//============================================================================
std::string VxTimeUtil::formatGmtTimeStampIntoHoursAndMinutesAndSeconds( int64_t timeSinceJan1970GmtMs, bool militaryTime )
{
    time_t timeSec = ( time_t )( timeSinceJan1970GmtMs / 1000 );
    struct tm * structTime;
    structTime = gmtime( &timeSec );
    char timeBuf[ 32 ];

    if( structTime )
    {
        if( militaryTime )
        {
            sprintf( timeBuf, "%02d:%02d:%02d",
                     structTime->tm_hour,
                     structTime->tm_min,
                     structTime->tm_sec );
        }
        else
        {
            if( structTime->tm_hour > 12 )
            {
                sprintf( timeBuf, "%02d:%02d:%02d PM",
                         structTime->tm_hour - 12,
                         structTime->tm_min,
                         structTime->tm_sec );
            }
            else
            {
                sprintf( timeBuf, "%02d:%02d:%02d AM",
                         structTime->tm_hour,
                         structTime->tm_min,
                         structTime->tm_sec );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "Invalid Time %d", timeSec );
        strcpy( timeBuf, "00:00:00" );
    }

    return timeBuf;
}

//============================================================================
std::string VxTimeUtil::formatTimeDiffIntoMinutesAndSeconds( int64_t timeDifMs )
{
	char lclBuf[ 64 ];
	int hrs;
	int min; 
	int sec;
	splitIntoHoursMinutesSeconds( timeDifMs, hrs, min, sec );
	if( hrs )
	{
		sprintf( lclBuf, "%02d:%02d:%02d", hrs, min, sec );
	}
	else
	{
		sprintf( lclBuf, "%02d:%02d", min, sec );
	}

	return lclBuf;
}

//============================================================================
void VxTimeUtil::splitIntoHoursMinutesSeconds( int64_t timeDifMs, int& hrs, int& min, int& sec )
{
    int64_t timeDifSec = ( timeDifMs / 1000 );

	hrs				= (int)(timeDifSec / ( 60 * 60 ));
	int remaining	= timeDifSec % ( 60 * 60 );
	min				= remaining / 60;
	sec				= remaining % 60;
}
