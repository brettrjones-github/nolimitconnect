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

#include <PktLib/VxCommon.h>




class HostSettingBase
{
public:
    HostSettingBase() = default;
    virtual ~HostSettingBase() = default;


protected:
    std::string                 m_ServiceName;
    std::string                 m_ServiceUrl;
    std::string                 m_ServiceDesc;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
    VxGUID                      m_ThumbAssetId;
    int64_t                     m_LastThumbUpdateTimestamp = 0;
    int64_t                     m_LastConnectTimestamp = 0;
    int64_t                     m_LastChangeTimestamp = 0;
    uint8_t                     m_PermissionLevel = 0;

};

class HostSettingService : public HostSettingBase
{
public:
    HostSettingService() = default;
    virtual ~HostSettingService() = default;


protected:
    uint32_t                    m_MaxStoreAndForwardPerUser = 0;
    uint32_t                    m_MaxInConnections = 0;
    uint32_t                    m_MaxOutConnections = 0;
    uint32_t                    m_MaxBandwidth = 0;
};