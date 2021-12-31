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

#include "HostedInfo.h"

#include <CoreLib/DbBase.h>

class HostedListDb : public DbBase
{
public:
	HostedListDb();
	virtual ~HostedListDb();

	RCODE						hostedListDbStartup( int dbVersion, const char* dbFileName );
	RCODE						hostedListDbShutdown( void );

	void						getAllHosteds( std::vector<HostedInfo>& hostedList );
	bool						saveHosted( HostedInfo& hostedInfo );
	void						removeClosedPortIdent( VxGUID& onlineId );

protected:
	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );
};


