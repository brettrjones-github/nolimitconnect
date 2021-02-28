//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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

#include "PktTypes.h"
#include "PktsPluginSetting.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PktPluginSettingReq::PktPluginSettingReq()
{
    LogMsg( LOG_DEBUG, "Alive %d grp find %d grp share err %d set req %d", PKT_TYPE_IM_ALIVE_REQ, PKT_TYPE_GROUP_FIND_REQ, PKT_TYPE_GROUP_SHARE_ERR, PKT_TYPE_PLUGIN_SETTING_REQ );
	setPktType( PKT_TYPE_PLUGIN_SETTING_REQ );
    setPktLength( sizeof( PktPluginSettingReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktPluginSettingReply::PktPluginSettingReply()
{
    setPktType( PKT_TYPE_PLUGIN_SETTING_REPLY );
    setPktLength( sizeof( PktPluginSettingReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktPluginSettingReply::calcPktLen( void )
{
    uint16_t pktLen = ( uint16_t)sizeof( PktPluginSettingReply ) - sizeof(PktBlobEntry);
    pktLen += getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktPluginSettingReply::setTotalMatches( uint16_t matchCnt )
{
    m_TotalMatches = htons( matchCnt );
}

//============================================================================
uint16_t PktPluginSettingReply::getTotalMatches( void ) const
{
    return ntohs( m_TotalMatches );
}

//============================================================================
// add guid and plugin type to blob and increment total match count
bool PktPluginSettingReply::addPluginId( const PluginId& pluginId )
{
    bool result = m_BlobEntry.setValue( pluginId );
    if( result )
    {
        setTotalMatches( getTotalMatches() + 1 );
    }

    return result;
}
