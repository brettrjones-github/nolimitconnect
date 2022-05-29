#pragma once
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
#include <CoreLib/config_corelib.h>

enum EAppErr
{
	eAppErrUnknown							= -1,
	eAppErrNone								= 0,

	eAppErrFailedToConnect					= 1000000,
	eAppErrTxError							= 1000001,
	eAppErrRxError							= 1000002,
	eAppErrParseError						= 1000003,
	eAppErrPortIsClosed						= 1000004,
	eAppErrFailedToResolveAddr				= 1000005,
	eAppErrThreadCreateFailed				= 1000006,
	eAppErrFailedConnectNetServices			= 1000007,
	eAppErrNetServicesFailedToRespond		= 1000008,
	eAppErrFailedConnectHost				= 1000009,
	eAppErrAccessDenied						= 1000010,
	eAppErrBadParameter						= 1000011,
	eAppErrNoFileExist						= 1000012,
	eAppErrFileRead							= 1000013,
	eAppErrFileWrite						= 1000014,
	eAppErrFileSeek							= 1000015,
	eAppErrBusy								= 1000016,
    eAppErrSeviceError                      = 1000017,

	eAppErrSslMgrFileMissing				= 1100100,
	eAppErrSslMgrUnassignedLogOn			= 1100101,

	eMaxAppErr
};

typedef void (*APP_ERR_FUNCTION )( void *, EAppErr, char * );

void VxSetAppErrHandler( APP_ERR_FUNCTION pfuncAppErrHandler, void * userData );

void AppErr( EAppErr eAppErr, const char* errMsg, ...);

