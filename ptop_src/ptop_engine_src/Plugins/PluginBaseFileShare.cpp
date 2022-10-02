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
PluginBaseFileShare::PluginBaseFileShare( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, std::string fileInfoDbName )
	: m_FileInfoSharedFilesMgr( engine, *this, fileInfoDbName )
	, PluginBaseFiles( engine, pluginMgr, myIdent, pluginType, m_FileInfoSharedFilesMgr )
{
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
