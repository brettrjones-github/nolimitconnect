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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/BlobXferMgr/BlobInfo.h>

//============================================================================
void P2PEngine::callbackBlobAdded( BlobInfo* blobInfo )
{
    IToGui::getToGui().toGuiBlobAdded( blobInfo );
}

//============================================================================
void P2PEngine::callbackBlobRemoved( BlobInfo* blobInfo )
{
    IToGui::getToGui().toGuiBlobAction( eAssetActionRemoveFromAssetMgr, blobInfo->getAssetUniqueId(), 0 );
}

//============================================================================
void P2PEngine::callbackBlobHistory( BlobInfo* blobInfo )
{
	IToGui::getToGui().toGuiBlobSessionHistory( blobInfo );
}
