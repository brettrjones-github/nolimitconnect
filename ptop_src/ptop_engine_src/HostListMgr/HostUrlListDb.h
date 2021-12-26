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
// http://www.nolimitconnect.com
//============================================================================

#include "HostUrlInfo.h"

#include <CoreLib/DbBase.h>

class HostUrlListDb : public DbBase
{
public:
	HostUrlListDb();
	virtual ~HostUrlListDb();

	RCODE						hostUrlListDbStartup( int dbVersion, const char* dbFileName );
	RCODE						hostUrlListDbShutdown( void );

	void						getAllHostUrls( std::vector<HostUrlInfo>& hostUrlList );
	bool						saveHostUrl( HostUrlInfo& hostUrlInfo );
	void						removeClosedPortIdent( VxGUID& onlineId );

protected:
	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );
};


