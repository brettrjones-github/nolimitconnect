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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/config_corelib.h>

class OsDetect
{
public:
    static char *					getOsName( void ); // returns os platform name.. ie Windows or Linux etc
	
    static bool						isAndroid( void );
    static bool						isLinux( void );
    static bool						isApple( void );
    static bool						isRasberryPi( void );
    static bool						isWindowsPlatform( void );

    static double					getWindowsVersionNumber( void );
    static bool						isWindows7( void );
    static bool						isVista( void );
    static bool						isWindows2003( void );
    static bool						isXP( void );
};
