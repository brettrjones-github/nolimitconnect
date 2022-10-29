//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "OfferBaseMgr.h"
#include "OfferBaseInfo.h"
#include "OfferBaseInfoDb.h"
#include "OfferCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>

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
	const char* OFFER_INFO_DB_NAME = "OfferBaseInfoDb.db3";

	//============================================================================
    static void * OfferBaseMgrGenHashIdsThreadFunc( void * pvContext )
	{
		VxThread* poThread = (VxThread*)pvContext;
		poThread->setIsThreadRunning( true );
		OfferBaseMgr * poMgr = (OfferBaseMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->offerInfoMgrStartup( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
OfferBaseMgr::OfferBaseMgr( P2PEngine& engine, const char* dbName, const char* stateDbName, EOfferMgrType assetMgrType )
: m_Engine( engine )
, m_OfferMgrType( assetMgrType )
, m_OfferBaseInfoDb( *this, dbName )
{
}

//============================================================================
void OfferBaseMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		// user specific directory should be set
		std::string dbFileName = VxGetSettingsDirectory();
		dbFileName += m_OfferBaseInfoDb.getDatabaseName();
		lockResources();
		m_OfferBaseInfoDb.dbShutdown();
		m_OfferBaseInfoDb.dbStartup( 1, dbFileName );

		clearOfferInfoList();
		m_OfferBaseInfoDb.getAllOffers( m_OfferBaseInfoList );

		unlockResources();
		m_Initialized = true;

		m_GenHashThread.startThread( (VX_THREAD_FUNCTION_T)OfferBaseMgrGenHashIdsThreadFunc, this, "OfferBaseMgrGenHash" );			
	}
}

//============================================================================
void OfferBaseMgr::offerInfoMgrStartup( VxThread* startupThread )
{
	if( startupThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += OFFER_INFO_DB_NAME; 
	lockResources();
	m_OfferBaseInfoDb.dbShutdown();
	m_OfferBaseInfoDb.dbStartup( 1, dbName );
	unlockResources();
	if( startupThread->isAborted() )
		return;
	updateOfferListFromDb( startupThread );
	m_OfferBaseListInitialized = true;
	if( startupThread->isAborted() )
		return;
	generateHashIds( startupThread );
}

//============================================================================
void OfferBaseMgr::offerInfoMgrShutdown( void )
{
	m_GenHashThread.abortThreadRun( true );
	m_GenHashSemaphore.signal();
	lockResources();
	clearOfferInfoList();
	clearOfferFileListPackets();
	m_OfferBaseInfoDb.dbShutdown();
	unlockResources();
	m_OfferBaseListInitialized = false;
	m_Initialized = false;
}

//============================================================================
void OfferBaseMgr::generateHashForFile( std::string fileName )
{
	m_GenHashMutex.lock();
	m_GenHashList.push_back( fileName );
	m_GenHashMutex.unlock();
	m_GenHashSemaphore.signal();
}

//============================================================================
void OfferBaseMgr::generateHashIds( VxThread* genHashThread )
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

				std::vector<OfferBaseInfo*>::iterator iter;
				OfferBaseInfo* offerInfo = 0;
				lockResources();
				// move from waiting to completed
				for( iter = m_WaitingForHastList.begin(); iter != m_WaitingForHastList.end(); ++iter )
				{
					OfferBaseInfo* inListOfferBaseInfo = *iter;
					if( inListOfferBaseInfo->getOfferName() == thisFile )
					{
						offerInfo = inListOfferBaseInfo;
						m_WaitingForHastList.erase( iter );
						offerInfo->setOfferHashId( fileHash );
						m_OfferBaseInfoList.push_back( offerInfo );
						break;
					}
				}

				unlockResources();

				lockClientList();
				for( auto client : m_OfferClients )
				{
					client->callbackHashIdGenerated( thisFile, fileHash );
				}

				unlockClientList();
				if( offerInfo )
				{
					m_OfferBaseInfoDb.addOffer( offerInfo );
					announceOfferAdded( offerInfo );
				}
			}
		}
	}
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::findOffer( std::string& fileName )
{
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( (*iter)->getOfferName() == fileName )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::findOffer( VxSha1Hash& fileHashId )
{
	if( false == fileHashId.isHashValid() )
	{
		LogMsg( LOG_ERROR, "OfferBaseMgr::findOffer: invalid file hash id\n" );
		return 0;
	}

	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( (*iter)->getOfferHashId() == fileHashId )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::findOffer( VxGUID& assetId )
{
	if( false == assetId.isVxGUIDValid() )
	{
		//LogMsg( LOG_ERROR, "OfferBaseMgr::findOffer: invalid VxGUID asset id\n" );
        return nullptr;
	}

	for( OfferBaseInfo* offerInfo : m_OfferBaseInfoList )
	{
		if( offerInfo->getOfferId() == assetId )
		{
			return offerInfo;
		}
	}

	return nullptr;
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::addOfferFile( const char* fileName, uint64_t fileLen, uint16_t fileType )
{
    OfferBaseInfo* offerInfo = createOfferInfo( fileName, fileLen, fileType );
    if( offerInfo )
    {
        if( insertNewInfo( offerInfo ) )
        {
            return offerInfo;
        }
    }

    return NULL;
}

//============================================================================
bool OfferBaseMgr::addOfferFile(	const char*	fileName, 
									VxGUID&			assetId,  
									uint8_t *		hashId, 
									EOfferLocation	locationFlags, 
									const char*	assetTag, 
                                    int64_t			timestamp )
{
	OfferBaseInfo* offerInfo = createOfferInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( offerInfo )
	{
		return insertNewInfo( offerInfo );
	}
	
	return false;
}

//============================================================================
bool OfferBaseMgr::addOfferFile(	const char*	fileName, 
									VxGUID&			assetId,  
									VxGUID&		    creatorId, 
									VxGUID&		    historyId, 
									uint8_t *		hashId, 
									EOfferLocation	locationFlags, 
									const char*	assetTag, 
                                    int64_t			timestamp )
{
	OfferBaseInfo* offerInfo = createOfferInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( offerInfo )
	{
		offerInfo->setCreatorId( creatorId );
		offerInfo->setCreatorId( historyId );
		return insertNewInfo( offerInfo );
	}
	
	return false;
}

//============================================================================
bool OfferBaseMgr::addOffer( OfferBaseInfo& offerInfo )
{
	OfferBaseInfo* newOfferBaseInfo = createOfferInfo( offerInfo );
	LogMsg( LOG_INFO, "OfferBaseMgr::addOffer" );
	return insertNewInfo( newOfferBaseInfo );
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::createOfferInfo( std::string fileName, uint64_t fileLen, uint16_t fileType )
{
    OfferBaseInfo* offerInfo = new OfferBaseInfo( fileName, fileLen, fileType );
    if( offerInfo )
    {
        offerInfo->getOfferId().initializeWithNewVxGUID();
    }

    return offerInfo;
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::createOfferInfo( OfferBaseInfo& offerInfo )
{
	return new OfferBaseInfo( offerInfo );
}

//============================================================================
OfferBaseInfo* OfferBaseMgr::createOfferInfo( std::string	fileName,
										        VxGUID&			assetId,  
										        uint8_t *	    hashId, 
										        EOfferLocation	locationFlags, 
										        const char*	assetTag, 
                                                int64_t			timestamp )
{
	uint64_t  fileLen = VxFileUtil::getFileLen( fileName.c_str() );
	uint8_t	fileType = VxFileExtensionToFileTypeFlag( fileName.c_str() );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
        LogMsg( LOG_ERROR, "ERROR %d OfferBaseMgr::createOfferInfo could not get file info %s", VxGetLastError(), fileName.c_str() );
		return NULL;
	}

	OfferBaseInfo* offerInfo = createOfferInfo( fileName, fileLen, fileType );
	offerInfo->setOfferId( assetId );
	if( false == offerInfo->getOfferId().isVxGUIDValid() )
	{
		offerInfo->getOfferId().initializeWithNewVxGUID();
	}

	offerInfo->getOfferHashId().setHashData( hashId );
	offerInfo->setLocationFlags( locationFlags );
	offerInfo->setOfferTag( assetTag );
	offerInfo->setCreationTime( timestamp ? timestamp : GetTimeStampMs() );

	return offerInfo;
}

//============================================================================
bool OfferBaseMgr::insertNewInfo( OfferBaseInfo* offerInfo )
{
	bool result = false;
	OfferBaseInfo* offerInfoExisting = findOffer( offerInfo->getOfferId() );
	if( offerInfoExisting )
	{
		LogMsg( LOG_ERROR, "ERROR OfferBaseMgr::insertNewInfo: duplicate assset %s", offerInfo->getOfferName().c_str() );
		if( offerInfoExisting != offerInfo )
		{
			*offerInfoExisting = *offerInfo;
			offerInfo = offerInfoExisting;
		}
	}

	if( 0 == offerInfo->getCreationTime() )
	{
		offerInfo->setCreationTime( GetTimeStampMs() );
	}

	//if( offerInfo->needsHashGenerated() )
	//{
	//	lockResources();
	//	m_WaitingForHastList.push_back( offerInfo );
	//	unlockResources();
	//	generateHashForFile( offerInfo->getOfferName() );
	//	result = true;
	//}
	//else
	{
        updateDatabase( offerInfo );
		if( !offerInfoExisting )
		{
			lockResources();
			m_OfferBaseInfoList.push_back( offerInfo );
			unlockResources();
			announceOfferAdded( offerInfo );
		}
        else
        {
            announceOfferUpdated( offerInfo );
        }
	
		result = true;
	}

	return result;
}

//============================================================================
bool OfferBaseMgr::updateOffer( OfferBaseInfo& offerInfo )
{
    OfferBaseInfo* existingOffer = findOffer( offerInfo.getOfferId() );
    if( existingOffer )
    {
        *existingOffer = offerInfo;
        updateDatabase( existingOffer );
        announceOfferUpdated( existingOffer );
        return true;
    }

    return false;
}

//============================================================================
void OfferBaseMgr::announceOfferAdded( OfferBaseInfo* offerInfo )
{
	// LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferAdded start" );
	if( offerInfo->isFileOffer() )
	{
		updateFileListPackets();
		updateOfferFileTypes();
	}
	
	lockClientList();
	for( auto client : m_OfferClients )
	{
		client->callbackOfferAdded( offerInfo );
	}

	unlockClientList();
	// LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferAdded done" );
}

//============================================================================
void OfferBaseMgr::announceOfferUpdated( OfferBaseInfo* offerInfo )
{
    // LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferUpdated start" );
    lockClientList();
	for( auto client : m_OfferClients )
	{
        client->callbackOfferAdded( offerInfo );
    }

    unlockClientList();
    // LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferUpdated done" );
}

//============================================================================
void OfferBaseMgr::announceOfferRemoved( OfferBaseInfo* offerInfo )
{
	//if( offerInfo->getIsOfferBase() )
	{
		updateFileListPackets();
		updateOfferFileTypes();
	}

	lockClientList();
	for( auto client : m_OfferClients )
	{
		client->callbackOfferRemoved( offerInfo );
	}

	unlockClientList();
}

//============================================================================
void OfferBaseMgr::announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferXferState state %d start\n", assetSendState );
	lockClientList();
	for( auto client : m_OfferClients )
	{
		client->callbackOfferSendState( assetOfferId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferXferState state %d done\n", assetSendState );
}

//============================================================================
void OfferBaseMgr::announceOfferAction( VxGUID& assetOfferId, EOfferAction offerAction, int param )
{
    LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferXferState state %d start\n", offerAction );
    lockClientList();
	for( auto client : m_OfferClients )
	{
        client->callbackOfferAction( assetOfferId, offerAction, param );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferXferState state %d done\n", offerAction );
}

//============================================================================
bool OfferBaseMgr::removeOffer( std::string fileName )
{
	bool assetRemoved = false;
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getOfferName() )
		{
			OfferBaseInfo* offerInfo = *iter;
			m_OfferBaseInfoList.erase( iter );
			m_OfferBaseInfoDb.removeOffer( fileName.c_str() );
			announceOfferRemoved( offerInfo );
			delete offerInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
bool OfferBaseMgr::removeOffer( VxGUID& assetOfferId )
{
	bool assetRemoved = false;
	std::vector<OfferBaseInfo*>::iterator iter;
	for ( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( assetOfferId == ( *iter )->getOfferId() )
		{
			OfferBaseInfo* offerInfo = *iter;
			m_OfferBaseInfoList.erase( iter );
			m_OfferBaseInfoDb.removeOffer( offerInfo );
			announceOfferRemoved( offerInfo );
			delete offerInfo;
			assetRemoved = true;
			break;
		}
	}

	return assetRemoved;
}

//============================================================================
void OfferBaseMgr::clearOfferInfoList( void )
{
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		delete (*iter);
	}

	m_OfferBaseInfoList.clear();
}

//============================================================================
void OfferBaseMgr::updateOfferListFromDb( VxThread* startupThread )
{
	std::vector<OfferBaseInfo*>::iterator iter;
	lockResources();
	clearOfferInfoList();
	m_OfferBaseInfoDb.getAllOffers( m_OfferBaseInfoList );
	bool movedToGenerateHash = true;
	while(	movedToGenerateHash 
			&& ( false == startupThread->isAborted() ) )
	{
		// there should not be any without valid hash but if is then generate it
		movedToGenerateHash = false;
		for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
		{
			OfferBaseInfo* offerInfo = (*iter);
			EOfferSendState sendState = offerInfo->getOfferSendState();
			if( eOfferSendStateTxProgress == sendState ) 
			{
				offerInfo->setOfferSendState( eOfferSendStateTxFail );
				m_OfferBaseInfoDb.updateOfferSendState( offerInfo->getOfferId(), eOfferSendStateTxFail );
			}
			else if(  eOfferSendStateRxProgress == sendState  )
			{
				offerInfo->setOfferSendState( eOfferSendStateRxFail );
				m_OfferBaseInfoDb.updateOfferSendState( offerInfo->getOfferId(), eOfferSendStateRxFail );
			}

			if( offerInfo->needsHashGenerated() )
			{
				m_WaitingForHastList.push_back( offerInfo );
				m_OfferBaseInfoList.erase( iter );
				generateHashForFile( offerInfo->getOfferName() );
				movedToGenerateHash = true;
				break;
			}
		}
	}

	unlockResources();
	updateFileListPackets();
	updateOfferFileTypes();
}

//============================================================================
void OfferBaseMgr::updateOfferFileTypes( void )
{
	uint16_t u16FileTypes = 0;
	std::vector<OfferBaseInfo*>::iterator iter;
	lockResources();
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( (*iter)->isFileOffer() )
		{
			u16FileTypes		|= (*iter)->getOfferType();
		}
	}

	unlockResources();
	// ignore extended types
	u16FileTypes = u16FileTypes & 0xff;
	m_u16OfferBaseFileTypes = u16FileTypes;
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
		for( auto client : m_OfferClients )
		{
			client->callbackOfferFileTypesChanged( u16FileTypes );
		}

		unlockClientList();
	}
}

//============================================================================
void OfferBaseMgr::updateFileListPackets( void )
{
	bool hadOfferBaseFiles = m_FileListPackets.size() ? true : false;
	PktFileListReply * pktFileList = 0;
	clearOfferFileListPackets();
	lockFileListPackets();
	lockResources();
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		OfferBaseInfo* offerInfo = (*iter); 
		if( ( false == offerInfo->isFileOffer() ) || ( false == offerInfo->getOfferHashId().isHashValid() ) )
			continue;

		if( 0 == pktFileList )
		{
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
		}

		if( pktFileList->canAddFile( (uint32_t)(offerInfo->getRemoteOfferName().size() + 1) ) )
		{
			pktFileList->addFile(	offerInfo->getOfferHashId(),
									offerInfo->getOfferLength(),
									offerInfo->getOfferType(),
									offerInfo->getRemoteOfferName().c_str() );
		}
		else
		{
			m_FileListPackets.push_back( pktFileList );
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
			pktFileList->addFile(	offerInfo->getOfferHashId(),
									offerInfo->getOfferLength(),
									offerInfo->getOfferType(),
									offerInfo->getOfferName().c_str() );
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
	if( hadOfferBaseFiles || m_FileListPackets.size() )
	{
		lockClientList();
		for( auto client : m_OfferClients )
		{
			client->callbackOfferPktFileListUpdated();
		}

		unlockClientList();
	}
}

//============================================================================
void OfferBaseMgr::clearOfferFileListPackets( void )
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
bool OfferBaseMgr::fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId )
{
	lockResources();
	OfferBaseInfo* offerInfo = findOffer( fileName );
	if( offerInfo )
	{
		if( ( false == shareFile ) && offerInfo->isSharedFileOffer() )
		{
			offerInfo->setIsSharedFileOffer( false );
			updateDatabase( offerInfo );
			unlockResources();
			updateOfferFileTypes();
			updateFileListPackets();
			return true;
		}
	}

	unlockResources();

	if( shareFile )
	{
		// file is not currently OfferBase and should be
		VxGUID guid;
		OfferBaseInfo* offerInfo = createOfferInfo( fileName.c_str(), guid, fileHashId, eOfferLocShared );
		if( offerInfo )
		{
			insertNewInfo( offerInfo );
		}
	}
	else
	{
		return false;
	}

	return true;
}

//============================================================================
bool OfferBaseMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
{
	bool foundHash = false;
	lockResources();
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( fileFullName == (*iter)->getOfferName() )
		{
			retFileHashId = (*iter)->getOfferHashId();
			foundHash = retFileHashId.isHashValid();
			break;
		}
	}

	unlockResources();
	return foundHash;
}

//============================================================================
bool OfferBaseMgr::getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName )
{
	bool isOfferBase = false;
	lockResources();
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		if( fileHashId == (*iter)->getOfferHashId() )
		{
			isOfferBase = true;
			retFileFullName = (*iter)->getOfferName();
			break;
		}
	}

	unlockResources();
	return isOfferBase;
}


//============================================================================
bool OfferBaseMgr::fromGuiGetOfferBaseInfo( uint8_t fileTypeFilter )
{
	lockResources();
	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		OfferBaseInfo* offerInfo = (*iter);
		if( 0 != ( fileTypeFilter & offerInfo->getOfferType() ) )
		{
			if( offerInfo->isSharedFileOffer() || offerInfo->isInLibary() )
			{
				FileInfo fileInfo( offerInfo->getCreatorId(), offerInfo->getOfferName(), offerInfo->getOfferLength(),
					(uint8_t)offerInfo->getOfferType(), offerInfo->getOfferId(), offerInfo->getOfferHashId() );
				fileInfo.setIsInLibrary( offerInfo->isInLibary() );
				fileInfo.setIsSharedFile( offerInfo->isSharedFileOffer() );

				IToGui::getToGui().toGuiFileList( fileInfo );
			}
		}
	}

	unlockResources();
	IToGui::getToGui().toGuiFileListCompleted();
	return true;
}

