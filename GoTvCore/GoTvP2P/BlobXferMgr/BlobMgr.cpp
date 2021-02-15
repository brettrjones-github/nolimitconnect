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

#include "BlobMgr.h"
#include "BlobInfo.h"
#include "BlobInfoDb.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

namespace
{
	const char * ASSET_INFO_DB_NAME = "BlobInfoDb.db3";

	//============================================================================
    static void * BlobMgrGenHashIdsThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		BlobMgr * poMgr = (BlobMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetInfoMgrStartup( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
BlobMgr::BlobMgr( P2PEngine& engine )
: BlobMgrBase( engine )
, m_Initialized( false )
, m_BlobListInitialized( false )
, m_BlobInfoDb( * new BlobInfoDb( *this ) )
{
}

//============================================================================
BlobMgr::~BlobMgr()
{
	delete &m_BlobInfoDb;
}

//============================================================================
void BlobMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_GenHashThread.startThread( (VX_THREAD_FUNCTION_T)BlobMgrGenHashIdsThreadFunc, this, "BlobMgrGenHash" );			
	}
}

//============================================================================
void BlobMgr::assetInfoMgrStartup( VxThread * startupThread )
{
	if( startupThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += ASSET_INFO_DB_NAME; 
	lockResources();
	m_BlobInfoDb.dbShutdown();
	m_BlobInfoDb.dbStartup( 1, dbName );
	unlockResources();
	if( startupThread->isAborted() )
		return;
	updateBlobListFromDb( startupThread );
	m_BlobListInitialized = true;
	if( startupThread->isAborted() )
		return;
	generateHashIds( startupThread );
}

//============================================================================
void BlobMgr::assetInfoMgrShutdown( void )
{
	m_GenHashThread.abortThreadRun( true );
	m_GenHashSemaphore.signal();
	lockResources();
	clearBlobInfoList();
	clearBlobFileListPackets();
	m_BlobInfoDb.dbShutdown();
	unlockResources();
	m_BlobListInitialized = false;
	m_Initialized = false;
}

//============================================================================
void BlobMgr::generateHashForFile( std::string fileName )
{
	m_GenHashMutex.lock();
	m_GenHashList.push_back( fileName );
	m_GenHashMutex.unlock();
	m_GenHashSemaphore.signal();
}

//============================================================================
void BlobMgr::generateHashIds( VxThread * genHashThread )
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

				std::vector<BlobInfo*>::iterator iter;
				BlobInfo * assetInfo = 0;
				lockResources();
				// move from waiting to completed
				for( iter = m_WaitingForHastList.begin(); iter != m_WaitingForHastList.end(); ++iter )
				{
					BlobInfo * inListBlobInfo = *iter;
					if( inListBlobInfo->getBlobName() == thisFile )
					{
						assetInfo = inListBlobInfo;
						m_WaitingForHastList.erase( iter );
						assetInfo->setBlobHashId( fileHash );
						m_BlobInfoList.push_back( assetInfo );
						break;
					}
				}

				unlockResources();
				std::vector<BlobCallbackInterface *>::iterator callbackIter;
				lockClientList();
				for( callbackIter = m_BlobClients.begin(); callbackIter != m_BlobClients.end(); ++callbackIter )
				{
					BlobCallbackInterface * client = *callbackIter;
					client->callbackHashIdGenerated( thisFile, fileHash );
				}

				unlockClientList();
				if( assetInfo )
				{
					m_BlobInfoDb.addBlob( assetInfo );
					announceBlobAdded( assetInfo );
				}
			}
		}
	}
}

