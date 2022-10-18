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

#include "PluginFileShareServer.h"
#include "PluginMgr.h"

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
PluginFileShareServer::PluginFileShareServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBaseFilesServer( engine, pluginMgr, myIdent, pluginType, "FileShareService.db3" )
{
	setPluginType( ePluginTypeFileShareServer );
}

//============================================================================
void PluginFileShareServer::onNetworkConnectionReady( bool requiresRelay )
{
	if( isPluginEnabled() )
	{
		updateSharedFilesInfo();
	}
}

//============================================================================
void PluginFileShareServer::updateSharedFilesInfo( void )
{
	getFileInfoMgr().updateFileTypes();
}

//============================================================================
void PluginFileShareServer::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	m_Engine.lockAnnouncePktAccess();
	PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();
	if( pktAnn.getSharedFileTypes() != fileTypes )
	{
		pktAnn.setSharedFileTypes( fileTypes );
		m_Engine.unlockAnnouncePktAccess();
		if( isPluginEnabled() )
		{
			m_Engine.doPktAnnHasChanged( false );
		}
	}
	else
	{
		m_Engine.unlockAnnouncePktAccess();
	}
}

//============================================================================
void PluginFileShareServer::deleteFile( std::string fileName, bool shredFile )
{
	m_Engine.getPluginLibraryServer().fromGuiSetFileIsInLibrary( fileName, false );
	PluginBaseFilesServer::deleteFile( fileName, shredFile );
}