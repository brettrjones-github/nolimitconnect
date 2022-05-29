#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "VxPktHdr.h"
#include "VxSearchFlags.h"
#include "VxCommon.h"
#include <GuiInterface/IScan.h>

#include <CoreLib/BinaryBlob.h>


#pragma pack(push) 
#pragma pack(1)
class PktPluginSettingReq : public VxPktHdr
{
public:
    PktPluginSettingReq();

private:
	//=== vars ===//
    uint32_t					m_SettingRes1 = 0;
    uint32_t					m_SettingRes2 = 0;
};

class PktPluginSettingReply : public VxPktHdr
{
public:
    PktPluginSettingReply();

	void                        calcPktLen( void );

    bool                        getSettingBinary( BinaryBlob& binarySetting );
    bool                        setSettingBinary( BinaryBlob& binarySetting );

private:
	//=== vars ===//
    uint32_t					m_SettingRes1 = 0;
    uint32_t					m_SettingRes2 = 0;
    uint8_t						m_SettingData[ BLOB_PLUGIN_SETTING_MAX_STORAGE_LEN + 16 ];
};

#pragma pack(pop)





