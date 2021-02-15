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
// http://www.nolimitconnect.com
//============================================================================

#include "BlobMgrBase.h"
#include <GoTvInterface/IToGui.h>

#include <CoreLib/VxFileIsTypeFunctions.h>

//============================================================================
BlobMgrBase::BlobMgrBase( P2PEngine& engine )
: m_Engine( engine )
{
}

//============================================================================
IToGui&	BlobMgrBase::getToGui()
{
    return IToGui::getToGui();
}

//============================================================================
void BlobMgrBase::addBlobMgrClient( BlobCallbackInterface * client, bool enable )
{
	AutoResourceLock( this );
	if( enable )
	{
		m_BlobClients.push_back( client );
	}
	else
	{
		std::vector<BlobCallbackInterface *>::iterator iter;
		for( iter = m_BlobClients.begin(); iter != m_BlobClients.end(); ++iter )
		{
			if( *iter == client )
			{
				m_BlobClients.erase( iter );
				return;
			}
		}
	}
}

//============================================================================
bool BlobMgrBase::isAllowedFileOrDir( std::string strFileName )
{
	if( VxIsExecutableFile( strFileName ) 
		|| VxIsShortcutFile( strFileName ) )
	{
		return false;
	}

	return true;
}
