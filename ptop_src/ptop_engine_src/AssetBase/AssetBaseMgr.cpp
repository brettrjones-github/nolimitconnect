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

#include "AssetBaseMgr.h"
#include "AssetBaseInfo.h"
#include "AssetBaseInfoDb.h"
#include "AssetBaseCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfoDb.h>
#include <ptop_src/ptop_engine_src/BlobXferMgr/BlobInfoDb.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfoDb.h>

#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

namespace
{
	const char * ASSET_INFO_DB_NAME = "AssetBaseInfoDb.db3";

	//============================================================================
    static void * AssetBaseMgrGenHashIdsThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		AssetBaseMgr * poMgr = (AssetBaseMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetInfoMgrStartup( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
AssetBaseMgr::AssetBaseMgr( P2PEngine& engine, const char * dbName, const char * dbStateName, EAssetMgrType assetMgrType )
: m_Engine( engine )
, m_AssetMgrType( assetMgrType )
, m_AssetBaseInfoDb( createAssetInfoDb( dbName, assetMgrType ) )
{
}

//============================================================================
AssetBaseMgr::~AssetBaseMgr()
{
	delete &m_AssetBaseInfoDb;
}

//============================================================================
AssetBaseInfoDb& AssetBaseMgr::createAssetInfoDb( const char * dbName, EAssetMgrType assetMgrType )
{
    switch( assetMgrType )
    {
    case eAssetMgrTypeAssets:
        return *(new AssetInfoDb( *this, dbName ));
    case eAssetMgrTypeBlob:
        return *(new BlobInfoDb( *this, dbName ));
    case eAssetMgrTypeThumb:
        return *(new ThumbInfoDb( *this, dbName ));

    default:
        return *(new AssetInfoDb( *this, dbName ));
    }
}

//============================================================================
void AssetBaseMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_GenHashThread.startThread( (VX_THREAD_FUNCTION_T)AssetBaseMgrGenHashIdsThreadFunc, this, "AssetBaseMgrGenHash" );			
	}
}

//============================================================================
void AssetBaseMgr::assetInfoMgrStartup( VxThread * startupThread )
{
	if( startupThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += ASSET_INFO_DB_NAME; 
	lockResources();
	m_AssetBaseInfoDb.dbShutdown();
	m_AssetBaseInfoDb.dbStartup( 1, dbName );
	unlockResources();
	if( startupThread->isAborted() )
		return;
	updateAssetListFromDb( startupThread );
	m_AssetBaseListInitialized = true;
	if( startupThread->isAborted() )
		return;
	generateHashIds( startupThread );
}

//============================================================================
void AssetBaseMgr::assetInfoMgrShutdown( void )
{
	m_GenHashThread.abortThreadRun( true );
	m_GenHashSemaphore.signal();
	lockResources();
	clearAssetInfoList();
	clearAssetFileListPackets();
	m_AssetBaseInfoDb.dbShutdown();
	unlockResources();
	m_AssetBaseListInitialized = false;
	m_Initialized = false;
}

//============================================================================
void AssetBaseMgr::generateHashForFile( std::string fileName )
{
	m_GenHashMutex.lock();
	m_GenHashList.push_back( fileName );
	m_GenHashMutex.unlock();
	m_GenHashSemaphore.signal();
}

//============================================================================
void AssetBaseMgr::generateHashIds( VxThread * genHashThread )
{
	while( false == genHashThread->isAborted() )
	{
		m_GenHashSemaphore.wait(1000);
		if( genHashThread->isAborted() )
		{
			return;
		}

		while( m_GenHashList.size() )
		{
			if( genHashThread->isAborted() )
			{
				return;
			}

			VxSha1Hash fileHash;
			m_GenHashMutex.lock();
			std::string thisFile = m_GenHashList[0];
			m_GenHashList.erase( m_GenHashList.begin() );
			m_GenHashMutex.unlock();
			if( fileHash.generateHashFromFile( thisFile.c_str(), genHashThread ) )
			{
				if( genHashThread->isAborted() )
				{
					return;
				}

				std::vector<AssetBaseInfo*>::iterator iter;
				AssetBaseInfo * assetInfo = 0;
				lockResources();
				// move from waiting to completed
				for( iter = m_WaitingForHastList.begin(); iter != m_WaitingForHastList.end(); ++iter )
				{
					AssetBaseInfo * inListAssetBaseInfo = *iter;
					if( inListAssetBaseInfo->getAssetName() == thisFile )
					{
						assetInfo = inListAssetBaseInfo;
						m_WaitingForHastList.erase( iter );
						assetInfo->setAssetHashId( fileHash );
						m_AssetBaseInfoList.push_back( assetInfo );
						break;
					}
				}

				unlockResources();
				std::vector<AssetBaseCallbackInterface *>::iterator callbackIter;
				lockClientList();
				for( callbackIter = m_AssetClients.begin(); callbackIter != m_AssetClients.end(); ++callbackIter )
				{
					AssetBaseCallbackInterface * client = *callbackIter;
					client->callbackHashIdGenerated( thisFile, fileHash );
				}

				unlockClientList();
				if( assetInfo )
				{
					m_AssetBaseInfoDb.addAsset( assetInfo );
					announceAssetAdded( assetInfo );
				}
			}
		}
	}
}

//============================================================================
bool AssetBaseMgr::doesAssetExist( AssetBaseInfo& assetInfo ) // check if file still exists in directory or database
{
    if( assetInfo.isDeleted() )
    {
        return false;
    }

    if( assetInfo.hasFileName() )
    {
        if( !( assetInfo.getAssetLength() == VxFileUtil::getFileLen( assetInfo.getAssetName().c_str() ) ) )
        {
            LogMsg( LOG_WARN, "File %s no longer exists for asset %s", assetInfo.getAssetName().c_str(), assetInfo.getAssetUniqueId().toOnlineIdString().c_str() );
            assetInfo.setIsDeleted( true );
            updateDatabase( &assetInfo );
            return false;
        }

        return true;
    }

    // TODO verify exists in database
    return true;
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::findAsset( std::string& fileName )
{
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( (*iter)->getAssetName() == fileName )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::findAsset( VxSha1Hash& fileHashId )
{
	if( false == fileHashId.isHashValid() )
	{
		LogMsg( LOG_ERROR, "AssetBaseMgr::findAsset: invalid file hash id\n" );
		return 0;
	}

	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( (*iter)->getAssetHashId() == fileHashId )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::findAsset( VxGUID& assetId )
{
	if( false == assetId.isVxGUIDValid() )
	{
		//LogMsg( LOG_ERROR, "AssetBaseMgr::findAsset: invalid VxGUID asset id\n" );
        return nullptr;
	}

	std::vector<AssetBaseInfo*>::iterator iter;
	for( AssetBaseInfo * assetInfo : m_AssetBaseInfoList )
	{
		if( assetInfo->getAssetUniqueId() == assetId )
		{
			return assetInfo;
		}
	}

	return nullptr;
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::addAssetFile( EAssetType assetType, const char * fileName, uint64_t fileLen )
{
    AssetBaseInfo * assetInfo = createAssetInfo( assetType, fileName, fileLen );
    if( assetInfo )
    {
        if( insertNewInfo( assetInfo ) )
        {
            return assetInfo;
        }
    }

    return NULL;
}

//============================================================================
bool AssetBaseMgr::addAssetFile(	EAssetType      assetType,
                                    const char *	fileName, 
									VxGUID&			assetId,  
									uint8_t *		hashId, 
									EAssetLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	AssetBaseInfo * assetInfo = createAssetInfo( assetType, fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool AssetBaseMgr::addAssetFile(	EAssetType      assetType,
                                    const char *	fileName, 
									VxGUID&			assetId,  
									VxGUID&		    creatorId, 
									VxGUID&		    historyId, 
									uint8_t *		hashId, 
									EAssetLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	AssetBaseInfo * assetInfo = createAssetInfo( assetType, fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		assetInfo->setCreatorId( creatorId );
		assetInfo->setCreatorId( historyId );
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool AssetBaseMgr::addAsset( AssetBaseInfo& assetInfo )
{
	AssetBaseInfo * newAssetBaseInfo = createAssetInfo( assetInfo );
	LogMsg( LOG_INFO, "AssetBaseMgr::addAsset" );
	return insertNewInfo( newAssetBaseInfo );
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::createAssetInfo( EAssetType assetType, const char * fileName, uint64_t fileLen )
{
    AssetBaseInfo * assetInfo = new AssetBaseInfo( assetType, fileName, fileLen );
    if( assetInfo )
    {
        assetInfo->getAssetUniqueId().initializeWithNewVxGUID();
    }

    return assetInfo;
}

//============================================================================
AssetBaseInfo * AssetBaseMgr::createAssetInfo( 	EAssetType      assetType, 
                                                const char *	fileName, 
										        VxGUID&			assetId,  
										        uint8_t *	    hashId, 
										        EAssetLocation	locationFlags, 
										        const char *	assetTag, 
                                                int64_t			timestamp )
{
	uint64_t  fileLen = VxFileUtil::getFileLen( fileName );
	uint8_t	fileType = VxFileExtensionToFileTypeFlag( fileName );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
		LogMsg( LOG_ERROR, "ERROR %d AssetBaseMgr::createAssetInfo could not get file info %s\n", VxGetLastError(), fileName );
		return NULL;
	}

	AssetBaseInfo * assetInfo = createAssetInfo( assetType, fileName, fileLen );
	assetInfo->setAssetUniqueId( assetId );
	if( false == assetInfo->getAssetUniqueId().isVxGUIDValid() )
	{
		assetInfo->getAssetUniqueId().initializeWithNewVxGUID();
	}

	assetInfo->getAssetHashId().setHashData( hashId );
	assetInfo->setLocationFlags( locationFlags );
	assetInfo->setAssetTag( assetTag );
	assetInfo->setCreationTime( timestamp ? timestamp : GetTimeStampMs() );

	return assetInfo;
}

//============================================================================
bool AssetBaseMgr::insertNewInfo( AssetBaseInfo * assetInfo )
{
	bool result = false;
	AssetBaseInfo * assetInfoExisting = findAsset( assetInfo->getAssetUniqueId() );
	if( assetInfoExisting )
	{
		LogMsg( LOG_ERROR, "ERROR AssetBaseMgr::insertNewInfo: duplicate assset %s\n", assetInfo->getAssetName().c_str() );
		if( assetInfoExisting != assetInfo )
		{
            // update existing asset
			*assetInfoExisting = *assetInfo;
		}
	}

	if( 0 == assetInfo->getCreationTime() )
	{
		assetInfo->setCreationTime( GetTimeStampMs() );
	}

	//if( assetInfo->needsHashGenerated() )
	//{
	//	lockResources();
	//	m_WaitingForHastList.push_back( assetInfo );
	//	unlockResources();
	//	generateHashForFile( assetInfo->getAssetName() );
	//	result = true;
	//}
	//else
	{
        updateDatabase( assetInfo );
		if( !assetInfoExisting )
		{
			lockResources();
			m_AssetBaseInfoList.push_back( assetInfo );
			unlockResources();
			announceAssetAdded( assetInfo );
		}
        else
        {
            announceAssetUpdated( assetInfo );
        }
	
		result = true;
	}

	return result;
}

//============================================================================
bool AssetBaseMgr::updateAsset( AssetBaseInfo& assetInfo )
{
    AssetBaseInfo * existingAsset = findAsset( assetInfo.getAssetUniqueId() );
    if( existingAsset )
    {
        *existingAsset = assetInfo;
        updateDatabase( existingAsset );
        announceAssetUpdated( existingAsset );
        return true;
    }

    return false;
}

//============================================================================
void AssetBaseMgr::announceAssetAdded( AssetBaseInfo * assetInfo )
{
	// LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetAdded start" );
	if( assetInfo->isFileAsset() )
	{
		updateFileListPackets();
		updateAssetFileTypes();
	}
	
	lockClientList();
	std::vector<AssetBaseCallbackInterface *>::iterator iter;
	for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
	{
		AssetBaseCallbackInterface * client = *iter;
		client->callbackAssetAdded( assetInfo );
	}

	unlockClientList();
	// LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetAdded done" );
}

//============================================================================
void AssetBaseMgr::announceAssetUpdated( AssetBaseInfo * assetInfo )
{
    // LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetUpdated start" );
    lockClientList();
    std::vector<AssetBaseCallbackInterface *>::iterator iter;
    for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
    {
        AssetBaseCallbackInterface * client = *iter;
        client->callbackAssetAdded( assetInfo );
    }

    unlockClientList();
    // LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetUpdated done" );
}

//============================================================================
void AssetBaseMgr::announceAssetRemoved( AssetBaseInfo * assetInfo )
{
	//if( assetInfo->getIsAssetBase() )
	{
		updateFileListPackets();
		updateAssetFileTypes();
	}

	lockClientList();
	std::vector<AssetBaseCallbackInterface *>::iterator iter;
	for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
	{
		AssetBaseCallbackInterface * client = *iter;
		client->callbackAssetRemoved( assetInfo );
	}

	unlockClientList();
}

//============================================================================
void AssetBaseMgr::announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<AssetBaseCallbackInterface *>::iterator iter;
	for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
	{
		AssetBaseCallbackInterface * client = *iter;
		client->callbackAssetSendState( assetUniqueId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "AssetBaseMgr::announceAssetXferState state %d done\n", assetSendState );
}

//============================================================================
bool AssetBaseMgr::removeAsset( std::string fileName )
{
	bool assetRemoved = false;
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getAssetName() )
		{
			AssetBaseInfo * assetInfo = *iter;
			m_AssetBaseInfoList.erase( iter );
			m_AssetBaseInfoDb.removeAsset( fileName.c_str() );
			announceAssetRemoved( assetInfo );
			delete assetInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
bool AssetBaseMgr::removeAsset( VxGUID& assetUniqueId )
{
	bool assetRemoved = false;
	std::vector<AssetBaseInfo*>::iterator iter;
	for ( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( assetUniqueId == ( *iter )->getAssetUniqueId() )
		{
			AssetBaseInfo * assetInfo = *iter;
			m_AssetBaseInfoList.erase( iter );
			m_AssetBaseInfoDb.removeAsset( assetInfo );
			announceAssetRemoved( assetInfo );
			delete assetInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
void AssetBaseMgr::clearAssetInfoList( void )
{
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		delete (*iter);
	}

	m_AssetBaseInfoList.clear();
}

//============================================================================
void AssetBaseMgr::updateAssetListFromDb( VxThread * startupThread )
{
	std::vector<AssetBaseInfo*>::iterator iter;
	lockResources();
	clearAssetInfoList();
	m_AssetBaseInfoDb.getAllAssets( m_AssetBaseInfoList );
	bool movedToGenerateHash = true;
	while(	movedToGenerateHash 
			&& ( false == startupThread->isAborted() ) )
	{
		// there should not be any without valid hash but if is then generate it
		movedToGenerateHash = false;
		for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
		{
			AssetBaseInfo* assetInfo = (*iter);
			EAssetSendState sendState = assetInfo->getAssetSendState();
			if( eAssetSendStateTxProgress == sendState ) 
			{
				assetInfo->setAssetSendState( eAssetSendStateTxFail );
				m_AssetBaseInfoDb.updateAssetSendState( assetInfo->getAssetUniqueId(), eAssetSendStateTxFail );
			}
			else if(  eAssetSendStateRxProgress == sendState  )
			{
				assetInfo->setAssetSendState( eAssetSendStateRxFail );
				m_AssetBaseInfoDb.updateAssetSendState( assetInfo->getAssetUniqueId(), eAssetSendStateRxFail );
			}

			if( assetInfo->needsHashGenerated() )
			{
				m_WaitingForHastList.push_back( assetInfo );
				m_AssetBaseInfoList.erase( iter );
				generateHashForFile( assetInfo->getAssetName() );
				movedToGenerateHash = true;
				break;
			}
		}
	}

	unlockResources();
	updateFileListPackets();
	updateAssetFileTypes();
}

//============================================================================
void AssetBaseMgr::updateAssetFileTypes( void )
{
	uint16_t u16FileTypes = 0;
	std::vector<AssetBaseInfo*>::iterator iter;
	lockResources();
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( (*iter)->isFileAsset() )
		{
			u16FileTypes		|= (*iter)->getAssetType();
		}
	}

	unlockResources();
	// ignore extended types
	u16FileTypes = u16FileTypes & 0xff;
	m_u16AssetBaseFileTypes = u16FileTypes;
	bool fileTypesChanged = false;

	m_Engine.lockAnnouncePktAccess();
	PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();
	if( pktAnn.getSharedFileTypes() != u16FileTypes )
	{
		fileTypesChanged = true;
		pktAnn.setSharedFileTypes( (uint8_t)u16FileTypes );
	}

	m_Engine.unlockAnnouncePktAccess();
	if( fileTypesChanged )
	{
		lockClientList();
		std::vector<AssetBaseCallbackInterface *>::iterator iter;
		for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
		{
			AssetBaseCallbackInterface * client = *iter;
			client->callbackAssetFileTypesChanged( u16FileTypes );
		}

		unlockClientList();
	}
}

//============================================================================
void AssetBaseMgr::updateFileListPackets( void )
{
	bool hadAssetBaseFiles = m_FileListPackets.size() ? true : false;
	PktFileListReply * pktFileList = 0;
	clearAssetFileListPackets();
	lockFileListPackets();
	lockResources();
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		AssetBaseInfo * assetInfo = (*iter); 
		if( ( false == assetInfo->isFileAsset() ) || ( false == assetInfo->getAssetHashId().isHashValid() ) )
			continue;

		if( 0 == pktFileList )
		{
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
		}

		if( pktFileList->canAddFile( (uint32_t)(assetInfo->getRemoteAssetName().size() + 1) ) )
		{
			pktFileList->addFile(	assetInfo->getAssetHashId(),
									assetInfo->getAssetLength(),
									assetInfo->getAssetType(),
									assetInfo->getRemoteAssetName().c_str() );
		}
		else
		{
			m_FileListPackets.push_back( pktFileList );
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
			pktFileList->addFile(	assetInfo->getAssetHashId(),
									assetInfo->getAssetLength(),
									assetInfo->getAssetType(),
									assetInfo->getAssetName().c_str() );
		}
	}

	if( 0 != pktFileList )
	{
		if( pktFileList->getFileCount() )
		{
			pktFileList->setIsListCompleted( true ); // last pkt in list
			m_FileListPackets.push_back( pktFileList );
		}
		else
		{
			delete pktFileList;
		}
	}

	unlockResources();
	unlockFileListPackets();
	if( hadAssetBaseFiles || m_FileListPackets.size() )
	{
		lockClientList();
		std::vector<AssetBaseCallbackInterface *>::iterator iter;
		for( iter = m_AssetClients.begin();	iter != m_AssetClients.end(); ++iter )
		{
			AssetBaseCallbackInterface * client = *iter;
			client->callbackAssetPktFileListUpdated();
		}

		unlockClientList();
	}
}

//============================================================================
void AssetBaseMgr::clearAssetFileListPackets( void )
{
	lockFileListPackets();
	std::vector<PktFileListReply*>::iterator iter;
	for( iter = m_FileListPackets.begin(); iter != m_FileListPackets.end(); ++iter )
	{
		delete (*iter);
	}

	m_FileListPackets.clear();
	unlockFileListPackets();
}

//============================================================================
bool AssetBaseMgr::fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId )
{
	lockResources();
	AssetBaseInfo* assetInfo = findAsset( fileName );
	if( assetInfo )
	{
		if( ( false == shareFile ) && assetInfo->isSharedFileAsset() )
		{
			assetInfo->setIsSharedFileAsset( false );
			updateDatabase( assetInfo );
			unlockResources();
			updateAssetFileTypes();
			updateFileListPackets();
			return true;
		}
	}

	unlockResources();

	if( shareFile )
	{
		// file is not currently AssetBase and should be
		VxGUID guid;
		AssetBaseInfo * assetInfo = createAssetInfo( VxFileNameToAssetType( fileName ), fileName.c_str(), guid, fileHashId, eAssetLocShared );
		if( assetInfo )
		{
			insertNewInfo( assetInfo );
		}
	}
	else
	{
		return false;
	}

	return true;
}

//============================================================================
bool AssetBaseMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
{
	bool foundHash = false;
	lockResources();
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( fileFullName == (*iter)->getAssetName() )
		{
			retFileHashId = (*iter)->getAssetHashId();
			foundHash = retFileHashId.isHashValid();
			break;
		}
	}

	unlockResources();
	return foundHash;
}

//============================================================================
bool AssetBaseMgr::getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName )
{
	bool isAssetBase = false;
	lockResources();
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		if( fileHashId == (*iter)->getAssetHashId() )
		{
			isAssetBase = true;
			retFileFullName = (*iter)->getAssetName();
			break;
		}
	}

	unlockResources();
	return isAssetBase;
}


//============================================================================
bool AssetBaseMgr::fromGuiGetAssetBaseInfo( uint8_t fileTypeFilter )
{
	lockResources();
	std::vector<AssetBaseInfo*>::iterator iter;
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		AssetBaseInfo* assetInfo = (*iter);
		if( 0 != ( fileTypeFilter & assetInfo->getAssetType() ) )
		{
			if( assetInfo->isSharedFileAsset() || assetInfo->isInLibary() )
			{
				IToGui::getToGui().toGuiFileList(	assetInfo->getAssetName().c_str(), 
										            assetInfo->getAssetLength(), 
										            assetInfo->getAssetType(), 
										            assetInfo->isSharedFileAsset(),
										            assetInfo->isInLibary(),
										            assetInfo->getAssetHashId().getHashData() );
			}
		}
	}

	unlockResources();
	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false );
	return true;
}

//============================================================================
void AssetBaseMgr::updateDatabase( AssetBaseInfo * assetInfo )
{
	m_AssetBaseInfoDb.addAsset( assetInfo );
}

//============================================================================
void AssetBaseMgr::updateAssetDatabaseSendState( VxGUID& assetUniqueId, EAssetSendState sendState )
{
	m_AssetBaseInfoDb.updateAssetSendState( assetUniqueId, sendState );
}

//============================================================================
void AssetBaseMgr::queryHistoryAssets( VxGUID& historyId )
{
	std::vector<AssetBaseInfo*>::iterator iter;
	AssetBaseInfo * assetInfo;
	lockResources();
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getHistoryId() == historyId )
		{
            onQueryHistoryAsset( assetInfo );
			// BRJ IToGui::getToGui().toGuiAssetSessionHistory( assetInfo );
		}
	}

	unlockResources();
}

//============================================================================
void AssetBaseMgr::updateAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "AssetBaseMgr::updateAssetXferState state %d start\n", assetSendState );
	std::vector<AssetBaseInfo*>::iterator iter;
	AssetBaseInfo* assetInfo;
	bool assetSendStateChanged = false;
	lockResources();
	for( iter = m_AssetBaseInfoList.begin(); iter != m_AssetBaseInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getAssetUniqueId() == assetUniqueId )
		{
			EAssetSendState oldSendState = assetInfo->getAssetSendState();
			if( oldSendState != assetSendState )
			{
				assetInfo->setAssetSendState( assetSendState );
				assetSendStateChanged = true;
				//updateDatabase( assetInfo );
				updateAssetDatabaseSendState( assetUniqueId, assetSendState );
				switch( assetSendState )
				{
				case eAssetSendStateTxProgress:
					if( eAssetSendStateNone == oldSendState )
					{
						IToGui::getToGui().toGuiAssetAction( eAssetActionTxBegin, assetUniqueId, param );
					}

					IToGui::getToGui().toGuiAssetAction( eAssetActionTxProgress, assetUniqueId, param );
					break;

				case eAssetSendStateRxProgress:
					if( eAssetSendStateNone == oldSendState )
					{
						IToGui::getToGui().toGuiAssetAction( eAssetActionRxBegin, assetUniqueId, param );
					}

					IToGui::getToGui().toGuiAssetAction( eAssetActionRxProgress, assetUniqueId, param );
					break;

				case eAssetSendStateRxSuccess:
					if( ( eAssetSendStateNone == oldSendState )
						|| ( eAssetSendStateRxProgress == oldSendState ) )
					{
						IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, assetUniqueId, param );
						IToGui::getToGui().toGuiAssetAction( eAssetActionRxNotifyNewMsg, assetInfo->getCreatorId(), 100 );
					}
					else 
					{
						IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, assetUniqueId, param );
					}

					break;

				case eAssetSendStateTxSuccess:
					IToGui::getToGui().toGuiAssetAction( eAssetActionTxSuccess, assetUniqueId, param );
					break;

				case eAssetSendStateRxFail:
					IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, assetUniqueId, param );
					break;

				case eAssetSendStateTxFail:
					IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, assetUniqueId, param );
					break;

