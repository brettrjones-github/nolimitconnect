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
#pragma once

#include "PluginSetting.h"

#include <CoreLib/VxMutex.h>

class PluginSettingDb;
class P2PEngine;

class PluginSettingMgr 
{
public:
    PluginSettingMgr( P2PEngine& engine );
    virtual ~PluginSettingMgr() = default;

    bool                        setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs = 0 );
    bool                        getPluginSetting( EPluginType pluginType, PluginSetting& pluginSetting );
    bool                        getHostTitleAndDescription( EPluginType pluginType, std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime );

protected:
    bool                        initPluginSettingMgr( void );
    PluginSettingDb&            getPluginSettingDb() { return m_PluginSettingDb; }

    //=== vars ===//
    P2PEngine&					m_Engine;

    bool                        m_SettingMgrInitied = false;
    VxMutex                     m_SettingMutex;
    PluginSettingDb&            m_PluginSettingDb;
    std::vector<PluginSetting>  m_SettingList;
};

