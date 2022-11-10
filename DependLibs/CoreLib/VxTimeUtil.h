#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/config_corelib.h>
#include <string>
#include <time.h>

class VxTimeUtil
{
public:

	static std::string			getFileNameCompatibleDate( int64_t timeGmtMs );
	static std::string			getFileNameCompatibleDateAndTime( int64_t timeGmtMs );

	static std::string			getLocalTime( void );
	static std::string			getChatHourMinTimeStamp( void );

	static std::string			getLocalDateAndTimeWithTextMonths( void );
	static std::string			getLocalDateAndTimeWithNumberMonths( void );
	// shortest time ie. if less than a day ago will be just hour and minute. if more than a year ago then full HH:MM MM/DD/YYYY
	static std::string	        getShortestTime( int64_t timeGmtMs, bool localTime = true );

	// puts http formated time in retBuf.. returns length
	static int					getHttpDateTime( int64_t timeSinceJan1970GmtMs, char * retBuf );
	static std::string			formatTimeStampIntoHoursAndMinutes( int64_t timeSinceJan1970GmtMs );
    static std::string          formatGmtTimeStampIntoHoursAndMinutes( int64_t timeSinceJan1970GmtMs );
	static std::string			formatTimeStampIntoHoursAndMinutesAndSeconds( int64_t timeSinceJan1970GmtMs );
    static std::string			formatGmtTimeStampIntoHoursAndMinutesAndSeconds( int64_t timeSinceJan1970GmtMs );
    static std::string			formatTimeStampIntoDateAndTimeWithTextMonths( int64_t timeSinceJan1970GmtMs );
    static std::string			formatTimeStampIntolDateAndTimeWithNumberMonths( int64_t timeSinceJan1970GmtMs );
	static std::string			formatTimeDiffIntoMinutesAndSeconds( int64_t timeDifMs );
	static void					splitIntoHoursMinutesSeconds( int64_t timeDifMs, int& hrs, int& min, int& sec );
};
