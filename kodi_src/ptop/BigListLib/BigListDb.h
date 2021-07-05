#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <config_gotvcore.h>
#include "BigList.h"

#include <CoreLib/DbBase.h>
#include <CoreLib/VxThread.h>

class BigListMgr;
class P2PEngine;

class BigListDb : public DbBase, public BigList
{
public:
	BigListDb( BigListMgr& bigListMgr );
	virtual ~BigListDb() override = default;

	bool						isBigListInitialized( void )			{ return m_BigListDbInitialized; }
	std::string&				getNetworkKey( void )					{ return m_NetworkName; }

	//! restore all of given network to lists from database
	RCODE						dbRestoreAll( const char * networkName );
	RCODE						dbUpdateSessionTime( VxGUID& onlineId, int64_t lastSessionTime, const char * networkName );

protected:
	//=== overrides ===//
	//! create tables in database 
    virtual RCODE				onCreateTables( int iDbVersion ) override;
	//! delete tables from database 
    virtual RCODE				onDeleteTables( int oldVersion ) override;

	RCODE						bigListDbStartup( const char * pDbFileName );
	RCODE						bigListDbShutdown( void );

	//! if not in db insert BigListInfo else update database
	RCODE						dbUpdateBigListInfo( BigListInfo * poInfo, const char * networkName );

	//! remove friend by id
	RCODE						dbRemoveBigListInfo( VxGUID& oId );

	//! insert big list info node into database
	RCODE						dbInsertBigListInfoIntoDb( BigListInfo * poInfo, const char * networkName );

	//! update big list info node in database
	RCODE						dbUpdateBigListInfoInDb( BigListInfo * poInfo, const char * networkName );

	//! make big list info into blob
	RCODE						saveBigListInfoIntoBlob( BigListInfo * poInfo, uint8_t * * ppu8RetBlob, int * piRetBlobLen );
	//! restore big list info from blob
	RCODE						restoreBigListInfoFromBlob( uint8_t * pu8Data, int iDataLen, BigListInfo * poInfo, uint64_t lastSessionTime );

	//=== vars ===//
	VxMutex						m_DbMutex;						// mutex for database access
	VxThread					m_BigListLoadThread;			// thread to load nodes from database
	BigListMgr&					m_BigListMgr;
	std::string					m_NetworkName;
	bool						m_BigListDbInitialized = false;
};
