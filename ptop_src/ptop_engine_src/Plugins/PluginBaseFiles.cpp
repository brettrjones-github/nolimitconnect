//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "PluginBaseFiles.h"

#include "FileInfoBaseMgr.h"
#include "PluginMgr.h"

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
PluginBaseFiles::PluginBaseFiles( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, FileInfoBaseMgr& fileInfoBaseMgr )
: PluginBase( engine, pluginMgr, myIdent, pluginType ) 
, m_FileShredder( GetVxFileShredder() )
, m_PluginSessionMgr( engine, *this, pluginMgr)
, m_FileInfoMgr( fileInfoBaseMgr )
, m_FileInfoXferMgr( m_FileInfoMgr.getFileInfoXferMgr() )
{
	setPluginType( pluginType );
	LogMsg( LOG_VERBOSE, "PluginBaseFiles::PluginBaseFiles %s %p", DescribePluginType( pluginType ), this );
}

//============================================================================
void PluginBaseFiles::onAfterUserLogOnThreaded( void )
{
	m_FileInfoMgr.onAfterUserLogOnThreaded();
	m_FileInfoXferMgr.onAfterUserLogOnThreaded();
}

//============================================================================
void PluginBaseFiles::fromGuiStartPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_FileInfoXferMgr.fromGuiStartPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginBaseFiles::fromGuiStopPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId)
{
	return m_FileInfoXferMgr.fromGuiStopPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
bool PluginBaseFiles::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_FileInfoXferMgr.fromGuiIsPluginInSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginBaseFiles::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_FileInfoXferMgr.fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void PluginBaseFiles::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_FileInfoXferMgr.fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
void PluginBaseFiles::fromGuiCancelDownload( VxGUID& fileInstance )
{
	return m_FileInfoXferMgr.fromGuiCancelDownload( fileInstance );
}

//============================================================================
void PluginBaseFiles::fromGuiCancelUpload( VxGUID& fileInstance )
{
	return m_FileInfoXferMgr.fromGuiCancelUpload( fileInstance );
}

//============================================================================
bool PluginBaseFiles::fromGuiBrowseFiles( std::string& dir, uint8_t fileFilterMask )
{
	if( 0 == fileFilterMask )
	{
		fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY;
	}

	std::vector<VxFileInfo> fileList;
	RCODE rc = VxFileUtil::listFilesAndFolders( dir.c_str(), fileList, fileFilterMask );
	if( rc )
	{
		LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles error %d", rc );
		return false;
	}

	std::vector<VxFileInfo>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		VxFileInfo& vxFileInfo = *iter;
		if ( ( false == vxFileInfo.isExecutableFile() )
			&& ( false == vxFileInfo.isShortcutFile() ) )
		{
			if ( 0 != ( fileFilterMask & vxFileInfo.getFileType() ) )
			{
				LogMsg( LOG_INFO, "PluginBaseFiles::fromGuiBrowseFiles sending file %s", vxFileInfo.getFileName().c_str() );
				bool isShared = m_Engine.fromGuiGetIsFileShared( vxFileInfo.getFileName() );
				bool isInLibrary = m_Engine.fromGuiGetIsFileInLibrary( vxFileInfo.getFileName() );

				FileInfo fileInfo( m_Engine.getMyOnlineId(), vxFileInfo.getFileName(), vxFileInfo.getFileLength(),
					vxFileInfo.getFileType(), VxGUID::nullVxGUID() );
				fileInfo.setIsInLibrary( isInLibrary );
				fileInfo.setIsSharedFile( isShared );

				IToGui::getToGui().toGuiFileList( fileInfo );
			}
			else
			{
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip file type 0x%x because filter mask 0x%x file %s", 
					vxFileInfo.getFileType(), fileFilterMask, vxFileInfo.getFileName().c_str() );
			}
		}
		else
		{
			if ( vxFileInfo.isExecutableFile() )
			{
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip executeable file %s", vxFileInfo.getFileName().c_str() );
			}
			else
			{
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip shortcut file %s", vxFileInfo.getFileName().c_str() );
			}
		}
	}

	IToGui::getToGui().toGuiFileListCompleted();
	return isPluginEnabled();
}

