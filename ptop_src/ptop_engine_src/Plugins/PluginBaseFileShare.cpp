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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBaseFileShare.h"
#include "PluginMgr.h"
#include "SharedFileInfo.h"

#include "FileLibraryMgr.h"

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktsFileShare.h>
#include <PktLib/PktsPluginOffer.h>
#include <PktLib/VxSearchDefs.h>
#include <PktLib/PktsFileInfo.h>
#include <PktLib/SearchParams.h>
#include <PktLib/VxCommon.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileShredder.h>
#include <CoreLib/VxGlobals.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

//============================================================================
PluginBaseFileShare::PluginBaseFileShare( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, std::string fileLibraryDbName, std::string fileShareDbName )
: PluginBase( engine, pluginMgr, myIdent, pluginType ) 
, m_PluginSessionMgr( engine, *this, pluginMgr)
, m_SharedFilesMgr( engine, *this, fileShareDbName )
, m_FileLibraryMgr( engine, *this, m_SharedFilesMgr, fileLibraryDbName )
, m_FileShareXferMgr( engine, *this, m_SharedFilesMgr, m_FileLibraryMgr )
, m_FileShredder( GetVxFileShredder() )
{
	setPluginType( pluginType );
}

//============================================================================
void PluginBaseFileShare::fromGuiUserLoggedOn( void )
{
	m_FileLibraryMgr.fromGuiUserLoggedOn();
	m_SharedFilesMgr.fromGuiUserLoggedOn();
	m_FileShareXferMgr.fromGuiUserLoggedOn();
}

//============================================================================
void PluginBaseFileShare::fromGuiStartPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_FileShareXferMgr.fromGuiStartPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginBaseFileShare::fromGuiStopPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId)
{
	return m_FileShareXferMgr.fromGuiStopPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
bool PluginBaseFileShare::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_FileShareXferMgr.fromGuiIsPluginInSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginBaseFileShare::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_FileShareXferMgr.fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void PluginBaseFileShare::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_FileShareXferMgr.fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
void PluginBaseFileShare::fromGuiCancelDownload( VxGUID& fileInstance )
{
	return m_FileShareXferMgr.fromGuiCancelDownload( fileInstance );
}

//============================================================================
void PluginBaseFileShare::fromGuiCancelUpload( VxGUID& fileInstance )
{
	return m_FileShareXferMgr.fromGuiCancelUpload( fileInstance );
}

//============================================================================
bool PluginBaseFileShare::fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
	if( 0 == fileFilterMask )
	{
		fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY;
	}

	std::vector<VxFileInfo> fileList;
	RCODE rc = VxFileUtil::listFilesAndFolders( dir, fileList, fileFilterMask );
	if( rc )
	{
		LogMsg( LOG_ERROR, "PluginBaseFileShare::fromGuiBrowseFiles error %d", rc );
		return false;
	}

	std::vector<VxFileInfo>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		VxFileInfo& fileInfo = *iter;
		if ( ( false == fileInfo.isExecutableFile() )
			&& ( false == fileInfo.isShortcutFile() ) )
		{
			if ( 0 != ( fileFilterMask & fileInfo.getFileType() ) )
			{
				LogMsg( LOG_INFO, "PluginBaseFileShare::fromGuiBrowseFiles sending file %s", fileInfo.getFileName().c_str() );
				IToGui::getToGui().toGuiFileList(	fileInfo.getFileName().c_str(), 
					fileInfo.getFileLength(), 
					fileInfo.getFileType(), 
					isFileShared( fileInfo.getFileName() ),
					isFileInLibrary( fileInfo.getFileName() ),
					VxGUID::nullVxGUID(),
					0 );
			}
			else
			{
				LogMsg( LOG_ERROR, "PluginBaseFileShare::fromGuiBrowseFiles skip file type 0x%x because filter mask 0x%x file %s", fileInfo.getFileType(), fileFilterMask, fileInfo.getFileName().c_str() );
			}
		}
		else
		{
			if ( fileInfo.isExecutableFile() )
			{
				LogMsg( LOG_ERROR, "PluginBaseFileShare::fromGuiBrowseFiles skip executeable file %s", fileInfo.getFileName().c_str() );
			}
			else
			{
				LogMsg( LOG_ERROR, "PluginBaseFileShare::fromGuiBrowseFiles skip shortcut file %s", fileInfo.getFileName().c_str() );
			}
		}
	}

	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false, VxGUID::nullVxGUID() );
	return isPluginEnabled();
}

