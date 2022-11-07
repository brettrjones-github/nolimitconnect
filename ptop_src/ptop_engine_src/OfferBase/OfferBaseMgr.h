#pragma once
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

#include "OfferCallbackInterface.h"
#include "OfferBaseInfoDb.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class FileInfo;
class GuiUser;
class IToGui;
class OfferCallbackInterface;
class OfferBaseInfo;
class OfferBaseInfoDb;
class OfferBaseHistoryMgr;
class P2PEngine;
class PktFileListReply;
class QWidget;

class OfferBaseMgr
{
public:
	OfferBaseMgr( P2PEngine& engine, const char* dbName, const char* stateDbName, EOfferMgrType assetMgrType );
	virtual ~OfferBaseMgr() = default;

    class AutoResourceLock
    {
    public:
        AutoResourceLock( OfferBaseMgr * assetMgrBase ) : m_Mutex(assetMgrBase->getResourceMutex())	{ m_Mutex.lock(); }
        ~AutoResourceLock()																			{ m_Mutex.unlock(); }
        VxMutex&				m_Mutex;
    };

	void                        wantGuiOfferCallbacks( OfferCallbackInterface* client, bool enable );

    virtual OfferBaseInfoDb&    getOfferInfoDb( void ) { return m_OfferBaseInfoDb; }

    virtual std::vector<OfferBaseInfo*>&	getOfferBaseInfoList( void )				{ return m_OfferBaseInfoList; }

    // startup when user specific directory has been set after user logs on
    virtual void				fromGuiUserLoggedOn( void );
    virtual bool				fromGuiGetOfferBaseInfo( uint8_t fileTypeFilter );
    virtual bool				fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId );

	virtual void				fromGuiMakePluginOffer( QWidget* parent, EPluginType pluginType, GuiUser* guiUser, FileInfo& fileInfo ) {};

    virtual void				announceOfferAdded( OfferBaseInfo* offerInfo );
    virtual void				announceOfferUpdated( OfferBaseInfo* offerInfo );
    virtual void				announceOfferRemoved( OfferBaseInfo* offerInfo );
    virtual void				announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param );
    virtual void				announceOfferAction( VxGUID& assetOfferId, EOfferAction offerAction, int param );

    virtual void                onQueryHistoryOffer( OfferBaseInfo* offerInfo ) {}; // should be overriden

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    bool						isAllowedFileOrDir( std::string strFileName );

	virtual bool				isOfferListInitialized( void )				{ return m_OfferBaseListInitialized; }

	void						offerInfoMgrStartup( VxThread* startupThread );
	void						offerInfoMgrShutdown( void );

    bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );
	bool						getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName );


	OfferBaseInfo*				findOffer( std::string& fileName );
	OfferBaseInfo*				findOffer( VxSha1Hash& fileHashId );
	OfferBaseInfo*				findOffer( VxGUID& assetId );

	uint16_t					getOfferBaseFileTypes( void )				{ return m_u16OfferBaseFileTypes; }
	void						updateOfferFileTypes( void );

	void						lockFileListPackets( void )				{ m_FileListPacketsMutex.lock(); }
	void						unlockFileListPackets( void )			{ m_FileListPacketsMutex.unlock(); }
	std::vector<PktFileListReply*>&	getFileListPackets( void )			{ return m_FileListPackets; }
	void						updateFileListPackets( void );

    OfferBaseInfo* 				addOfferFile( const char* fileName, uint64_t fileLen, uint16_t fileType );

	bool						addOfferFile(	const char*	fileName, 
												VxGUID&			assetId,  
												uint8_t *		hashId = 0, 
												EOfferLocation	locationFlags = eOfferLocUnknown, 
												const char*	assetTag = "", 
												int64_t		    timestamp = 0 );

	bool						addOfferFile(	const char*	fileName, 
												VxGUID&			assetId,  
												VxGUID&		    creatorId, 
												VxGUID&		    historyId, 
												uint8_t *		hashId = 0, 
												EOfferLocation	locationFlags = eOfferLocUnknown, 
												const char*	assetTag = "", 
                                                int64_t			timestamp = 0 );

	bool						addOffer( OfferBaseInfo& offerInfo );

    bool						updateOffer( OfferBaseInfo& offerInfo );
	bool						removeOffer( std::string fileName );
	bool						removeOffer( VxGUID& assetOfferId );
	void						queryHistoryOffers( VxGUID& historyId );

	void						generateHashForFile( std::string fileName );
	void						updateOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param = 0 );

protected:
    virtual OfferBaseInfo*     createOfferInfo( std::string fileName, uint64_t fileLen, uint16_t fileType );
    virtual OfferBaseInfo*     createOfferInfo( OfferBaseInfo& offerInfo );

   void							lockClientList( void )						{ m_ClientListMutex.lock(); }
   void							unlockClientList( void )					{ m_ClientListMutex.unlock(); }

	void						updateOfferListFromDb( VxThread* thread );
	void						generateHashIds( VxThread* thread );
	void						clearOfferFileListPackets( void );
	void						clearOfferInfoList( void );
	OfferBaseInfo*				createOfferInfo( std::string fileName,
													VxGUID&			assetId,  
													uint8_t *		hashId, 
													EOfferLocation	locationFlags = eOfferLocUnknown, 
													const char*	assetTag = "", 
													int64_t			timestamp = 0 );
	bool						insertNewInfo( OfferBaseInfo* offerInfo );
	void						updateDatabase( OfferBaseInfo* offerInfo );
	void						updateOfferDatabaseSendState( VxGUID& assetOfferId, EOfferSendState sendState );

    //=== vars ===//
    P2PEngine&					m_Engine;
    EOfferMgrType               m_OfferMgrType{ eOfferMgrNotSet };
    VxMutex						m_ResourceMutex;
    VxMutex						m_ClientListMutex;

    std::vector<OfferCallbackInterface *> m_OfferClients;
	bool						m_Initialized{ false };

	std::vector<OfferBaseInfo*>	m_WaitingForHastList;
	std::vector<std::string>	m_GenHashList;
	VxMutex						m_GenHashMutex;
	VxThread					m_GenHashThread;
	VxSemaphore					m_GenHashSemaphore;

    uint16_t					m_u16OfferBaseFileTypes{ 0 };
	VxMutex						m_FileListPacketsMutex;
	std::vector<PktFileListReply*> m_FileListPackets;

    bool						m_OfferBaseListInitialized{ false };
    OfferBaseInfoDb				m_OfferBaseInfoDb;
    std::vector<OfferBaseInfo*>	m_OfferBaseInfoList;
};

