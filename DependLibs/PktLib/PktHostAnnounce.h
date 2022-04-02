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
// http://www.nolimitconnect.com
//============================================================================

#include "PktAnnounce.h"

#include <CoreLib/BinaryBlob.h>

#pragma pack(push) 
#pragma pack(1)

//      1 bytes m_HostType
//      1 bytes m_SettingRes1
// +    4 bytes m_SettingLength
// + 4120 bytes m_SettingData    (4096 + 8 + 16)
// = 4128
// +  480 bytes PktAnnounce
// = 4608 bytes total

class PluginSetting;

class PktHostAnnounce :  public PktAnnounce
{
public:
    PktHostAnnounce();

	bool						isValidPktHostAnn( void );

    void						setHostType( EHostType hostType ) { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) { return (EHostType)m_HostType; }

    void                        calcPktLen( void );
    void                        setPktAnn( PktAnnounce& pktAnn );
    bool                        fillPktHostAnn( PktHostAnnounce& pktAnn ); // copy to another PktHostAnnounce

    bool                        setPluginSettingBinary( BinaryBlob& settingBinary );
    bool                        getPluginSettingBinary( BinaryBlob& settingBinary );

    PktHostAnnounce *			makeHostAnnCopy( void );
    PktHostAnnounce *			makeHostAnnReverseCopy( void );
	void						DebugHostDump( void );

    uint8_t					    m_HostType = 0;
    uint8_t					    m_SettingRes1 = 0;
    uint16_t					m_SettingRes2 = 0;
    uint32_t					m_SettingLength = 0;
    uint8_t						m_SettingData[ BLOB_PLUGIN_SETTING_MAX_STORAGE_LEN + 16 ];
};

#pragma pack() 
#pragma pack(pop)