//============================================================================
bool PluginBaseFileShare::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	m_SharedFilesMgr.fromGuiGetSharedFiles( fileTypeFilter );
	return isPluginEnabled();
}

//============================================================================
bool PluginBaseFileShare::fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId )
{
	return m_SharedFilesMgr.fromGuiSetFileIsShared( fileName, isShared, fileHashId );
}

//============================================================================
bool PluginBaseFileShare::fromGuiGetIsFileShared( const char * fileName )
{
	std::string strFileName = fileName;
	return isFileShared( strFileName );
}

//============================================================================
// returns -1 if unknown else percent downloaded
int PluginBaseFileShare::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	return -1;
	int result = m_FileLibraryMgr.fromGuiGetFileDownloadState( fileHashId );
	if( -1 == result )
	{
		result = m_FileShareXferMgr.fromGuiGetFileDownloadState( fileHashId );
	}

	return result;
}

//============================================================================
bool PluginBaseFileShare::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
	return m_FileLibraryMgr.fromGuiAddFileToLibrary( fileName, addFile, fileHashId );
}

//============================================================================
void PluginBaseFileShare::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	m_FileLibraryMgr.fromGuiGetFileLibraryList(	fileTypeFilter );
	m_SharedFilesMgr.fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool PluginBaseFileShare::fromGuiGetIsFileInLibrary( const char * fileName )
{
	std::string strFileName = fileName;
	return isFileInLibrary( strFileName );
}

//============================================================================
bool PluginBaseFileShare::isFileShared( std::string& fileName )
{
	return m_SharedFilesMgr.isFileShared( fileName );
}

//============================================================================
bool PluginBaseFileShare::isFileInLibrary( std::string& fileName )
{
	return m_FileLibraryMgr.isFileInLibrary( fileName );
}

//============================================================================
bool PluginBaseFileShare::isServingFiles( void )
{
	return ( m_MyIdent->hasSharedFiles() && isPluginEnabled() );
}

//============================================================================
void PluginBaseFileShare::deleteFile( const char * fileName, bool shredFile )
{
	std::string strFileName = fileName;
	m_FileShareXferMgr.fileAboutToBeDeleted( strFileName );
	m_SharedFilesMgr.fromGuiSetFileIsShared( strFileName, false );
	m_FileLibraryMgr.removeFromLibrary( strFileName );
	if( shredFile )
	{
		m_FileShredder.shredFile( strFileName );
	}
	else
	{
		VxFileUtil::deleteFile( strFileName.c_str() );	
	}
}

//============================================================================
void PluginBaseFileShare::onSharedFilesUpdated( uint16_t u16FileTypes )
{
	if( m_MyIdent->getSharedFileTypes() != u16FileTypes )
	{
		m_MyIdent->setSharedFileTypes( (uint8_t)u16FileTypes );
		m_Engine.doPktAnnHasChanged( false );
	}
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginBaseFileShare::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		
												int				pvUserData,
												const char *	pOfferMsg,		
												const char *	pFileName,
												uint8_t *			fileHashId,
												VxGUID			lclSessionId )		
{
	return m_FileShareXferMgr.fromGuiMakePluginOffer(	netIdent,		
														pvUserData,
														pOfferMsg,		
														pFileName,
														fileHashId,
														lclSessionId );
}

//============================================================================
int PluginBaseFileShare::fromGuiPluginControl(	VxNetIdent *	netIdent,
											const char *	pControl, 
											const char *	pAction,
											uint32_t				u32ActionData,
											VxGUID&			lclSessionId,
											uint8_t *			fileHashId )
{
	return m_FileShareXferMgr.fromGuiPluginControl(		netIdent,		
														pControl, 
														pAction,
														u32ActionData,
														lclSessionId,
														fileHashId );
}