//============================================================================
void OfferBaseMgr::updateDatabase( OfferBaseInfo* offerInfo )
{
	m_OfferBaseInfoDb.addOffer( offerInfo );
}

//============================================================================
void OfferBaseMgr::updateOfferDatabaseSendState( VxGUID& assetOfferId, EOfferSendState sendState )
{
	m_OfferBaseInfoDb.updateOfferSendState( assetOfferId, sendState );
}

//============================================================================
void OfferBaseMgr::queryHistoryOffers( VxGUID& historyId )
{
	std::vector<OfferBaseInfo*>::iterator iter;
	OfferBaseInfo* offerInfo;
	lockResources();
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		offerInfo = (*iter);
		if( offerInfo->getHistoryId() == historyId )
		{
            onQueryHistoryOffer( offerInfo );
			// BRJ IToGui::getToGui().toGuiOfferSessionHistory( offerInfo );
		}
	}

	unlockResources();
}

//============================================================================
void OfferBaseMgr::updateOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "OfferBaseMgr::updateOfferXferState state %d start", assetSendState );
	std::vector<OfferBaseInfo*>::iterator iter;
	OfferBaseInfo* offerInfo;
	bool assetSendStateChanged = false;
	lockResources();
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		offerInfo = (*iter);
		if( offerInfo->getOfferId() == assetOfferId )
		{
			EOfferSendState oldSendState = offerInfo->getOfferSendState();
			if( oldSendState != assetSendState )
			{
				offerInfo->setOfferSendState( assetSendState );
				assetSendStateChanged = true;
				//updateDatabase( offerInfo );
				updateOfferDatabaseSendState( assetOfferId, assetSendState );
				switch( assetSendState )
				{
				case eOfferSendStateTxProgress:
					if( eOfferSendStateNone == oldSendState )
					{
						announceOfferAction( assetOfferId, eOfferActionTxBegin, param );
					}

					announceOfferAction( assetOfferId, eOfferActionTxProgress, param );
					break;

				case eOfferSendStateRxProgress:
					if( eOfferSendStateNone == oldSendState )
					{
						announceOfferAction( assetOfferId, eOfferActionRxBegin, param );
					}

					announceOfferAction( assetOfferId, eOfferActionRxProgress, param );
					break;

				case eOfferSendStateRxSuccess:
					if( ( eOfferSendStateNone == oldSendState )
						|| ( eOfferSendStateRxProgress == oldSendState ) )
					{
						announceOfferAction( assetOfferId, eOfferActionRxSuccess, param );
						announceOfferAction( offerInfo->getCreatorId(), eOfferActionRxNotifyNewMsg, 100 );
					}
					else 
					{
						announceOfferAction( assetOfferId, eOfferActionRxSuccess, param );
					}

					break;

				case eOfferSendStateTxSuccess:
					announceOfferAction( assetOfferId, eOfferActionTxSuccess, param );
					break;

				case eOfferSendStateRxFail:
					announceOfferAction( assetOfferId, eOfferActionRxError, param );
					break;

				case eOfferSendStateTxFail:
					announceOfferAction( assetOfferId, eOfferActionTxError, param );
					break;

				case eOfferSendStateTxPermissionErr:
					announceOfferAction( assetOfferId, eOfferActionTxError, param );
					break;

				case eOfferSendStateRxPermissionErr:
					announceOfferAction( assetOfferId, eOfferActionRxError, param );
					break;


				case eOfferSendStateNone:
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
        announceOfferXferState( assetOfferId, assetSendState, param );
	}

	LogMsg( LOG_INFO, "OfferBaseMgr::updateOfferXferState state %d done\n", assetSendState );
}


//============================================================================
bool OfferBaseMgr::isAllowedFileOrDir( std::string strFileName )
{
    if( VxIsExecutableFile( strFileName ) 
        || VxIsShortcutFile( strFileName ) )
    {
        return false;
    }

    return true;
}

//============================================================================
void OfferBaseMgr::wantToGuiOfferCallbacks( OfferCallbackInterface* client, bool enable )
{
	lockClientList();
	if( enable )
	{
		m_OfferClients.push_back( client );
	}
	else
	{
		for( auto iter = m_OfferClients.begin(); iter != m_OfferClients.end(); ++iter )
		{
			if( *iter == client )
			{
				m_OfferClients.erase( iter );
				break;
			}
		}
	}

	unlockClientList();
}
