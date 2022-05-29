//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "SktListMgrBase.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
SktListMgrBase::SktListMgrBase( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
void SktListMgrBase::onUpdateIdent( VxGUID& onlineId, int64_t timestamp )
{
    m_Engine.getToGui().toGuiIndentListUpdate( m_ListType, onlineId, timestamp );
}

//============================================================================
void SktListMgrBase::onRemoveIdent( VxGUID& onlineId )
{
    m_Engine.getToGui().toGuiIndentListRemove( m_ListType, onlineId );
}