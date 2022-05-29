//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
#ifndef VXLINUXONLY_H_INCLUDED
#define VXLINUXONLY_H_INCLUDED
#include <CoreLib/config_corelib.h>

#ifndef TARGET_OS_WINDOWS

//=== functions missing in linux that are in visual studio ===//
//! convert int to ascii string
char * itoa(int value, char* str, int base);
//! convert long to ascii string
char * ltoa(long value, char* str, int base);
//! convert unsigned long to ascii string
char * ultoa(unsigned long value, char* str, int base);
//! convert string to upper case
void strupr( char * pStr );
//! convert string to lower case
void strlwr( char * pStr );

#ifndef TARGET_OS_ANDROID
//! case insensitive string compare
int stricmp( const char * pStr1, const char * pStr2 );
//! case insensitive string compare
int strnicmp( const char * pStr1, const char * pStr2, int n );
#endif // TARGET_OS_ANDROID

#endif // TARGET_OS_WINDOWS
#endif // VXLINUXONLY_H_INCLUDED
