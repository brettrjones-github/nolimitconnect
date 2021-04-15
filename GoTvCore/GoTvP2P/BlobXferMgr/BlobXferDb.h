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

#include <config_gotvcore.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class BlobInfo;

class BlobXferDb : public DbBase
{
public:
	BlobXferDb( const char * stateDbName );
	virtual ~BlobXferDb() = default;

	void						lockBlobXferDb( void )			    { m_BlobXferDbMutex.lock(); }
	void						unlockBlobXferDb( void )			{ m_BlobXferDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addBlob( VxGUID& assetUniqueId );
	void						removeBlob( VxGUID& assetUniqueId );

	void						getAllBlobs( std::vector<VxGUID>& assetList );
	void						purgeAllBlobXfer( void ); 

protected:
	VxMutex						m_BlobXferDbMutex;
};

