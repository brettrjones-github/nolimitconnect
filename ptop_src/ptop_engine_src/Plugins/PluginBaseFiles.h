#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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


#include "PluginBase.h"
#include "PluginSessionMgr.h"
#include "FileXferMgr.h"
#include "FileInfoXferMgr.h"
#include "FileInfoMgr.h"

#include "SharedFilesMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>

#include <PktLib/VxCommon.h>

class PktFileListReply;
class FileShareSettings;
class SharedFileInfo;
class P2PEngine;
class IToGui;
class VxNetIdent;
class FileTxSession;
class FileRxSession;
class VxFileShredder;

class PluginBaseFiles : public PluginBase
{
public:
	PluginBaseFiles( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType, std::string fileInfoDbName );
	virtual ~PluginBaseFiles() = default;

	PluginSessionBase *			createPluginSession( VxSktBase * sktBase, VxNetIdent * netIdent );
	bool						isFileInLibrary( std::string& fileName );

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiStartPluginSession( VxNetIdent * netIdent = NULL, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( VxNetIdent * netIdent = NULL, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual bool				fromGuiIsPluginInSession( VxNetIdent * netIdent = NULL, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	void						fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
	void						fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );

	void						fromGuiCancelDownload( VxGUID& fileInstance );
	void						fromGuiCancelUpload( VxGUID& fileInstance );

	bool						fromGuiMakePluginOffer( VxNetIdent *	netIdent,				// identity of friend
														int				pvUserData,
														const char *	pOfferMsg,				// offer message
														const char *	pFileName = 0,
														uint8_t *		fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );	

	int							fromGuiPluginControl(	VxNetIdent *	netIdent,
														const char *	pControl, 
														const char *	pAction,
														uint32_t		u32ActionData,
														VxGUID&			lclSessionId,
														uint8_t *		fileHashId );

	virtual bool				fromGuiBrowseFiles(	const char * dir, bool lookupShareStatus, uint8_t fileFilterMask ); 
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId = 0 );
	virtual bool				fromGuiGetIsFileShared( const char * fileName );
	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );
	virtual bool				fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId = 0 );
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );
	virtual bool				fromGuiGetIsFileInLibrary( const char * fileName );

	virtual void				onSharedFilesUpdated( uint16_t u16FileTypes );

	virtual bool				isServingFiles( void );
	void						deleteFile( const char * fileName, bool shredFile );

protected:
	void						onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktFileGetReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileChunkReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetCompleteReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktFindFileReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFindFileReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileListReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileListReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileShareErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

private:
	bool						isFileShared( std::string& fileName );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );


	//=== vars ===//
	VxFileShredder&				m_FileShredder;
	PluginSessionMgr			m_PluginSessionMgr;
    FileInfoMgr				    m_FileInfoMgr;
	FileInfoXferMgr				m_FileInfoXferMgr;
};


