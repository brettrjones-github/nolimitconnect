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

#include "PluginSettingMgr.h"
#include "PluginSettingDb.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
PluginSettingMgr::PluginSettingMgr( P2PEngine& engine )
    : m_Engine( engine )
    , m_PluginSettingDb( GetPluginSettingDbInstance() )
{
}

//============================================================================
bool PluginSettingMgr::initPluginSettingMgr( void )
{
    bool result = m_SettingMgrInitied;
    if( !m_SettingMgrInitied )
    {
        std::string dbFileName = VxGetAppDirectory( eAppDirUserSpecific );
        if( !dbFileName.empty() )
        {
            dbFileName += "pluginSettings.db3";
            if( 0 == m_PluginSettingDb.dbStartup( 1, dbFileName ) )
            {
                m_PluginSettingDb.getAllPluginSettings( m_SettingList );
                m_SettingMgrInitied = true;
                result = true;
            }
        }
    }

    return result;
}

//============================================================================
bool PluginSettingMgr::setPluginSetting( PluginSetting& pluginSetting )
{
    m_SettingMutex.lock();
    bool result = initPluginSettingMgr();
    if( result )
    {
        result = false;
        if( pluginSetting.getPluginType() != ePluginTypeInvalid )
        {
            for( PluginSetting& setting : m_SettingList )
            {
                if( setting.getPluginType() == pluginSetting.getPluginType() )
                {
                    setting = pluginSetting;
                    result = m_PluginSettingDb.updatePluginSetting( setting.getPluginType(), setting );
                }
            }

            if( !result )
            {
                m_SettingList.push_back( pluginSetting );
                result = m_PluginSettingDb.updatePluginSetting( pluginSetting.getPluginType(), pluginSetting );
            }

            if( result )
            {
                m_Engine.getPluginMgr().onPluginSettingChange( pluginSetting );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "setPluginSetting invalid plugin type " );
        }
    }

    m_SettingMutex.unlock();
    return result;
}

//============================================================================
bool PluginSettingMgr::getPluginSetting( EPluginType pluginType, PluginSetting& pluginSetting )
{
    m_SettingMutex.lock();
    bool result = initPluginSettingMgr();
    if( result )
    {
        result = false;
        if( pluginType != ePluginTypeInvalid )
        {
            for( PluginSetting& setting : m_SettingList )
            {
                if( setting.getPluginType() == pluginType )
                {
                    pluginSetting = setting;
                    result = true;
                    break;
                }
            }

            if( !result )
            {
                PluginSetting setting;
                if( setting.setDefaultValues( m_Engine, pluginType ) )
                {
                    m_SettingList.push_back( setting );
                    pluginSetting = setting;
                    result = true;
                }
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "getPluginSetting invalid plugin type " );
            result = false;
        }
    }

    m_SettingMutex.unlock();
    return result;
}
