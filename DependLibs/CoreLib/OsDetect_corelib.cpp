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
#include "config_corelib.h"

#include "OsDetect.h"
#include "VxDebug.h"

#ifdef TARGET_OS_WINDOWS
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
# pragma warning( disable : 4996 ) //  'GetVersionExA': was declared deprecated
#endif // TARGET_OS_WINDOWS

//============================================================================
char * OsDetect::getOsName( void )
{
     if( isAndroid() )
     {
         return (char *)"Android";
     }
     else if( isWindowsPlatform() )
     {
         return (char *)"Windows";
     }
     else if( isLinux() )
     {
         return (char *)"Linux";
     }
     else if( isApple() )
     {
         return (char *)"Apple";
     }
     else if( isRasberryPi() )
     {
         return (char *)"RasberryPi";
     }
     else
     {
         return (char *)"UnknownOS";
     }
}

//============================================================================
bool OsDetect::isAndroid( void )
{
#ifdef TARGET_OS_ANDROID
	return true;
#endif // TARGET_OS_ANDROID

	return false;
}

//============================================================================
bool OsDetect::isLinux( void )
{
    if( !isAndroid()
        && !isWindowsPlatform()
        && !isApple()
        && !isRasberryPi() )
    {
        // assume some flavor of linux
        return true;
    }

    return false;
}

//============================================================================
bool OsDetect::isApple( void )
{
#ifdef TARGET_OS_APPLE
    return true;
#endif

    return false;
}

//============================================================================
bool OsDetect::isRasberryPi( void )
{
#ifdef TARGET_OS_RASPBERRY_PI
    return true;
#endif

    return false;
}

//============================================================================
bool OsDetect::isWindowsPlatform( void )
{
#ifdef TARGET_OS_WINDOWS
	return true;
#else
	return false;
#endif // TARGET_OS_WINDOWS
}


//Version Number    Description
//6.1               Windows 7     / Windows 2008 R2
//6.0               Windows Vista / Windows 2008
//5.2               Windows 2003 
//5.1               Windows XP
//5.0               Windows 2000

//============================================================================
double OsDetect::getWindowsVersionNumber( void )
{
#ifdef TARGET_OS_WINDOWS
	OSVERSIONINFO verInfo;
	memset( &verInfo, 0, sizeof( verInfo ) );
	verInfo.dwOSVersionInfoSize = sizeof(verInfo);
	BOOL result = GetVersionEx( &verInfo );
	if( result )
	{
		char buf[ 32 ];
		sprintf( buf, "%d.%d", verInfo.dwMajorVersion, verInfo.dwMinorVersion );
		return atof( buf );
	}

	RCODE rc = VxGetLastError();
	LogMsg( LOG_ERROR, "OsDetect::getWindowsVersionNumber failed with error %d\n", rc );
	return 0;
#else
	LogMsg( LOG_ERROR, "OsDetect::getWindowsVersionNumber called from unknown OS" );
	return 0;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
bool OsDetect::isWindows7( void )
{
#ifdef TARGET_OS_WINDOWS
	return ( 6.1 <= getWindowsVersionNumber() );
#else
	return false;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
bool OsDetect::isVista( void )
{
#ifdef TARGET_OS_WINDOWS
	return ( 6.0 == getWindowsVersionNumber() );
#else
	return false;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
bool OsDetect::isWindows2003( void )
{
#ifdef TARGET_OS_WINDOWS
	return ( 5.2 == getWindowsVersionNumber() );
#else
	return false;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
bool OsDetect::isXP( void )
{
#ifdef TARGET_OS_WINDOWS
	return ( 5.1 == getWindowsVersionNumber() );
#else
	return false;
#endif // TARGET_OS_WINDOWS
}
