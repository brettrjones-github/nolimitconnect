//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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
// http://www.gotvptop.com
//============================================================================

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>


//============================================================================
void P2PEngine::callbackFileWasShredded( std::string& fileName )
{
}

//============================================================================
void P2PEngine::callbackAssetAdded( AssetInfo * assetInfo )
{
	IToGui::getToGui().toGuiAssetAdded( assetInfo );
}

//============================================================================
void P2PEngine::callbackAssetRemoved( AssetInfo * assetInfo )
{
	IToGui::getToGui().toGuiAssetAction( eAssetActionRemoveFromAssetMgr, assetInfo->getAssetUniqueId(), 0 );
}

//============================================================================
void P2PEngine::callbackSharedFileTypesChanged( uint16_t fileTypes )
{
}

//============================================================================
void P2PEngine::callbackSharedPktFileListUpdated( void )
{
}

//============================================================================
void P2PEngine::callbackAssetHistory( void * /*userData*/, AssetInfo * assetInfo )
{
	IToGui::getToGui().toGuiSessionHistory( assetInfo );
}

