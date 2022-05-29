//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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
#include "AppErr.h"
#include "VxDebug.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MAX_APP_ERR_MSG_SIZE 1024

void default_app_err_handler( void * userData, EAppErr eAppErr, char * errMsg );

namespace
{
	APP_ERR_FUNCTION g_pfuncAppErrHandler = default_app_err_handler;
	void * g_pvUserData = 0;
}

//============================================================================
void VxSetAppErrHandler( APP_ERR_FUNCTION pfuncAppErrHandler, void * userData )
{
	g_pfuncAppErrHandler = pfuncAppErrHandler;
	g_pvUserData = userData;
}

//============================================================================
void default_app_err_handler( void * userData, EAppErr eAppErr, char * errMsg )
{
    GOTV_DECLARE_UNUSED( userData );
	LogMsg( LOG_ERROR, "AppErr: %d %s\n", eAppErr, errMsg );
}

//============================================================================
void AppErr( EAppErr eAppErr, const char* errMsg, ...)
{
	char as8Buf[ MAX_APP_ERR_MSG_SIZE ];
	va_list argList;
	va_start( argList, errMsg );
	vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	g_pfuncAppErrHandler( g_pvUserData, eAppErr, as8Buf );
}

