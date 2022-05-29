#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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


class VxAppInfo
{
public:
	const char *			getAppName( void );
	const char *			getAppNameNoSpaces( void );
    const char *			getAppNameNoSpacesLowerCase( void );
    const char *            getCompanyWebsite( void );
    const char *            getCompanyDomain( void );

	int						getVersionMajor( void );
	int						getVersionMinor( void );
	const char *			getVersionSuffix( void );
    std::string			    getVersionString( void );

	const char *			getSCMID( void );
};

