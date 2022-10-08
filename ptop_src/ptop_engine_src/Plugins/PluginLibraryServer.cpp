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

#include "PluginLibraryServer.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginFileShareServer.h>
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
PluginLibraryServer::PluginLibraryServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBaseFilesServer( engine, pluginMgr, myIdent, pluginType, "LibraryService.db3" )
{
	setPluginType( ePluginTypeLibraryServer );
}

//============================================================================
void PluginLibraryServer::onNetworkConnectionReady( bool requiresRelay )
{
	if( isPluginEnabled() )
	{
		updateSharedFilesInfo();
	}
}

//============================================================================
void PluginLibraryServer::updateSharedFilesInfo( void )
{
	getFileInfoMgr().updateFileTypes();
}

//============================================================================
void PluginLibraryServer::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{

}

//============================================================================
bool PluginLibraryServer::fromGuiSetFileIsInLibrary( FileInfo& fileInfo, bool inLibrary )
{
	return fromGuiSetFileIsShared( fileInfo, inLibrary );
	fileInfo.setIsInLibrary( inLibrary );
	if( inLibrary )
	{
		return m_FileInfoMgr.addFileToDbAndList( fileInfo );
	}
	else
	{
		return m_FileInfoMgr.removeFromDbAndList( fileInfo.getFullFileName() );
	}
}

//============================================================================
bool PluginLibraryServer::fromGuiSetFileIsInLibrary( std::string& fileName, bool inLibrary )
{
	return fromGuiSetFileIsShared( fileName, inLibrary );
}

//============================================================================
void PluginLibraryServer::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	m_FileInfoMgr.fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool PluginLibraryServer::fromGuiGetIsFileInLibrary( std::string& fileName )
{
	return isFileInLibrary( fileName );
}

//============================================================================
bool PluginLibraryServer::fromGuiRemoveFromLibrary( std::string& fileName )
{
	return m_FileInfoMgr.removeFromDbAndList( fileName );
}

//============================================================================
bool PluginLibraryServer::isFileInLibrary( std::string& fileName )
{
	return m_FileInfoMgr.isFileShared( fileName );
}

//============================================================================
void PluginLibraryServer::deleteFile( const char* fileName, bool shredFile )
{
	PluginBaseFilesServer::deleteFile( fileName, shredFile );
	m_Engine.getPluginFileShareServer().deleteFile( fileName, shredFile );
}