//============================================================================
void PluginBaseFileShare::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFindFileReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFindFileReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFindFileReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFindFileReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileListReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileListReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileListReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileShareXferMgr.onPktFileListReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::onPktFileShareErr( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_FileShareXferMgr.onPktFileShareErr( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFileShare::replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginBaseFileShare::onConnectionLost( VxSktBase* sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginBaseFileShare::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

//============================================================================
void PluginBaseFileShare::updateSharedFilesInfo( void )
{
	m_SharedFilesMgr.updateFileTypes();
}


//============================================================================
void PluginBaseFileShare::onPktFileInfoInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoInfoReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoInfoReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoAnnReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoAnnReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "PluginBaseFileShare::onPktFileInfoSearchReq rxed" );

	PktFileInfoSearchReq* pktReq = (PktFileInfoSearchReq*)pktHdr;
	if( pktReq && pktReq->isValidPkt() )
	{
		PktBlobEntry& blobEntry = pktReq->getBlobEntry();
		blobEntry.resetRead();
		std::string searchText;
		if( pktReq->getSearchText( searchText ) )
		{
			PktFileInfoSearchReply pktReply;
			pktReply.setSearchSessionId( pktReq->getSearchSessionId() );
			pktReply.setHostOnlineId( pktReq->getHostOnlineId() );

			pktReply.setSearchText( searchText );
			EPluginAccess pluginAccess = getPluginAccessState( netIdent );
			pktReply.setAccessState( pluginAccess );
			if( ePluginAccessOk == pluginAccess )
			{
				if( !searchText.empty() && searchText.size() < FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFileShare::onPktFileInfoSearchReq search text too short" );
					pktReply.setCommError( eCommErrSearchTextToShort );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReq to short search text" );
				}
				else if( searchText.size() > FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFileShare::onPktFileInfoSearchReq search text too long" );
					pktReply.setCommError( eCommErrSearchTextToLong );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReq to long search text" );
				}
				else
				{
					// BRJ ECommErr searchErr = m_FileInfoMgr.searchRequest( pktReply, pktReq->getSpecificAssetId(), searchText, sktBase, netIdent );
					// pktReply.setCommError( searchErr );
				}
			}
			else
			{
				LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseFileShare::onPktFileInfoSearchReq service not enabled" );
				pktReply.setCommError( eCommErrPluginNotEnabled );
			}

			pktReply.calcPktLen();
			if( !txPacket( netIdent->getMyOnlineId(), sktBase, &pktReply, false ) )
			{
				LogModule( eLogHostSearch, LOG_VERBOSE, "PluginBaseFileShare::onPktFileInfoSearchReq failed send search reply" );
			}
		}
		else
		{
			LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseHostService invalid search packet" );
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReply invalid search text" );
		}
	}
	else
	{
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReq invalid ptk" );
	}
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReply should not be recieved ptk" );
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	/*
	PktFileInfoMoreReq* pktReq = (PktFileInfoMoreReq*)pktHdr;
	if( pktReq && pktReq->isValidPkt() )
	{
		PktBlobEntry& blobEntry = pktReq->getBlobEntry();
		blobEntry.resetRead();
		std::string searchText;
		ECommErr commErr = getCommAccessState( netIdent );
		PktFileInfoMoreReply pktReply;
		pktReply.setCommError( commErr );
		if( pktReq->getSearchText( searchText ) )
		{
			pktReply.setSearchText( searchText );
			EHostType hostType = pktReq->getHostType();
			pktReply.setHostType( hostType );
			VxGUID nextSearchOnlineId = pktReq->getNextSearchAssetId();
			pktReply.setSearchSessionId( pktReq->getSearchSessionId() );

			if( eCommErrNone == commErr )
			{
				if( !searchText.empty() && searchText.size() < FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFileShare::onPktFileInfoMoreReq search text too short" );
					pktReply.setCommError( eCommErrSearchTextToShort );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoMoreReq to short search text" );
				}
				else if( searchText.size() > FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFileShare::onPktFileInfoMoreReq search text too long" );
					pktReply.setCommError( eCommErrSearchTextToLong );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoMoreReq to long search text" );
				}
				else
				{
					if( nextSearchOnlineId.isVxGUIDValid() )
					{
						ECommErr searchErr = m_FileInfoMgr.searchMoreRequest( pktReply, nextSearchOnlineId, searchText, sktBase, netIdent );
						pktReply.setCommError( searchErr );
					}
					else
					{
						LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFileShare::onPktFileInfoMoreReq search text too long" );
						pktReply.setCommError( eCommErrSearchTextToLong );
						VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoMoreReq to long search text" );
					}
				}

				commErr = m_FileInfoMgr.searchMoreRequest( pktReply, nextSearchOnlineId, searchText, sktBase, netIdent );
			}

			pktReply.calcPktLen();
			txPacket( netIdent, sktBase, &pktReply );
		}
		else
		{
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoMoreReq invalid search text" );
		}
	}
	else
	{
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoMoreReq invalid ptk" );
	}
	*/
}

//============================================================================
void PluginBaseFileShare::onPktFileInfoMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	/*x
	PktFileInfoMoreReply* pktReply = (PktFileInfoMoreReply*)pktHdr;
	if( pktReply && pktReply->isValidPkt() )
	{
		std::string searchStr;
		if( pktReply->getCommError() )
		{
			logCommError( pktReply->getCommError(), "PluginBaseFileShare::onPktFileInfoMoreReply", sktBase, netIdent );
			fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getCommError() );
		}
		else if( pktReply->getSearchText( searchStr ) )
		{
			VxGUID hostOnlineId = pktReply->getDestOnlineId();
			updateFromFileInfoSearchBlob( pktReply->getSearchSessionId(), hostOnlineId, sktBase, netIdent, pktReply->getBlobEntry(), pktReply->getFileInfoCountThisPkt() );
			if( pktReply->getMoreFileInfosExist() )
			{
				if( !requestMoreFileInfoFromServer( pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getNextSearchAssetId(), searchStr ) )
				{
					fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrUserOffline );
				}
			}
			else
			{
				fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrNone );
			}
		}
		else
		{
			fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrInvalidPkt );
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReply invalid search text" );
		}
	}
	else
	{
		VxGUID nullGuid;
		fileInfoSearchCompleted( nullGuid, sktBase, netIdent, eCommErrInvalidPkt );
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFileShare::onPktFileInfoSearchReply invalid ptk" );
	}
	*/
}
/*
//============================================================================
bool PluginBaseFileShare::updateFromFileInfoSearchBlob( VxGUID& searchSessionId, VxGUID& hostOnlineId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int fileInfoCount )
{
	// assumes blobEntry.resetRead(); has been called and any procceeding values like search text has been extracted
	bool result{ true };
	for( int i = 0; i < fileInfoCount; i++ )
	{
		FileInfo fileIInfo;
		if( fileIInfo.extractFromBlob( blobEntry ) )
		{
			result &= fileInfoSearchResult( searchSessionId, sktBase, netIdent, fileIInfo );
			if( !result )
			{
				break;
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "Could not extract FileInfoListMgr::updateFromFileInfoSearchBlob" );
			result = false;
			break;
		}
	}

	return result;
}

//============================================================================
bool PluginBaseFileShare::requestMoreFileInfoFromServer( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextFileInfoAssetId, std::string& searchText )
{
	PktFileInfoMoreReq pktReq;
	pktReq.setSearchSessionId( searchSessionId );
	pktReq.setNextSearchAssetId( nextFileInfoAssetId );
	pktReq.setSearchText( searchText );
	pktReq.calcPktLen();
	return txPacket( netIdent, sktBase, &pktReq );
}

//============================================================================
ECommErr PluginBaseFileShare::searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return m_FileInfoMgr.searchRequest( pktReply, specificAssetId, searchStr, sktBase, netIdent );
}

//============================================================================
ECommErr PluginBaseFileShare::searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return m_FileInfoMgr.searchMoreRequest( pktReply, nextFileAssetId, searchStr, sktBase, netIdent );
}


//============================================================================
void PluginBaseFileShare::toGuiFileXferState( VxGUID& localSessionId, EXferState xferState, EXferError xferErr, int param )
{

}
*/