//============================================================================
bool PluginBaseFiles::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	return m_FileInfoMgr.fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool PluginBaseFiles::fromGuiSetFileIsShared( FileInfo& fileInfo, bool isShared )
{
	return m_FileInfoMgr.fromGuiSetFileIsShared( fileInfo, isShared );
}

//============================================================================
bool PluginBaseFiles::fromGuiSetFileIsShared( std::string& fileName, bool isShared )
{
	return m_FileInfoMgr.fromGuiSetFileIsShared( fileName, isShared );
}

//============================================================================
bool PluginBaseFiles::fromGuiRemoveSharedFile( std::string& fileName )
{
	return m_FileInfoMgr.fromGuiRemoveSharedFile( fileName );
}

//============================================================================
bool PluginBaseFiles::fromGuiGetIsFileShared( std::string& fileName )
{
	return isFileShared( fileName );
}

//============================================================================
// returns -1 if unknown else percent downloaded
int PluginBaseFiles::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	return -1;
	int result = m_FileInfoMgr.fromGuiGetFileDownloadState( fileHashId );
	if( -1 == result )
	{
		result = m_FileInfoXferMgr.fromGuiGetFileDownloadState( fileHashId );
	}

	return result;
}

//============================================================================
bool PluginBaseFiles::isFileShared( std::string& fileName )
{
	return m_FileInfoMgr.isFileShared( fileName );
}

//============================================================================
bool PluginBaseFiles::isServingFiles( void )
{
	return ( m_MyIdent->hasSharedFiles() && isPluginEnabled() );
}

