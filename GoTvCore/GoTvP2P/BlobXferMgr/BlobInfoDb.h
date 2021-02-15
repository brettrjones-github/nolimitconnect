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
#include <CoreLib/BlobDefs.h>

class BlobInfo;
class BlobMgr;
class VxGUID;
class VxSha1Hash;

class BlobInfoDb : public DbBase
{
public:
	BlobInfoDb( BlobMgr& mgr );
	virtual ~BlobInfoDb();

	void						lockBlobInfoDb( void )					{ m_BlobInfoDbMutex.lock(); }
	void						unlockBlobInfoDb( void )				{ m_BlobInfoDbMutex.unlock(); }

	void						addBlob(	VxGUID&			assetId, 
											VxGUID&			creatorId, 
											VxGUID&			historyId, 
											const char *	assetName, 
											int64_t			assetLen, 
											uint32_t		assetType, 							
											VxSha1Hash&		hashId, 
											uint32_t		locationFlags, 
											const char *	assetTag = "", 
											time_t			timestamp = 0,
											EBlobSendState sendState = eBlobSendStateNone );

	void 						addBlob( BlobInfo * assetInfo );
	void						removeBlob( const char * assetName );
	void						removeBlob( VxGUID& assetId );
	void						removeBlob( BlobInfo * assetInfo );

	void						getAllBlobs( std::vector<BlobInfo*>& BlobBlobList );
	void						purgeAllBlobs( void ); 
	void						updateBlobTypes( void );
	void						updateBlobSendState( VxGUID& assetId, EBlobSendState sendState );

protected:
	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );
	void						insertBlobInTimeOrder( BlobInfo *assetInfo, std::vector<BlobInfo*>& assetList );

	BlobMgr&					m_BlobMgr;
	VxMutex						m_BlobInfoDbMutex;
};