				case eAssetSendStateTxPermissionErr:
					IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, assetUniqueId, param );
					break;

				case eAssetSendStateRxPermissionErr:
					IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, assetUniqueId, param );
					break;


				case eAssetSendStateNone:
				default:
					break;
				}
			}

			break;
		}
	}

	unlockResources();
	if( assetSendStateChanged )
	{
		announceAssetXferState( assetUniqueId, assetSendState, param );
	}

	LogMsg( LOG_INFO, "AssetBaseMgr::updateAssetXferState state %d done\n", assetSendState );
}


//============================================================================
bool AssetBaseMgr::isAllowedFileOrDir( std::string strFileName )
{
    if( VxIsExecutableFile( strFileName ) 
        || VxIsShortcutFile( strFileName ) )
    {
        return false;
    }

    return true;
}

//============================================================================
void AssetBaseMgr::addAssetMgrClient( AssetBaseCallbackInterface * client, bool enable )
{
    AutoResourceLock( this );
    if( enable )
    {
        m_AssetClients.push_back( client );
    }
    else
    {
        std::vector<AssetBaseCallbackInterface *>::iterator iter;
        for( iter = m_AssetClients.begin(); iter != m_AssetClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_AssetClients.erase( iter );
                return;
            }
        }
    }
}


/*
//============================================================================
RCODE AssetBaseMgr::SendMatchList(	uint32_t			u32SktNum,		// Tronacom socket number
									CString &	csMatchString,	// Search Match name
									uint16_t			u16FileTypes,	// types of files to match
									uint16_t			u16LimitType,	// file size limit type
									uint64_t			u64SizeLimit,	// file size limit
									bool		bCloseAfterSend,// if true close the socket after send
									bool		bEncrypt )		// if true encrypt
{	
	int i;
	RCODE rc = 0;

	PktShareFileList gPktList;
	char as8FileName[ VX_MAX_PATH ];
	char as8TmpFileName[ VX_MAX_PATH ];
	long s32PktIdx = 0;
	unsigned long u32FileLen = 0;
	bool bFileOpen = false;
	FILE * pgFile = 0;
	CStringArray acsMatchTokens;
	bool bSendAll = false;
	ShareFileInfo * poInfo;

	if( 1 == csMatchString.GetLength() && 
		'*' == csMatchString[ 0 ] )
	{
		bSendAll = true; //send complete list
	}
	else
	{

		rc = ParseSearchString(	csMatchString,		//String of search tokens
							acsMatchTokens );	//return seperate token in each String
	}
	if( rc )
	{
		LogMsg( LOG_INFO, "AssetBaseMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
		ASSERT( false );
	}
	else
	{
		int	iCnt = m_List.GetSize();
		for( i = 0; i < iCnt; i++ )
		{
			poInfo = &m_List[ i ];
			if( 0 == ( u16FileTypes & poInfo->m_u32Flags ) )
			{
				//is not the file type we are searching for
				continue;
			}
			if( bSendAll ||
				Match( acsMatchTokens, poInfo->m_csDisplayName ) )
			{
				if( 0 == poInfo->m_s64FileLen )
				{
					LogMsg( LOG_INFO, "AssetBaseMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
					continue;
				}
				//add file to PktFileList
				unsigned char * pu8Temp = poInfo->m_pu8ChoppedDisplayName;
				unsigned long u32ItemLen = * pu8Temp + sizeof( __int64 );
				if( gPktList.m_u16PktLen + u32ItemLen > sizeof( PktShareFileList ) )
				{
					//wont fit in this Pkt so store and start another
					gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
					if( false == bFileOpen )
					{	
						tmpnam( as8TmpFileName );
						strcpy( as8FileName, g_oGlobals.m_csExeDir );
						strcat( as8FileName, "TcFileShareRes" );
						strcat( as8FileName, as8TmpFileName );
						strcat( as8FileName, "tmp" );
						pgFile = fopen( as8FileName, "wb+" );
						if( 0 == pgFile )
						{
							rc = FILE_OPEN_ERR;
							break;
						}
						bFileOpen = true;
					}

					if( (unsigned long)gPktList.m_u16PktLen != fwrite( &gPktList, 
																		1, 
																		(unsigned long)gPktList.m_u16PktLen, 
																		pgFile ) )
					{
						rc = FILE_WRITE_ERR;
					}
					u32FileLen += gPktList.m_u16PktLen;
					gPktList.m_u16PktLen = gPktList.emptyLen();
					s32PktIdx = 0;
					gPktList.m_u16ListCnt = 0;
				}
				//copy file length first
				*( ( __int64 *)&gPktList.m_au8List[ s32PktIdx ] ) = poInfo->m_s64FileLen;
				//copy chopped string that has display name
				memcpy( &gPktList.m_au8List[ s32PktIdx + sizeof( __int64 ) ], poInfo->m_pu8ChoppedDisplayName, u32ItemLen - sizeof( __int64 ) );

				//show trace of file we added to list
				char as8Buf[ 256 * 2 ];
				VxUnchopStr( poInfo->m_pu8ChoppedDisplayName, as8Buf );
				LogMsg( LOG_INFO, "Added to PktFileList file size %d file %s\n", 
						*( ( unsigned long *)&gPktList.m_au8List[ s32PktIdx ] ),
						as8Buf );


				//increment vars for next item
				s32PktIdx += u32ItemLen;
				gPktList.m_u16ListCnt++;
				gPktList.m_u16PktLen += (unsigned short)u32ItemLen;
			}	
		}
	}
	if( pgFile )
	{
		if( u32FileLen )
		{
			//we allready have at least one pkt in file
			//so add this one if has data
			if( s32PktIdx )
			{
				//we have a pkt with data
				gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
			
				//flush Pkt to file
				if( (unsigned long)gPktList.m_u16PktLen != fwrite( &gPktList, 
																	1, 
																	(unsigned long)gPktList.m_u16PktLen, 
																	pgFile ) )
				{
					rc = FILE_WRITE_ERR;
				}
				u32FileLen += gPktList.m_u16PktLen;
				if( u32FileLen & 0x0f )
				{
					//file length is not on 16 byte boundery so fill in a little
					if( 16 - (u32FileLen & 0x0f) != fwrite( &gPktList, 
																		16 - (u32FileLen & 0x0f),
																		(unsigned long)gPktList.m_u16PktLen, 
																		pgFile ) )
					{
						rc = FILE_WRITE_ERR;
					}
				}
				fclose( pgFile );
			}
			//use threaded file send to send list
			//this uses a separate thread to send file containing packets
			rc = g_poApi->SktSendFile(	u32SktNum,// Socket to send on
							as8FileName,	// packet to be sent via tcp connection
							0,	//offset into file to start at
							4096, //size of pkt chunks
							-2,	// PktType to put in header 	
								// positive value..add header of u16PktType to each chunk sent
								//  -1 = use no headers..send file in chunks of u32PktSize in length
								//	-2 = file is file of packets add value in size of pkt chunks to pkt types
							0, //instance value if has header
							true,		// if true delete file after it is sent
							true,		// if true disconnect after send
							0, //user data for callback
							NULL );		// callback from thread with File Send status
		}
	}
	else
	{
		//we havent even written one pkt to file so
		//just send this one
		gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
 
		g_poApi->SktSend( u32SktNum, &gPktList, bCloseAfterSend );
	}
	return rc;
}

//============================================================================
RCODE AssetBaseMgr::AddDir( CString & csDirPath )
{
	HRESULT hr;
	char as8FullPath[ VX_MAX_PATH ];

	struct _stat gStat;
	int i;


    CArray<CString, CString&> acsWildNameList;//Extensions ( file extentions )
	acsWildNameList.Add( CString( "*.*" ) );
	CArray<AssetBaseInfo, AssetBaseInfo&> agAssetBaseInfoList;//return FileInfo in array

	VxFindFilesByName(	csDirPath,				//start path to search in
						acsWildNameList,//Extensions ( file extentions )
						agAssetBaseInfoList,//return FileInfo in array
						true,						//recurse subdirectories if TRUE
						false		//if TRUE dont return files matching filter else return files that do
 						);
	int iCnt = agAssetBaseInfoList.GetSize();
	//expand list to include the new files
	int iCurListCnt = m_List.GetSize();
	m_List.SetSize( iCnt + iCurListCnt );
	int iResolvedListIdx = iCurListCnt;

	for( i = 0; i < iCnt; i++ )
	{
		CString cs = agAssetBaseInfoList[ i ];
		__int64 s64FileLen = agAssetBaseInfoList[ i ].m_s64Len;
		makeForwardSlashPath( cs );

		//get file extension
		CString csExt = getFileExtension( cs );
		char * pExt = (char *)((const char *)csExt);
		bool bIsLink = false;

		if( 0 == strcmp( pExt, "lnk" ) )
		{ 
			bIsLink = true; 
			as8FullPath[ 0 ] = 0;
			hr = VxResolveShortcut(	0,				//handle to window of caller
									cs,	//.lnk file
									as8FullPath );		//return path to target file
			if( FAILED( hr ) )
			{
				TRACE("Unable to resolve path in %s\n", (const char *)cs );
				continue;
			}
			else
			{
				cs = as8FullPath;	
				int iResult = ::_stat( (const char *)cs, &gStat );
				if( iResult != 0 )
				{
					TRACE( "Problem getting file information\n" );
					continue;
				}
				else
				{
					s64FileLen = gStat.st_size;
					csExt = getFileExtension( cs );
					pExt = (char *)((const char *)csExt);
				}
			}
		}
		//weve got a live one so

		unsigned long u32FileFlags = 0;

		if( VxIsPhotoFileExtention( pExt ) )
		{
			m_bHasImage = true;
			u32FileFlags |= FILE_TYPE_PHOTO;
			m_u32FileFlags |= FILE_TYPE_PHOTO;
		}
		else if( VxIsAudioFileExtention( pExt ) )
		{
			m_bHasAudio = true;
			u32FileFlags |= FILE_TYPE_AUDIO;
			m_u32FileFlags |= FILE_TYPE_AUDIO; 
		}
		else if( VxIsVideoFileExtention( pExt ) )
		{
			m_bHasVideo = true;
			u32FileFlags |= FILE_TYPE_VIDEO;
			m_u32FileFlags |= FILE_TYPE_VIDEO; 
		}
		else if( VxIsExecutableFileExtention( pExt ) )
		{
			m_bHasExecutable = true;
			u32FileFlags |= FILE_TYPE_EXECUTABLE;
			m_u32FileFlags |= FILE_TYPE_EXECUTABLE;
		}
		else if( VxIsArchiveOrDocFileExtention( pExt ) )
		{
			m_bHasArchiveOrDoc = true;
			u32FileFlags |= FILE_TYPE_ARCHIVE_OR_DOC;
			m_u32FileFlags |= FILE_TYPE_ARCHIVE_OR_DOC;
		}
		else if( VxIsArchiveOrDocFileExtention( pExt ) )
		{
			m_bHasCdImage = true;
			u32FileFlags |= FILE_TYPE_CD_IMAGE;
			m_u32FileFlags |= FILE_TYPE_CD_IMAGE;
		}
		else
		{
			m_bHasOther = true;
			u32FileFlags |= FILE_TYPE_OTHER;
			m_u32FileFlags |= FILE_TYPE_OTHER;
		}
		//fill in the data
		m_s64TotalByteCnt += s64FileLen;
		ShareFileInfo * poInfo = &m_List[ iResolvedListIdx ];
		poInfo->m_csPathAndName = cs;
		poInfo->m_s64FileLen = s64FileLen;
		poInfo->m_u32Flags = u32FileFlags;
		if( bIsLink )
		{
			//display name is the file name
			poInfo->m_csDisplayName = VxGetFileNameFromFullPath( cs );
		}
		else
		{
			//display name is full path - root path
			char * pTemp = (char *)(const char *)cs;
			int iRootLen =  csDirPath.GetLength();
			ASSERT( iRootLen < cs.GetLength() );
			if( iRootLen )
			{
				if( '\\' == pTemp[ iRootLen - 1 ] )
				{
					//root path has the slash
					poInfo->m_csDisplayName = &pTemp[ iRootLen ];
				}
				else
				{
					//root path doesnt has the slash so add 1 to length
					poInfo->m_csDisplayName = &pTemp[ iRootLen + 1 ];
				}

			}
		}

		unsigned char * pu8ChoppedStr = new unsigned char[ poInfo->m_csDisplayName.GetLength() + 10 ];
		VxChopStr( (char *)(const char *) poInfo->m_csDisplayName, pu8ChoppedStr );
		if( poInfo->m_pu8ChoppedDisplayName )
		{
			delete poInfo->m_pu8ChoppedDisplayName;
		}
		poInfo->m_pu8ChoppedDisplayName = pu8ChoppedStr;
		LogChoppedString( pu8ChoppedStr );
		poInfo = &m_List[0];
		const char * pTemp = (const char *)poInfo->m_csDisplayName;
		
		iResolvedListIdx++;

	}
	m_List.SetSize( iResolvedListIdx );//remove any unused entries
	return 0;
}
//============================================================================
//! make array of packets with matching files
RCODE AssetBaseMgr::MakeMatchList(	CPtrArray * paoRetListPkts, // return list of packets of file lists
									CString &	csMatchString,	// Search Match name
									uint16_t			u16FileTypes,	// types of files to match
									uint16_t			u16LimitType,	// file size limit type
									uint64_t			u64SizeLimit )	// file size limit
{	
	int i;
	RCODE rc = 0;

	PktShareFileList gPktList;
	long s32PktIdx = 0;
	unsigned long u32FileLen = 0;
	bool bFileOpen = false;
	FILE * pgFile = 0;
	CStringArray acsMatchTokens;
	bool bSendAll = false;
	ShareFileInfo * poInfo;

	if( 1 == csMatchString.GetLength() && 
		'*' == csMatchString[ 0 ] )
	{
		bSendAll = true; //send complete list
	}
	else
	{

		rc = ParseSearchString(	csMatchString,		//String of search tokens
							acsMatchTokens );	//return seperate token in each String
	}
	if( rc )
	{
		LogMsg( LOG_INFO, "AssetBaseMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
		ASSERT( false );
	}
	else
	{
		int	iCnt = m_List.GetSize();
		for( i = 0; i < iCnt; i++ )
		{
			poInfo = &m_List[ i ];
			if( 0 == ( u16FileTypes & poInfo->m_u32Flags ) )
			{
				//is not the file type we are searching for
				continue;
			}
			if( bSendAll ||
				Match( acsMatchTokens, poInfo->m_csDisplayName ) )
			{
				if( 0 == poInfo->m_s64FileLen )
				{
					LogMsg( LOG_INFO, "AssetBaseMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
					continue;
				}
				//add file to PktFileList
				unsigned char * pu8Temp = poInfo->m_pu8ChoppedDisplayName;
				unsigned long u32ItemLen = * pu8Temp + sizeof( __int64 );
				if( gPktList.m_u16PktLen + u32ItemLen > sizeof( PktShareFileList ) )
				{
					//wont fit in this Pkt so store and start another
					gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
					u32FileLen += gPktList.m_u16PktLen;
					
					paoRetListPkts->Add( gPktList.makeCopy() );

					gPktList.m_u16PktLen = gPktList.emptyLen();
					s32PktIdx = 0;
					gPktList.m_u16ListCnt = 0;
				}
				//copy file length first
				*( ( __int64 *)&gPktList.m_au8List[ s32PktIdx ] ) = poInfo->m_s64FileLen;
				//copy chopped string that has display name
				memcpy( &gPktList.m_au8List[ s32PktIdx + sizeof( __int64 ) ], poInfo->m_pu8ChoppedDisplayName, u32ItemLen - sizeof( __int64 ) );

				//show trace of file we added to list
				char as8Buf[ 256 * 2 ];
				VxUnchopStr( poInfo->m_pu8ChoppedDisplayName, as8Buf );
				LogMsg( LOG_INFO, "Added to PktFileList file size %d file %s\n", 
						*( ( unsigned long *)&gPktList.m_au8List[ s32PktIdx ] ),
						as8Buf );


				//increment vars for next item
				s32PktIdx += u32ItemLen;
				gPktList.m_u16ListCnt++;
				gPktList.m_u16PktLen += (unsigned short)u32ItemLen;
			}	
		}
	}
	gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
	paoRetListPkts->Add( gPktList.makeCopy() );
 	return 0;
}
*/