//============================================================================
BlobInfo * BlobMgr::findBlob( std::string& fileName )
{
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( (*iter)->getBlobName() == fileName )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
BlobInfo * BlobMgr::findBlob( VxSha1Hash& fileHashId )
{
	if( false == fileHashId.isHashValid() )
	{
		LogMsg( LOG_ERROR, "BlobMgr::findBlob: invalid file hash id\n" );
		return 0;
	}

	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( (*iter)->getBlobHashId() == fileHashId )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
BlobInfo * BlobMgr::findBlob( VxGUID& assetId )
{
	if( false == assetId.isVxGUIDValid() )
	{
		//LogMsg( LOG_ERROR, "BlobMgr::findBlob: invalid VxGUID asset id\n" );
        return nullptr;
	}

	std::vector<BlobInfo*>::iterator iter;
	for( BlobInfo * assetInfo : m_BlobInfoList )
	{
		if( assetInfo->getBlobUniqueId() == assetId )
		{
			return assetInfo;
		}
	}

	return nullptr;
}

//============================================================================
BlobInfo * BlobMgr::addBlobFile( const char * fileName, uint64_t fileLen, uint8_t fileType )
{
    BlobInfo * assetInfo = createBlobInfo( fileName, fileLen, fileType );
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
bool BlobMgr::addBlobFile(	    const char *	fileName, 
									VxGUID&			assetId,  
									uint8_t *		hashId, 
									EBlobLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	BlobInfo * assetInfo = createBlobInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool BlobMgr::addBlobFile(	    const char *	fileName, 
									VxGUID&			assetId,  
									VxGUID&		    creatorId, 
									VxGUID&		    historyId, 
									uint8_t *		hashId, 
									EBlobLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	BlobInfo * assetInfo = createBlobInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		assetInfo->setCreatorId( creatorId );
		assetInfo->setCreatorId( historyId );
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool BlobMgr::addBlob( BlobInfo& assetInfo )
{
	BlobInfo * newBlobInfo = new BlobInfo( assetInfo );
	LogMsg( LOG_INFO, "BlobMgr::addBlob\n" );
	return insertNewInfo( newBlobInfo );
}

//============================================================================
BlobInfo * BlobMgr::createBlobInfo( const char * fileName, uint64_t fileLen, uint8_t fileType )
{
    BlobInfo * assetInfo = new BlobInfo( fileName, fileLen, fileType );
    if( assetInfo )
    {
        assetInfo->getBlobUniqueId().initializeWithNewVxGUID();
    }

    return assetInfo;
}

//============================================================================
BlobInfo * BlobMgr::createBlobInfo( 	const char *	fileName, 
										VxGUID&			assetId,  
										uint8_t *	    hashId, 
										EBlobLocation	locationFlags, 
										const char *	assetTag, 
                                        int64_t			timestamp )
{
	uint64_t  fileLen = VxFileUtil::getFileLen( fileName );
	uint8_t	fileType = VxFileExtensionToFileTypeFlag( fileName );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
		LogMsg( LOG_ERROR, "ERROR %d BlobMgr::createBlobInfo could not get file info %s\n", VxGetLastError(), fileName );
		return NULL;
	}

	BlobInfo * assetInfo = new BlobInfo( fileName, fileLen, fileType );
	assetInfo->setBlobUniqueId( assetId );
	if( false == assetInfo->getBlobUniqueId().isVxGUIDValid() )
	{
		assetInfo->getBlobUniqueId().initializeWithNewVxGUID();
	}

	assetInfo->getBlobHashId().setHashData( hashId );
	assetInfo->setLocationFlags( locationFlags );
	assetInfo->setBlobTag( assetTag );
	assetInfo->setCreationTime( timestamp ? timestamp : GetTimeStampMs() );

	return assetInfo;
}

//============================================================================
bool BlobMgr::insertNewInfo( BlobInfo * assetInfo )
{
	bool result = false;
	BlobInfo * assetInfoExisting = findBlob( assetInfo->getBlobUniqueId() );
	if( assetInfoExisting )
	{
		LogMsg( LOG_ERROR, "ERROR BlobMgr::insertNewInfo: duplicate assset %s\n", assetInfo->getBlobName().c_str() );
		if( assetInfoExisting != assetInfo )
		{
			*assetInfoExisting = *assetInfo;
			assetInfo = assetInfoExisting;
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
	//	generateHashForFile( assetInfo->getBlobName() );
	//	result = true;
	//}
	//else
	//{
		if( 0 == assetInfoExisting )
		{
			lockResources();
			m_BlobInfoList.push_back( assetInfo );
			unlockResources();
			announceBlobAdded( assetInfo );
		}

		updateDatabase( assetInfo );
		result = true;
	//}

	return result;
}

//============================================================================
void BlobMgr::announceBlobAdded( BlobInfo * assetInfo )
{
	LogMsg( LOG_INFO, "BlobMgr::announceBlobAdded start\n" );
	if( assetInfo->getIsFileBlob() )
	{
		updateFileListPackets();
		updateBlobFileTypes();
	}
	
	lockClientList();
	std::vector<BlobCallbackInterface *>::iterator iter;
	for( iter = m_BlobClients.begin();	iter != m_BlobClients.end(); ++iter )
	{
		BlobCallbackInterface * client = *iter;
		client->callbackBlobAdded( assetInfo );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "BlobMgr::announceBlobAdded done\n" );
}

//============================================================================
void BlobMgr::announceBlobRemoved( BlobInfo * assetInfo )
{
	//if( assetInfo->getIsBlob() )
	{
		updateFileListPackets();
		updateBlobFileTypes();
	}

	lockClientList();
	std::vector<BlobCallbackInterface *>::iterator iter;
	for( iter = m_BlobClients.begin();	iter != m_BlobClients.end(); ++iter )
	{
		BlobCallbackInterface * client = *iter;
		client->callbackBlobRemoved( assetInfo );
	}

	unlockClientList();
}

//============================================================================
void BlobMgr::announceBlobXferState( VxGUID& assetUniqueId, EBlobSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "BlobMgr::announceBlobXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<BlobCallbackInterface *>::iterator iter;
	for( iter = m_BlobClients.begin();	iter != m_BlobClients.end(); ++iter )
	{
		BlobCallbackInterface * client = *iter;
		client->callbackBlobSendState( assetUniqueId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "BlobMgr::announceBlobXferState state %d done\n", assetSendState );
}

//============================================================================
bool BlobMgr::removeBlob( std::string fileName )
{
	bool assetRemoved = false;
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getBlobName() )
		{
			BlobInfo * assetInfo = *iter;
			m_BlobInfoList.erase( iter );
			m_BlobInfoDb.removeBlob( fileName.c_str() );
			announceBlobRemoved( assetInfo );
			delete assetInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
bool BlobMgr::removeBlob( VxGUID& assetUniqueId )
{
	bool assetRemoved = false;
	std::vector<BlobInfo*>::iterator iter;
	for ( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( assetUniqueId == ( *iter )->getBlobUniqueId() )
		{
			BlobInfo * assetInfo = *iter;
			m_BlobInfoList.erase( iter );
			m_BlobInfoDb.removeBlob( assetInfo );
			announceBlobRemoved( assetInfo );
			delete assetInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
void BlobMgr::clearBlobInfoList( void )
{
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		delete (*iter);
	}

	m_BlobInfoList.clear();
}

//============================================================================
void BlobMgr::updateBlobListFromDb( VxThread * startupThread )
{
	std::vector<BlobInfo*>::iterator iter;
	lockResources();
	clearBlobInfoList();
	m_BlobInfoDb.getAllBlobs( m_BlobInfoList );
	bool movedToGenerateHash = true;
	while(	movedToGenerateHash 
			&& ( false == startupThread->isAborted() ) )
	{
		// there should not be any without valid hash but if is then generate it
		movedToGenerateHash = false;
		for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
		{
			BlobInfo* assetInfo = (*iter);
			EBlobSendState sendState = assetInfo->getBlobSendState();
			if( eBlobSendStateTxProgress == sendState ) 
			{
				assetInfo->setBlobSendState( eBlobSendStateTxFail );
				m_BlobInfoDb.updateBlobSendState( assetInfo->getBlobUniqueId(), eBlobSendStateTxFail );
			}
			else if(  eBlobSendStateRxProgress == sendState  )
			{
				assetInfo->setBlobSendState( eBlobSendStateRxFail );
				m_BlobInfoDb.updateBlobSendState( assetInfo->getBlobUniqueId(), eBlobSendStateRxFail );
			}

			if( assetInfo->needsHashGenerated() )
			{
				m_WaitingForHastList.push_back( assetInfo );
				m_BlobInfoList.erase( iter );
				generateHashForFile( assetInfo->getBlobName() );
				movedToGenerateHash = true;
				break;
			}
		}
	}

	unlockResources();
	updateFileListPackets();
	updateBlobFileTypes();
}

//============================================================================
void BlobMgr::updateBlobFileTypes( void )
{
	uint16_t u16FileTypes = 0;
	std::vector<BlobInfo*>::iterator iter;
	lockResources();
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( (*iter)->getIsFileBlob() )
		{
			u16FileTypes		|= (*iter)->getBlobType();
		}
	}

	unlockResources();
	// ignore extended types
	u16FileTypes = u16FileTypes & 0xff;
	m_u16BlobFileTypes = u16FileTypes;
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
		std::vector<BlobCallbackInterface *>::iterator iter;
		for( iter = m_BlobClients.begin();	iter != m_BlobClients.end(); ++iter )
		{
			BlobCallbackInterface * client = *iter;
			client->callbackBlobFileTypesChanged( u16FileTypes );
		}

		unlockClientList();
	}
}

//============================================================================
void BlobMgr::updateFileListPackets( void )
{
	bool hadBlobFiles = m_FileListPackets.size() ? true : false;
	PktFileListReply * pktFileList = 0;
	clearBlobFileListPackets();
	lockFileListPackets();
	lockResources();
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		BlobInfo * assetInfo = (*iter); 
		if( ( false == assetInfo->getIsFileBlob() ) || ( false == assetInfo->getBlobHashId().isHashValid() ) )
			continue;

		if( 0 == pktFileList )
		{
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
		}

		if( pktFileList->canAddFile( (uint32_t)(assetInfo->getRemoteBlobName().size() + 1) ) )
		{
			pktFileList->addFile(	assetInfo->getBlobHashId(),
									assetInfo->getBlobLength(),
									assetInfo->getBlobType(),
									assetInfo->getRemoteBlobName().c_str() );
		}
		else
		{
			m_FileListPackets.push_back( pktFileList );
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
			pktFileList->addFile(	assetInfo->getBlobHashId(),
									assetInfo->getBlobLength(),
									assetInfo->getBlobType(),
									assetInfo->getBlobName().c_str() );
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
	if( hadBlobFiles || m_FileListPackets.size() )
	{
		lockClientList();
		std::vector<BlobCallbackInterface *>::iterator iter;
		for( iter = m_BlobClients.begin();	iter != m_BlobClients.end(); ++iter )
		{
			BlobCallbackInterface * client = *iter;
			client->callbackBlobPktFileListUpdated();
		}

		unlockClientList();
	}
}

//============================================================================
void BlobMgr::clearBlobFileListPackets( void )
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
bool BlobMgr::fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId )
{
	lockResources();
	BlobInfo* assetInfo = findBlob( fileName );
	if( assetInfo )
	{
		if( ( false == shareFile ) && assetInfo->getIsSharedFileBlob() )
		{
			assetInfo->setIsSharedFileBlob( false );
			updateDatabase( assetInfo );
			unlockResources();
			updateBlobFileTypes();
			updateFileListPackets();
			return true;
		}
	}

	unlockResources();

	if( shareFile )
	{
		// file is not currently Blob and should be
		VxGUID guid;
		BlobInfo * assetInfo = createBlobInfo( fileName.c_str(), guid, fileHashId, eBlobLocShared );
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

/*
//============================================================================
bool BlobMgr::isFileBlob( std::string& fileName )
{
	bool isBlob = false;
	lockResources();
	BlobInfo * assetInfo = findBlob( fileName );
	if( assetInfo )
	{
		isBlob = assetInfo->getIsBlob();
	}

	unlockResources();
	return isBlob;
}

//============================================================================
bool BlobMgr::isFileBlob( VxSha1Hash& fileHashId )
{
	bool isBlob = false;
	lockResources();
	BlobInfo * assetInfo = findBlob( fileHashId );
	if( assetInfo )
	{
		isBlob = assetInfo->getIsBlob();
	}

	unlockResources();
	return isBlob;
}
*/

//============================================================================
bool BlobMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
{
	bool foundHash = false;
	lockResources();
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( fileFullName == (*iter)->getBlobName() )
		{
			retFileHashId = (*iter)->getBlobHashId();
			foundHash = retFileHashId.isHashValid();
			break;
		}
	}

	unlockResources();
	return foundHash;
}

//============================================================================
bool BlobMgr::getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName )
{
	bool isBlob = false;
	lockResources();
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		if( fileHashId == (*iter)->getBlobHashId() )
		{
			isBlob = true;
			retFileFullName = (*iter)->getBlobName();
			break;
		}
	}

	unlockResources();
	return isBlob;
}


//============================================================================
bool BlobMgr::fromGuiGetBlobInfo( uint8_t fileTypeFilter )
{
	lockResources();
	std::vector<BlobInfo*>::iterator iter;
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		BlobInfo* assetInfo = (*iter);
		if( 0 != ( fileTypeFilter & assetInfo->getBlobType() ) )
		{
			if( assetInfo->getIsSharedFileBlob() || assetInfo->getIsInLibary() )
			{
				IToGui::getToGui().toGuiFileList(	assetInfo->getBlobName().c_str(), 
										assetInfo->getBlobLength(), 
										assetInfo->getBlobType(), 
										assetInfo->getIsSharedFileBlob(),
										assetInfo->getIsInLibary(),
										assetInfo->getBlobHashId().getHashData() );
			}
		}
	}

	unlockResources();
	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false );
	return true;
}

//============================================================================
void BlobMgr::updateDatabase( BlobInfo * assetInfo )
{
	m_BlobInfoDb.addBlob( assetInfo );
}

//============================================================================
void BlobMgr::updateBlobDatabaseSendState( VxGUID& assetUniqueId, EBlobSendState sendState )
{
	m_BlobInfoDb.updateBlobSendState( assetUniqueId, sendState );
}

//============================================================================
void BlobMgr::queryHistoryBlobs( VxGUID& historyId )
{
	std::vector<BlobInfo*>::iterator iter;
	BlobInfo * assetInfo;
	lockResources();
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getHistoryId() == historyId )
		{
			IToGui::getToGui().toGuiBlobSessionHistory( assetInfo );
		}
	}

	unlockResources();
}

//============================================================================
void BlobMgr::updateBlobXferState( VxGUID& assetUniqueId, EBlobSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "BlobMgr::updateBlobXferState state %d start\n", assetSendState );
	std::vector<BlobInfo*>::iterator iter;
	BlobInfo* assetInfo;
	bool assetSendStateChanged = false;
	lockResources();
	for( iter = m_BlobInfoList.begin(); iter != m_BlobInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getBlobUniqueId() == assetUniqueId )
		{
			EBlobSendState oldSendState = assetInfo->getBlobSendState();
			if( oldSendState != assetSendState )
			{
				assetInfo->setBlobSendState( assetSendState );
				assetSendStateChanged = true;
				//updateDatabase( assetInfo );
				updateBlobDatabaseSendState( assetUniqueId, assetSendState );
				switch( assetSendState )
				{
				case eBlobSendStateTxProgress:
					if( eBlobSendStateNone == oldSendState )
					{
						IToGui::getToGui().toGuiBlobAction( eBlobActionTxBegin, assetUniqueId, param );
					}

					IToGui::getToGui().toGuiBlobAction( eBlobActionTxProgress, assetUniqueId, param );
					break;

				case eBlobSendStateRxProgress:
					if( eBlobSendStateNone == oldSendState )
					{
						IToGui::getToGui().toGuiBlobAction( eBlobActionRxBegin, assetUniqueId, param );
					}

					IToGui::getToGui().toGuiBlobAction( eBlobActionRxProgress, assetUniqueId, param );
					break;

				case eBlobSendStateRxSuccess:
					if( ( eBlobSendStateNone == oldSendState )
						|| ( eBlobSendStateRxProgress == oldSendState ) )
					{
						IToGui::getToGui().toGuiBlobAction( eBlobActionRxSuccess, assetUniqueId, param );
						IToGui::getToGui().toGuiBlobAction( eBlobActionRxNotifyNewMsg, assetInfo->getCreatorId(), 100 );
					}
					else 
					{
						IToGui::getToGui().toGuiBlobAction( eBlobActionRxSuccess, assetUniqueId, param );
					}

					break;

				case eBlobSendStateTxSuccess:
					IToGui::getToGui().toGuiBlobAction( eBlobActionTxSuccess, assetUniqueId, param );
					break;

				case eBlobSendStateRxFail:
					IToGui::getToGui().toGuiBlobAction( eBlobActionRxError, assetUniqueId, param );
					break;

				case eBlobSendStateTxFail:
					IToGui::getToGui().toGuiBlobAction( eBlobActionTxError, assetUniqueId, param );
					break;

				case eBlobSendStateTxPermissionErr:
					IToGui::getToGui().toGuiBlobAction( eBlobActionTxError, assetUniqueId, param );
					break;

				case eBlobSendStateRxPermissionErr:
					IToGui::getToGui().toGuiBlobAction( eBlobActionRxError, assetUniqueId, param );
					break;


				case eBlobSendStateNone:
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
		announceBlobXferState( assetUniqueId, assetSendState, param );
	}

	LogMsg( LOG_INFO, "BlobMgr::updateBlobXferState state %d done\n", assetSendState );
}


/*
//============================================================================
RCODE BlobMgr::SendMatchList(	uint32_t			u32SktNum,		// Tronacom socket number
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
		LogMsg( LOG_INFO, "BlobMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
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
					LogMsg( LOG_INFO, "BlobMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
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
RCODE BlobMgr::AddDir( CString & csDirPath )
{
	HRESULT hr;
	char as8FullPath[ VX_MAX_PATH ];

	struct _stat gStat;
	int i;


    CArray<CString, CString&> acsWildNameList;//Extensions ( file extentions )
	acsWildNameList.Add( CString( "*.*" ) );
	CArray<BlobInfo, BlobInfo&> agBlobInfoList;//return FileInfo in array

	VxFindFilesByName(	csDirPath,				//start path to search in
						acsWildNameList,//Extensions ( file extentions )
						agBlobInfoList,//return FileInfo in array
						true,						//recurse subdirectories if TRUE
						false		//if TRUE dont return files matching filter else return files that do
 						);
	int iCnt = agBlobInfoList.GetSize();
	//expand list to include the new files
	int iCurListCnt = m_List.GetSize();
	m_List.SetSize( iCnt + iCurListCnt );
	int iResolvedListIdx = iCurListCnt;

	for( i = 0; i < iCnt; i++ )
	{
		CString cs = agBlobInfoList[ i ];
		__int64 s64FileLen = agBlobInfoList[ i ].m_s64Len;
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
RCODE BlobMgr::MakeMatchList(	CPtrArray * paoRetListPkts, // return list of packets of file lists
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
		LogMsg( LOG_INFO, "BlobMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
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
					LogMsg( LOG_INFO, "BlobMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
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
