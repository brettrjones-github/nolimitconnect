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
// http://www.nolimitconnect.org
//============================================================================

#include "PktTypes.h"
#include "PktsPluginSetting.h"

#include <string.h>

//============================================================================
PktPluginSettingReq::PktPluginSettingReq()
{
	setPktType( PKT_TYPE_PLUGIN_SETTING_REQ );
    setPktLength( sizeof( PktPluginSettingReq ) );
}

//============================================================================
PktPluginSettingReply::PktPluginSettingReply()
{
    setPktType( PKT_TYPE_PLUGIN_SETTING_REPLY );
}

//============================================================================
void PktPluginSettingReply::calcPktLen( void )
{
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktPluginSettingReply ) - ( MAX_PLUGIN_SETTING_STORAGE_LEN + 16 ) + getSettingBinary()->getSettingTotalStorgeLength() ) );
}

//============================================================================
bool PktPluginSettingReply::getSettingBinary( BinaryBlob& binarySetting )
{
    return settingBinary.setBlobData( m_SettingData, m_SettingLen, false, true );
}

//============================================================================
bool PktPluginSettingReply::setSettingBinary( BinaryBlob& binarySetting )
{
    if( settingBinary.getBlobLen() <= BLOB_PLUGIN_SETTING_MAX_STORAGE_LEN )
    {
        m_SettingLen = settingBinary.getBlobLen();
        memcpy( m_SettingData, settingBinary.getBlobData(), settingBinary.getBlobLen() );
        calcPktLen();
        return true;
    }

    return true;
}