//============================================================================
void PluginBaseFiles::deleteFile( const char* fileName, bool shredFile )
{
	std::string strFileName = fileName;
	m_FileInfoXferMgr.fileAboutToBeDeleted( strFileName );
	m_FileInfoMgr.removeFromDbAndList( strFileName );
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
void PluginBaseFiles::onSharedFilesUpdated( uint16_t u16FileTypes )
{
	if( m_MyIdent->getSharedFileTypes() != u16FileTypes )
	{
		m_MyIdent->setSharedFileTypes( (uint8_t)u16FileTypes );
		m_Engine.doPktAnnHasChanged( false );
	}
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginBaseFiles::fromGuiMakePluginOffer(	VxNetIdent*	netIdent,		
												int				pvUserData,
												const char*	pOfferMsg,		
												const char*	pFileName,
												uint8_t *		fileHashId,
												VxGUID			lclSessionId )		
{
	return m_FileInfoXferMgr.fromGuiMakePluginOffer(	netIdent,		
														pvUserData,
														pOfferMsg,		
														pFileName,
														fileHashId,
														lclSessionId );
}

//============================================================================
EXferError PluginBaseFiles::fromGuiFileXferControl( VxNetIdent* netIdent, EXferAction xferAction, FileInfo& fileInfo )
{
	return m_FileInfoXferMgr.fromGuiFileXferControl( netIdent, xferAction, fileInfo );
}

//============================================================================
void PluginBaseFiles::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFindFileReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFindFileReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFindFileReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFindFileReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileListReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileListReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileListReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_FileInfoXferMgr.onPktFileListReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileShareErr( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_FileInfoXferMgr.onPktFileShareErr( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginBaseFiles::onConnectionLost( VxSktBase* sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginBaseFiles::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

//============================================================================
void PluginBaseFiles::onPktFileInfoInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoInfoReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseFiles::onPktFileInfoInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoInfoReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseFiles::onPktFileInfoAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoAnnReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseFiles::onPktFileInfoAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//m_Engine.getFileInfoListMgr().onPktFileInfoAnnReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseFiles::onPktFileInfoSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "PluginBaseFiles::onPktFileInfoSearchReq rxed" );

	EPluginAccess pluginAccess = getPluginAccessState( netIdent );
	ECommErr commErr = getCommAccessState( netIdent );

	PktFileInfoSearchReq* pktReq = ( PktFileInfoSearchReq* )pktHdr;
	if( pktReq && pktReq->isValidPkt() )
	{
		uint8_t searchFileTypes = pktReq->getSearchFileTypes();
		PktBlobEntry& blobEntry = pktReq->getBlobEntry();
		blobEntry.resetRead();
		std::string searchText;
		if( pktReq->getSearchText( searchText ) || searchFileTypes )
		{
			PktFileInfoSearchReply pktReply;
			pktReply.setSearchSessionId( pktReq->getSearchSessionId() );
			pktReply.setHostOnlineId( pktReq->getHostOnlineId() );
			pktReply.setSearchFileTypes( searchFileTypes );
			pktReply.setSearchText( searchText );
			
			pktReply.setAccessState( pluginAccess );
			pktReply.setCommError( commErr );
			if( ePluginAccessOk == pluginAccess && eCommErrNone == commErr )
			{
				if( !searchText.empty() && searchText.size() < FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFiles::onPktFileInfoSearchReq search text too short" );
					pktReply.setCommError( eCommErrSearchTextToShort );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReq to short search text" );
				}
				else if( searchText.size() > FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFiles::onPktFileInfoSearchReq search text too long" );
					pktReply.setCommError( eCommErrSearchTextToLong );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReq to long search text" );
				}
				else
				{
					ECommErr searchErr = m_FileInfoMgr.searchRequest( pktReply, pktReq->getSpecificAssetId(), searchText, searchFileTypes, sktBase, netIdent );
					pktReply.setCommError( searchErr );
				}
			}
			else
			{
				LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseFiles::onPktFileInfoSearchReq service not enabled" );
				pktReply.setCommError( eCommErrPluginNotEnabled );
			}

			pktReply.calcPktLen();
			if( !txPacket( netIdent->getMyOnlineId(), sktBase, &pktReply, false ) )
			{
				LogModule( eLogHostSearch, LOG_VERBOSE, "PluginBaseFiles::onPktFileInfoSearchReq failed send search reply" );
			}
		}
		else
		{
			LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseHostService invalid search packet" );
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReply invalid search text" );
		}	
	}
	else
	{
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReq invalid ptk" );
	}
}

//============================================================================
void PluginBaseFiles::onPktFileInfoSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktFileInfoSearchReply* pktReply = ( PktFileInfoSearchReply* )pktHdr;

	if( pktReply && pktReply->isValidPkt() )
	{
		uint8_t searchFileTypes = pktReply->getSearchFileTypes();
		PktBlobEntry& blobEntry = pktReply->getBlobEntry();
		blobEntry.resetRead();
		std::string searchText;
		if( pktReply->getCommError() == eCommErrNone )
		{
			if( pktReply->getSearchText( searchText ) || searchFileTypes )
			{
				if( updateFromFileInfoSearchBlob( pktReply->getSearchSessionId(), pktReply->getHostOnlineId(), sktBase, netIdent, pktReply->getBlobEntry(), pktReply->getFileInfoCountThisPkt() ) )
				{
					if( pktReply->getMoreFileInfosExist() )
					{
						if( !requestMoreFileInfoFromServer( pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getNextSearchAssetId(), searchText ) )
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
					fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrInvalidParam );
				}
			}
			else
			{
				logCommError( eCommErrInvalidPkt, "PktFileInfoSearchReply", sktBase, netIdent );
				fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrInvalidPkt );
				VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReply invalid search text" );
			}
		}
		else
		{
			logCommError( pktReply->getCommError(), "PktFileInfoSearchReply", sktBase, netIdent );
			fileInfoSearchCompleted( pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getCommError() );
		}
	}
	else
	{
		VxGUID nullGuid;
		fileInfoSearchCompleted( nullGuid, sktBase, netIdent, eCommErrInvalidPkt );
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReply invalid ptk" );
	}
}

//============================================================================
void PluginBaseFiles::onPktFileInfoMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktFileInfoMoreReq* pktReq = ( PktFileInfoMoreReq* )pktHdr;
	if( pktReq && pktReq->isValidPkt() )
	{
		uint8_t searchFileTypes = pktReq->getSearchFileTypes();
		PktBlobEntry& blobEntry = pktReq->getBlobEntry();
		blobEntry.resetRead();
		std::string searchText;
		ECommErr commErr = getCommAccessState( netIdent );
		PktFileInfoMoreReply pktReply;
		pktReply.setCommError( commErr );
		if( pktReq->getSearchText( searchText ) || searchFileTypes )
		{
			pktReply.setSearchFileTypes( searchFileTypes );
			pktReply.setSearchText( searchText );
			EHostType hostType = pktReq->getHostType();
			pktReply.setHostType( hostType );
			VxGUID nextSearchOnlineId = pktReq->getNextSearchAssetId();
			pktReply.setSearchSessionId( pktReq->getSearchSessionId() );

			if( eCommErrNone == commErr )
			{				
				if( !searchText.empty() && searchText.size() < FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFiles::onPktFileInfoMoreReq search text too short" );
					pktReply.setCommError( eCommErrSearchTextToShort );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoMoreReq to short search text" );
				}
				else if( searchText.size() > FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
				{
					LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFiles::onPktFileInfoMoreReq search text too long" );
					pktReply.setCommError( eCommErrSearchTextToLong );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoMoreReq to long search text" );
				}
				else
				{
					if( nextSearchOnlineId.isVxGUIDValid() )
					{
						ECommErr searchErr = m_FileInfoMgr.searchMoreRequest( pktReply, nextSearchOnlineId, searchText, searchFileTypes, sktBase, netIdent );
						pktReply.setCommError( searchErr );
					}
					else
					{
						LogModule( eLogHostSearch, LOG_ERROR, "PluginBaseFiles::onPktFileInfoMoreReq search text too long" );
						pktReply.setCommError( eCommErrInvalidParam );
						VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoMoreReq to long search text" );
					}
				}
			}

			pktReply.calcPktLen();
			txPacket( netIdent, sktBase, &pktReply );
		}
		else
		{
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoMoreReq invalid search text" );
		}
	}
	else
	{
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoMoreReq invalid ptk" );
	}
}

//============================================================================
void PluginBaseFiles::onPktFileInfoMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktFileInfoMoreReply* pktReply = ( PktFileInfoMoreReply* )pktHdr;
	if( pktReply && pktReply->isValidPkt() )
	{
		std::string searchStr;
		if( pktReply->getCommError() )
		{
			logCommError( pktReply->getCommError(), "PluginBaseFiles::onPktFileInfoMoreReply", sktBase, netIdent );
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
			VxReportHack( eHackerLevelSuspicious, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReply invalid search text" );
		}
	}
	else
	{
		VxGUID nullGuid;
		fileInfoSearchCompleted( nullGuid, sktBase, netIdent, eCommErrInvalidPkt );
		VxReportHack( eHackerLevelSevere, eHackerReasonInvalidPkt, sktBase, "PluginBaseFiles::onPktFileInfoSearchReply invalid ptk" );
	}
}

//============================================================================
bool PluginBaseFiles::updateFromFileInfoSearchBlob( VxGUID& searchSessionId, VxGUID& hostOnlineId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int fileInfoCount )
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
bool PluginBaseFiles::requestMoreFileInfoFromServer(  VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextFileInfoAssetId, std::string& searchText )
{
	PktFileInfoMoreReq pktReq;
	pktReq.setSearchSessionId( searchSessionId );
	pktReq.setNextSearchAssetId( nextFileInfoAssetId );
	pktReq.setSearchText( searchText );
	pktReq.calcPktLen();
	return txPacket( netIdent, sktBase, &pktReq );
}

//============================================================================
ECommErr PluginBaseFiles::searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return m_FileInfoMgr.searchRequest( pktReply, specificAssetId, searchStr, searchFileTypes, sktBase, netIdent );
}

//============================================================================
ECommErr PluginBaseFiles::searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return m_FileInfoMgr.searchMoreRequest( pktReply, nextFileAssetId, searchStr, searchFileTypes, sktBase, netIdent );
}

//============================================================================
void PluginBaseFiles::sendFileSearchResultToGui( VxGUID& searchSessionId, VxNetIdent* netIdent, FileInfo& fileInfo )
{
	m_FileInfoMgr.sendFileSearchResultToGui( searchSessionId, netIdent, fileInfo );
}