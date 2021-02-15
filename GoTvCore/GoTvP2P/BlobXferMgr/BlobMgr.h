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

#include "BlobMgrBase.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class PktFileListReply;

class BlobInfo;
class BlobInfoDb;
class BlobHistoryMgr;

class BlobMgr : public BlobMgrBase
{
public:
	BlobMgr( P2PEngine& engine );
	virtual ~BlobMgr();

	bool						isBlobListInitialized( void )				{ return m_BlobListInitialized; }
	// startup when user specific directory has been set after user logs on
	void						fromGuiUserLoggedOn( void );
	void						assetInfoMgrStartup( VxThread * startupThread );
	void						assetInfoMgrShutdown( void );

	virtual bool				fromGuiGetBlobInfo( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId );

	bool						isSharedFileBlob( std::string& fileName );
	bool						isSharedFileBlob( VxSha1Hash& fileHashId );
	bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );
	bool						getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName );


	std::vector<BlobInfo*>&	getBlobInfoList( void )				{ return m_BlobInfoList; }
	BlobInfo *					findBlob( std::string& fileName );
	BlobInfo *					findBlob( VxSha1Hash& fileHashId );
	BlobInfo *					findBlob( VxGUID& assetId );

	uint16_t					getBlobFileTypes( void )				{ return m_u16BlobFileTypes; }
	void						updateBlobFileTypes( void );

	void						lockFileListPackets( void )				{ m_FileListPacketsMutex.lock(); }
	void						unlockFileListPackets( void )			{ m_FileListPacketsMutex.unlock(); }
	std::vector<PktFileListReply*>&	getFileListPackets( void )			{ return m_FileListPackets; }
	void						updateFileListPackets( void );

	void						fileWasShredded( const char * fileName );

    BlobInfo * 				addBlobFile( const char * fileName, uint64_t fileLen, uint8_t fileType );

	bool						addBlobFile(	const char *	fileName, 
												VxGUID&			assetId,  
												uint8_t *		hashId = 0, 
												EBlobLocation	locationFlags = eBlobLocUnknown, 
												const char *	assetTag = "", 
												int64_t		    timestamp = 0 );

	bool						addBlobFile(	const char *	fileName, 
												VxGUID&			assetId,  
												VxGUID&		    creatorId, 
												VxGUID&		    historyId, 
												uint8_t *		hashId = 0, 
												EBlobLocation	locationFlags = eBlobLocUnknown, 
												const char *	assetTag = "", 
                                                int64_t			timestamp = 0 );

	bool						addBlob( BlobInfo& assetInfo );
	bool						removeBlob( std::string fileName );
	bool						removeBlob( VxGUID& assetUniqueId );
	void						queryHistoryBlobs( VxGUID& historyId );

	void						generateHashForFile( std::string fileName );
	void						updateBlobXferState( VxGUID& assetUniqueId, EBlobSendState assetSendState, int param = 0 );
	void						announceBlobXferState( VxGUID& assetUniqueId, EBlobSendState assetSendState, int param );

protected:
	void						updateBlobListFromDb( VxThread * thread );
	void						generateHashIds( VxThread * thread );
	void						clearBlobFileListPackets( void );
	void						clearBlobInfoList( void );
    BlobInfo *                 createBlobInfo( const char * fileName, uint64_t fileLen, uint8_t fileType );
	BlobInfo *					createBlobInfo(	const char *	fileName, 
													VxGUID&			assetId,  
													uint8_t *		hashId, 
													EBlobLocation	locationFlags = eBlobLocUnknown, 
													const char *	assetTag = "", 
													int64_t			timestamp = 0 );
	bool						insertNewInfo( BlobInfo * assetInfo );
	void						updateDatabase( BlobInfo * assetInfo );
	void						updateBlobDatabaseSendState( VxGUID& assetUniqueId, EBlobSendState sendState );
	void						announceBlobAdded( BlobInfo * assetInfo );
	void						announceBlobRemoved( BlobInfo * assetInfo );
	void						insertBlobInTimeOrder( BlobInfo * assetInfo );

	//=== vars ===//
	bool						m_Initialized;
	bool						m_BlobListInitialized;
	BlobInfoDb&				m_BlobInfoDb;
	std::vector<BlobInfo*>	m_BlobInfoList;

	std::vector<BlobInfo*>	m_WaitingForHastList;
	std::vector<std::string>	m_GenHashList;
	VxMutex						m_GenHashMutex;
	VxThread					m_GenHashThread;
	VxSemaphore					m_GenHashSemaphore;

	uint16_t					m_u16BlobFileTypes;
	VxMutex						m_FileListPacketsMutex;
	std::vector<PktFileListReply*> m_FileListPackets;
};

