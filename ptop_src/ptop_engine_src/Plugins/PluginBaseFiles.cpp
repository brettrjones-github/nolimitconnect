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

#include "PluginBaseFiles.h"
#include "PluginMgr.h"
#include "SharedFileInfo.h"

#include "FileLibraryMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktsFileShare.h>
#include <PktLib/PktsPluginOffer.h>
#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileShredder.h>
#include <CoreLib/VxGlobals.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

//============================================================================
PluginBaseFiles::PluginBaseFiles( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType, std::string fileInfoDbName )
: PluginBase( engine, pluginMgr, myIdent, pluginType ) 
, m_FileShredder( GetVxFileShredder() )
, m_PluginSessionMgr( engine, *this, pluginMgr)
, m_FileInfoMgr( engine, *this, fileInfoDbName )
, m_FileInfoXferMgr( engine, *this, m_FileInfoMgr )
{
	setPluginType( pluginType );
}

//============================================================================
void PluginBaseFiles::fromGuiUserLoggedOn( void )
{
	m_FileInfoMgr.fromGuiUserLoggedOn();
	m_FileInfoXferMgr.fromGuiUserLoggedOn();
}

//============================================================================
void PluginBaseFiles::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_FileInfoXferMgr.fromGuiStartPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginBaseFiles::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId)
{
	return m_FileInfoXferMgr.fromGuiStopPluginSession( netIdent, pvUserData, lclSessionId );
}

//============================================================================
bool PluginBaseFiles::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
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
bool PluginBaseFiles::fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
	if( 0 == fileFilterMask )
	{
		fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY;
	}

	std::vector<VxFileInfo> fileList;
	RCODE rc = VxFileUtil::listFilesAndFolders( dir, fileList, fileFilterMask );
	if( rc )
	{
		LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles error %d", rc );
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
				LogMsg( LOG_INFO, "PluginBaseFiles::fromGuiBrowseFiles sending file %s", fileInfo.getFileName().c_str() );
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
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip file type 0x%x because filter mask 0x%x file %s", fileInfo.getFileType(), fileFilterMask, fileInfo.getFileName().c_str() );
			}
		}
		else
		{
			if ( fileInfo.isExecutableFile() )
			{
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip executeable file %s", fileInfo.getFileName().c_str() );
			}
			else
			{
				LogMsg( LOG_ERROR, "PluginBaseFiles::fromGuiBrowseFiles skip shortcut file %s", fileInfo.getFileName().c_str() );
			}
		}
	}

	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false, VxGUID::nullVxGUID() );
	return isPluginEnabled();
}

//============================================================================
bool PluginBaseFiles::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	//m_FileInfoMgr.fromGuiGetSharedFiles( fileTypeFilter );
	return isPluginEnabled();
}

//============================================================================
bool PluginBaseFiles::fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId )
{
	//return m_FileInfoMgr.fromGuiSetFileIsShared( fileName, isShared, fileHashId );
	return false;
}

//============================================================================
bool PluginBaseFiles::fromGuiGetIsFileShared( const char * fileName )
{
	std::string strFileName = fileName;
	return isFileShared( strFileName );
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
bool PluginBaseFiles::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
	return m_FileInfoMgr.fromGuiAddFileToLibrary( fileName, addFile, fileHashId );
}

//============================================================================
void PluginBaseFiles::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	//m_FileInfoMgr.fromGuiGetFileLibraryList(	fileTypeFilter );
	//m_FileInfoMgr.fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool PluginBaseFiles::fromGuiGetIsFileInLibrary( const char * fileName )
{
	std::string strFileName = fileName;
	return isFileInLibrary( strFileName );
}

//============================================================================
bool PluginBaseFiles::isFileShared( std::string& fileName )
{
	return m_FileInfoMgr.isFileShared( fileName );
}

//============================================================================
bool PluginBaseFiles::isFileInLibrary( std::string& fileName )
{
	return m_FileInfoMgr.isFileInLibrary( fileName );
}

//============================================================================
bool PluginBaseFiles::isServingFiles( void )
{
	return ( m_MyIdent->hasSharedFiles() && isPluginEnabled() );
}

//============================================================================
void PluginBaseFiles::deleteFile( const char * fileName, bool shredFile )
{
	std::string strFileName = fileName;
	m_FileInfoXferMgr.fileAboutToBeDeleted( strFileName );
	m_FileInfoMgr.fromGuiSetFileIsShared( strFileName, false );
	m_FileInfoMgr.removeFromLibrary( strFileName );
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
bool PluginBaseFiles::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		
												int				pvUserData,
												const char *	pOfferMsg,		
												const char *	pFileName,
												uint8_t *			fileHashId,
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
int PluginBaseFiles::fromGuiPluginControl(	VxNetIdent *	netIdent,
											const char *	pControl, 
											const char *	pAction,
											uint32_t				u32ActionData,
											VxGUID&			lclSessionId,
											uint8_t *			fileHashId )
{
	return m_FileInfoXferMgr.fromGuiPluginControl(		netIdent,		
														pControl, 
														pAction,
														u32ActionData,
														lclSessionId,
														fileHashId );
}

//============================================================================
void PluginBaseFiles::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFindFileReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFindFileReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileListReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_FileInfoXferMgr.onPktFileListReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::onPktFileShareErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//m_FileInfoXferMgr.onPktFileShareErr( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseFiles::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginBaseFiles::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginBaseFiles::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}
