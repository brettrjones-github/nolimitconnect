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

#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class PktFileListReply;

class AssetBaseCallbackInterface;
class AssetBaseInfo;
class AssetBaseInfoDb;
class AssetBaseHistoryMgr;
class IToGui;
class P2PEngine;

class AssetBaseMgr
{
public:
	AssetBaseMgr( P2PEngine& engine, const char * dbName );
	virtual ~AssetBaseMgr();

    class AutoResourceLock
    {
    public:
        AutoResourceLock( AssetBaseMgr * assetMgrBase ) : m_Mutex(assetMgrBase->getResourceMutex())	{ m_Mutex.lock(); }
        ~AutoResourceLock()																			{ m_Mutex.unlock(); }
        VxMutex&				m_Mutex;
    };

    virtual std::vector<AssetBaseInfo*>&	getAssetBaseInfoList( void )				{ return m_AssetBaseInfoList; }


    virtual void				announceAssetAdded( AssetBaseInfo * assetInfo );
    virtual void				announceAssetRemoved( AssetBaseInfo * assetInfo );
    virtual void				announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param );

    virtual void                onQueryHistoryAsset( AssetBaseInfo* assetInfo ) {}; // should be overriden

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    void						addAssetMgrClient( AssetBaseCallbackInterface * client, bool enable );
    bool						isAllowedFileOrDir( std::string strFileName );

	virtual bool				isAssetListInitialized( void )				{ return m_AssetBaseListInitialized; }
	// startup when user specific directory has been set after user logs on
	void						fromGuiUserLoggedOn( void );
	void						assetInfoMgrStartup( VxThread * startupThread );
	void						assetInfoMgrShutdown( void );

	virtual bool				fromGuiGetAssetBaseInfo( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId );

    bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );
	bool						getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName );


	AssetBaseInfo *				findAsset( std::string& fileName );
	AssetBaseInfo *				findAsset( VxSha1Hash& fileHashId );
	AssetBaseInfo *				findAsset( VxGUID& assetId );

	uint16_t					getAssetBaseFileTypes( void )				{ return m_u16AssetBaseFileTypes; }
	void						updateAssetFileTypes( void );

	void						lockFileListPackets( void )				{ m_FileListPacketsMutex.lock(); }
	void						unlockFileListPackets( void )			{ m_FileListPacketsMutex.unlock(); }
	std::vector<PktFileListReply*>&	getFileListPackets( void )			{ return m_FileListPackets; }
	void						updateFileListPackets( void );

	void						fileWasShredded( const char * fileName );

    AssetBaseInfo * 			addAssetFile( const char * fileName, uint64_t fileLen, uint8_t fileType );

	bool						addAssetFile(	const char *	fileName, 
												VxGUID&			assetId,  
												uint8_t *		hashId = 0, 
												EAssetLocation	locationFlags = eAssetLocUnknown, 
												const char *	assetTag = "", 
												int64_t		    timestamp = 0 );

	bool						addAssetFile(	const char *	fileName, 
												VxGUID&			assetId,  
												VxGUID&		    creatorId, 
												VxGUID&		    historyId, 
												uint8_t *		hashId = 0, 
												EAssetLocation	locationFlags = eAssetLocUnknown, 
												const char *	assetTag = "", 
                                                int64_t			timestamp = 0 );

	bool						addAsset( AssetBaseInfo& assetInfo );
	bool						removeAsset( std::string fileName );
	bool						removeAsset( VxGUID& assetUniqueId );
	void						queryHistoryAssets( VxGUID& historyId );

	void						generateHashForFile( std::string fileName );
	void						updateAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param = 0 );


protected:
    void						lockClientList( void )						{ m_ClientListMutex.lock(); }
    void						unlockClientList( void )					{ m_ClientListMutex.unlock(); }

	void						updateAssetListFromDb( VxThread * thread );
	void						generateHashIds( VxThread * thread );
	void						clearAssetFileListPackets( void );
	void						clearAssetInfoList( void );
    AssetBaseInfo *             createAssetInfo( const char * fileName, uint64_t fileLen, uint8_t fileType );
	AssetBaseInfo *				createAssetInfo(	const char *	fileName, 
													VxGUID&			assetId,  
													uint8_t *		hashId, 
													EAssetLocation	locationFlags = eAssetLocUnknown, 
													const char *	assetTag = "", 
													int64_t			timestamp = 0 );
	bool						insertNewInfo( AssetBaseInfo * assetInfo );
	void						updateDatabase( AssetBaseInfo * assetInfo );
	void						updateAssetDatabaseSendState( VxGUID& assetUniqueId, EAssetSendState sendState );
	void						insertAssetInTimeOrder( AssetBaseInfo * assetInfo );

    //=== vars ===//
    P2PEngine&					m_Engine;
    VxMutex						m_ResourceMutex;
    VxMutex						m_ClientListMutex;

    std::vector<AssetBaseCallbackInterface *> m_AssetClients;

	bool						m_Initialized{ false };
	bool						m_AssetBaseListInitialized{ false };
	AssetBaseInfoDb&			m_AssetBaseInfoDb;
	std::vector<AssetBaseInfo*>	m_AssetBaseInfoList;

	std::vector<AssetBaseInfo*>	m_WaitingForHastList;
	std::vector<std::string>	m_GenHashList;
	VxMutex						m_GenHashMutex;
	VxThread					m_GenHashThread;
	VxSemaphore					m_GenHashSemaphore;

    uint16_t					m_u16AssetBaseFileTypes{ 0 };
	VxMutex						m_FileListPacketsMutex;
	std::vector<PktFileListReply*> m_FileListPackets;
};
