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

#include <GuiInterface/IDefs.h>

#include <CoreLib/DbBase.h>

#include <vector>

#define PLUGIN_SETTING_DB_VERSION 1

class PluginSetting;

class PluginSettingDb : public DbBase
{
public:
    PluginSettingDb();
    virtual ~PluginSettingDb() = default;

    bool						isValid( void )							{ return m_bIsValid; }
    void						setIsValid( bool bValid )				{ m_bIsValid = bValid; }

    //=== overrides ===//
    //! delete tables from database 
    virtual RCODE				onDeleteTables( int oldVersion );
    //! create tables in database 
    virtual RCODE				onCreateTables( int iDbVersion );

    bool                        updatePluginSetting( EPluginType pluginType, PluginSetting& pluginSetting );
    bool                        getPluginSetting( EPluginType pluginType, PluginSetting& pluginSetting );
    bool                        getAllPluginSettings( std::vector<PluginSetting>& pluginSettingList );

protected:
    bool                        pluginExistsInTable( std::string pluginName, std::string& strTableName );

    //=== vars ===//
    bool						m_bIsValid;
};


PluginSettingDb& GetPluginSettingDbInstance();