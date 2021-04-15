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
// http://www.nolimitconnect.com
//============================================================================

#include "OfferBaseMgr.h"
#include "OfferBaseInfo.h"
#include "OfferBaseInfoDb.h"
#include "OfferBaseCallbackInterface.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/OfferClientMgr/OfferClientInfoDb.h>
#include <GoTvCore/GoTvP2P/OfferHostMgr/OfferHostInfoDb.h>

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
	const char * OFFER_INFO_DB_NAME = "OfferBaseInfoDb.db3";

	//============================================================================
    static void * OfferBaseMgrGenHashIdsThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		OfferBaseMgr * poMgr = (OfferBaseMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetInfoMgrStartup( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
OfferBaseMgr::OfferBaseMgr( P2PEngine& engine, const char * dbName, const char * stateDbName, EOfferMgrType assetMgrType )
: m_Engine( engine )
, m_OfferMgrType( assetMgrType )
, m_OfferBaseInfoDb( createOfferInfoDb( dbName, assetMgrType ) )
{
}

//============================================================================
OfferBaseMgr::~OfferBaseMgr()
{
	delete &m_OfferBaseInfoDb;
}

//============================================================================
OfferBaseInfoDb& OfferBaseMgr::createOfferInfoDb( const char * dbName, EOfferMgrType assetMgrType )
{
    switch( assetMgrType )
    {
    case eOfferMgrTypeOfferClient:
        return *(new OfferClientInfoDb( *this, dbName ));
    case eOfferMgrTypeOfferHost:
        return *(new OfferHostInfoDb( *this, dbName ));

    default:
        vx_assert( false );
        return *(new OfferClientInfoDb( *this, dbName ));
    }
}

//============================================================================
void OfferBaseMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_GenHashThread.startThread( (VX_THREAD_FUNCTION_T)OfferBaseMgrGenHashIdsThreadFunc, this, "OfferBaseMgrGenHash" );			
	}
}

//============================================================================
void OfferBaseMgr::assetInfoMgrStartup( VxThread * startupThread )
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
void OfferBaseMgr::assetInfoMgrShutdown( void )
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
void OfferBaseMgr::generateHashIds( VxThread * genHashThread )
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
				OfferBaseInfo * assetInfo = 0;
				lockResources();
				// move from waiting to completed
				for( iter = m_WaitingForHastList.begin(); iter != m_WaitingForHastList.end(); ++iter )
				{
					OfferBaseInfo * inListOfferBaseInfo = *iter;
					if( inListOfferBaseInfo->getOfferName() == thisFile )
					{
						assetInfo = inListOfferBaseInfo;
						m_WaitingForHastList.erase( iter );
						assetInfo->setOfferHashId( fileHash );
						m_OfferBaseInfoList.push_back( assetInfo );
						break;
					}
				}

				unlockResources();
				std::vector<OfferBaseCallbackInterface *>::iterator callbackIter;
				lockClientList();
				for( callbackIter = m_OfferClients.begin(); callbackIter != m_OfferClients.end(); ++callbackIter )
				{
					OfferBaseCallbackInterface * client = *callbackIter;
					client->callbackHashIdGenerated( thisFile, fileHash );
				}

				unlockClientList();
				if( assetInfo )
				{
					m_OfferBaseInfoDb.addOffer( assetInfo );
					announceOfferAdded( assetInfo );
				}
			}
		}
	}
}

//============================================================================
OfferBaseInfo * OfferBaseMgr::findOffer( std::string& fileName )
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
OfferBaseInfo * OfferBaseMgr::findOffer( VxSha1Hash& fileHashId )
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
OfferBaseInfo * OfferBaseMgr::findOffer( VxGUID& assetId )
{
	if( false == assetId.isVxGUIDValid() )
	{
		//LogMsg( LOG_ERROR, "OfferBaseMgr::findOffer: invalid VxGUID asset id\n" );
        return nullptr;
	}

	std::vector<OfferBaseInfo*>::iterator iter;
	for( OfferBaseInfo * assetInfo : m_OfferBaseInfoList )
	{
		if( assetInfo->getOfferId() == assetId )
		{
			return assetInfo;
		}
	}

	return nullptr;
}

//============================================================================
OfferBaseInfo * OfferBaseMgr::addOfferFile( const char * fileName, uint64_t fileLen, uint16_t fileType )
{
    OfferBaseInfo * assetInfo = createOfferInfo( fileName, fileLen, fileType );
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
bool OfferBaseMgr::addOfferFile(	const char *	fileName, 
									VxGUID&			assetId,  
									uint8_t *		hashId, 
									EOfferLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	OfferBaseInfo * assetInfo = createOfferInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool OfferBaseMgr::addOfferFile(	const char *	fileName, 
									VxGUID&			assetId,  
									VxGUID&		    creatorId, 
									VxGUID&		    historyId, 
									uint8_t *		hashId, 
									EOfferLocation	locationFlags, 
									const char *	assetTag, 
                                    int64_t			timestamp )
{
	OfferBaseInfo * assetInfo = createOfferInfo( fileName, assetId, hashId, locationFlags, assetTag, timestamp );
	if( assetInfo )
	{
		assetInfo->setCreatorId( creatorId );
		assetInfo->setCreatorId( historyId );
		return insertNewInfo( assetInfo );
	}
	
	return false;
}

//============================================================================
bool OfferBaseMgr::addOffer( OfferBaseInfo& assetInfo )
{
	OfferBaseInfo * newOfferBaseInfo = createOfferInfo( assetInfo );
	LogMsg( LOG_INFO, "OfferBaseMgr::addOffer" );
	return insertNewInfo( newOfferBaseInfo );
}

//============================================================================
OfferBaseInfo * OfferBaseMgr::createOfferInfo( const char * fileName, uint64_t fileLen, uint16_t fileType )
{
    OfferBaseInfo * assetInfo = new OfferBaseInfo( fileName, fileLen, fileType );
    if( assetInfo )
    {
        assetInfo->getOfferId().initializeWithNewVxGUID();
    }

    return assetInfo;
}

//============================================================================
OfferBaseInfo * OfferBaseMgr::createOfferInfo( 	const char *	fileName, 
										        VxGUID&			assetId,  
										        uint8_t *	    hashId, 
										        EOfferLocation	locationFlags, 
										        const char *	assetTag, 
                                                int64_t			timestamp )
{
	uint64_t  fileLen = VxFileUtil::getFileLen( fileName );
	uint8_t	fileType = VxFileExtensionToFileTypeFlag( fileName );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
		LogMsg( LOG_ERROR, "ERROR %d OfferBaseMgr::createOfferInfo could not get file info %s\n", VxGetLastError(), fileName );
		return NULL;
	}

	OfferBaseInfo * assetInfo = createOfferInfo( fileName, fileLen, fileType );
	assetInfo->setOfferId( assetId );
	if( false == assetInfo->getOfferId().isVxGUIDValid() )
	{
		assetInfo->getOfferId().initializeWithNewVxGUID();
	}

	assetInfo->getOfferHashId().setHashData( hashId );
	assetInfo->setLocationFlags( locationFlags );
	assetInfo->setOfferTag( assetTag );
	assetInfo->setCreationTime( timestamp ? timestamp : GetTimeStampMs() );

	return assetInfo;
}

//============================================================================
bool OfferBaseMgr::insertNewInfo( OfferBaseInfo * assetInfo )
{
	bool result = false;
	OfferBaseInfo * assetInfoExisting = findOffer( assetInfo->getOfferId() );
	if( assetInfoExisting )
	{
		LogMsg( LOG_ERROR, "ERROR OfferBaseMgr::insertNewInfo: duplicate assset %s\n", assetInfo->getOfferName().c_str() );
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
	//	generateHashForFile( assetInfo->getOfferName() );
	//	result = true;
	//}
	//else
	{
        updateDatabase( assetInfo );
		if( !assetInfoExisting )
		{
			lockResources();
			m_OfferBaseInfoList.push_back( assetInfo );
			unlockResources();
			announceOfferAdded( assetInfo );
		}
        else
        {
            announceOfferUpdated( assetInfo );
        }
	
		result = true;
	}

	return result;
}

//============================================================================
bool OfferBaseMgr::updateOffer( OfferBaseInfo& assetInfo )
{
    OfferBaseInfo * existingOffer = findOffer( assetInfo.getOfferId() );
    if( existingOffer )
    {
        *existingOffer = assetInfo;
        updateDatabase( existingOffer );
        announceOfferUpdated( existingOffer );
        return true;
    }

    return false;
}

//============================================================================
void OfferBaseMgr::announceOfferAdded( OfferBaseInfo * assetInfo )
{
	// LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferAdded start" );
	if( assetInfo->isFileOffer() )
	{
		updateFileListPackets();
		updateOfferFileTypes();
	}
	
	lockClientList();
	std::vector<OfferBaseCallbackInterface *>::iterator iter;
	for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
	{
		OfferBaseCallbackInterface * client = *iter;
		client->callbackOfferAdded( assetInfo );
	}

	unlockClientList();
	// LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferAdded done" );
}

//============================================================================
void OfferBaseMgr::announceOfferUpdated( OfferBaseInfo * assetInfo )
{
    // LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferUpdated start" );
    lockClientList();
    std::vector<OfferBaseCallbackInterface *>::iterator iter;
    for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
    {
        OfferBaseCallbackInterface * client = *iter;
        client->callbackOfferAdded( assetInfo );
    }

    unlockClientList();
    // LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferUpdated done" );
}

//============================================================================
void OfferBaseMgr::announceOfferRemoved( OfferBaseInfo * assetInfo )
{
	//if( assetInfo->getIsOfferBase() )
	{
		updateFileListPackets();
		updateOfferFileTypes();
	}

	lockClientList();
	std::vector<OfferBaseCallbackInterface *>::iterator iter;
	for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
	{
		OfferBaseCallbackInterface * client = *iter;
		client->callbackOfferRemoved( assetInfo );
	}

	unlockClientList();
}

//============================================================================
void OfferBaseMgr::announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "OfferBaseMgr::announceOfferXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<OfferBaseCallbackInterface *>::iterator iter;
	for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
	{
		OfferBaseCallbackInterface * client = *iter;
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
    std::vector<OfferBaseCallbackInterface *>::iterator iter;
    for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
    {
        OfferBaseCallbackInterface * client = *iter;
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
			OfferBaseInfo * assetInfo = *iter;
			m_OfferBaseInfoList.erase( iter );
			m_OfferBaseInfoDb.removeOffer( fileName.c_str() );
			announceOfferRemoved( assetInfo );
			delete assetInfo;
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
			OfferBaseInfo * assetInfo = *iter;
			m_OfferBaseInfoList.erase( iter );
			m_OfferBaseInfoDb.removeOffer( assetInfo );
			announceOfferRemoved( assetInfo );
			delete assetInfo;
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
void OfferBaseMgr::updateOfferListFromDb( VxThread * startupThread )
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
			OfferBaseInfo* assetInfo = (*iter);
			EOfferSendState sendState = assetInfo->getOfferSendState();
			if( eOfferSendStateTxProgress == sendState ) 
			{
				assetInfo->setOfferSendState( eOfferSendStateTxFail );
				m_OfferBaseInfoDb.updateOfferSendState( assetInfo->getOfferId(), eOfferSendStateTxFail );
			}
			else if(  eOfferSendStateRxProgress == sendState  )
			{
				assetInfo->setOfferSendState( eOfferSendStateRxFail );
				m_OfferBaseInfoDb.updateOfferSendState( assetInfo->getOfferId(), eOfferSendStateRxFail );
			}

			if( assetInfo->needsHashGenerated() )
			{
				m_WaitingForHastList.push_back( assetInfo );
				m_OfferBaseInfoList.erase( iter );
				generateHashForFile( assetInfo->getOfferName() );
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
		std::vector<OfferBaseCallbackInterface *>::iterator iter;
		for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
		{
			OfferBaseCallbackInterface * client = *iter;
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
		OfferBaseInfo * assetInfo = (*iter); 
		if( ( false == assetInfo->isFileOffer() ) || ( false == assetInfo->getOfferHashId().isHashValid() ) )
			continue;

		if( 0 == pktFileList )
		{
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
		}

		if( pktFileList->canAddFile( (uint32_t)(assetInfo->getRemoteOfferName().size() + 1) ) )
		{
			pktFileList->addFile(	assetInfo->getOfferHashId(),
									assetInfo->getOfferLength(),
									assetInfo->getOfferType(),
									assetInfo->getRemoteOfferName().c_str() );
		}
		else
		{
			m_FileListPackets.push_back( pktFileList );
			pktFileList = new PktFileListReply();
			pktFileList->setListIndex( (uint32_t)m_FileListPackets.size() );
			pktFileList->addFile(	assetInfo->getOfferHashId(),
									assetInfo->getOfferLength(),
									assetInfo->getOfferType(),
									assetInfo->getOfferName().c_str() );
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
		std::vector<OfferBaseCallbackInterface *>::iterator iter;
		for( iter = m_OfferClients.begin();	iter != m_OfferClients.end(); ++iter )
		{
			OfferBaseCallbackInterface * client = *iter;
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
	OfferBaseInfo* assetInfo = findOffer( fileName );
	if( assetInfo )
	{
		if( ( false == shareFile ) && assetInfo->isSharedFileOffer() )
		{
			assetInfo->setIsSharedFileOffer( false );
			updateDatabase( assetInfo );
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
		OfferBaseInfo * assetInfo = createOfferInfo( fileName.c_str(), guid, fileHashId, eOfferLocShared );
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
		OfferBaseInfo* assetInfo = (*iter);
		if( 0 != ( fileTypeFilter & assetInfo->getOfferType() ) )
		{
			if( assetInfo->isSharedFileOffer() || assetInfo->isInLibary() )
			{
				IToGui::getToGui().toGuiFileList(	assetInfo->getOfferName().c_str(), 
										            assetInfo->getOfferLength(), 
										            assetInfo->getOfferType(), 
										            assetInfo->isSharedFileOffer(),
										            assetInfo->isInLibary(),
										            assetInfo->getOfferHashId().getHashData() );
			}
		}
	}

	unlockResources();
	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false );
	return true;
}

//============================================================================
void OfferBaseMgr::updateDatabase( OfferBaseInfo * assetInfo )
{
	m_OfferBaseInfoDb.addOffer( assetInfo );
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
	OfferBaseInfo * assetInfo;
	lockResources();
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getHistoryId() == historyId )
		{
            onQueryHistoryOffer( assetInfo );
			// BRJ IToGui::getToGui().toGuiOfferSessionHistory( assetInfo );
		}
	}

	unlockResources();
}

//============================================================================
void OfferBaseMgr::updateOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
	LogMsg( LOG_INFO, "OfferBaseMgr::updateOfferXferState state %d start\n", assetSendState );
	std::vector<OfferBaseInfo*>::iterator iter;
	OfferBaseInfo* assetInfo;
	bool assetSendStateChanged = false;
	lockResources();
	for( iter = m_OfferBaseInfoList.begin(); iter != m_OfferBaseInfoList.end(); ++iter )
	{
		assetInfo = (*iter);
		if( assetInfo->getOfferId() == assetOfferId )
		{
			EOfferSendState oldSendState = assetInfo->getOfferSendState();
			if( oldSendState != assetSendState )
			{
				assetInfo->setOfferSendState( assetSendState );
				assetSendStateChanged = true;
				//updateDatabase( assetInfo );
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
						announceOfferAction( assetInfo->getCreatorId(), eOfferActionRxNotifyNewMsg, 100 );
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
void OfferBaseMgr::addOfferMgrClient( OfferBaseCallbackInterface * client, bool enable )
{
    AutoResourceLock( this );
    if( enable )
    {
        m_OfferClients.push_back( client );
    }
    else
    {
        std::vector<OfferBaseCallbackInterface *>::iterator iter;
        for( iter = m_OfferClients.begin(); iter != m_OfferClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_OfferClients.erase( iter );
                return;
            }
        }
    }
}
