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

#include "VxPktHdr.h"
#include "PktBlobEntry.h"
#include "PluginId.h"

#pragma pack(push)
#pragma pack(1)
class PktPluginSettingReq : public VxPktHdr
{
public:
    PktPluginSettingReq();

    void                        setHostType( enum EHostType hostType )                      { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }
    void						setPluginId( PluginId& pluginId )						    { m_PluginId = pluginId; }
    PluginId&					getPluginId( void )								            { return m_PluginId; }
    void						setPluginType( enum EPluginType pluginType )                { m_PluginType = (uint8_t)pluginType; }
    EPluginType					getPluginType( void )								        { return (EPluginType)m_PluginType; }
    void						setSessionId( VxGUID& guid )						        { m_SessionId = guid; }
    VxGUID&					    getSessionId( void )								        { return m_SessionId; }

private:
	//=== vars ===//
    // VxPktHdr 40 bytes 
    VxGUID                      m_SessionId;        // 16 bytes
    PluginId                    m_PluginId;         // 17 bytes 
    uint8_t					    m_HostType{ 0 };    // 1 byte
    // 74 bytes to here
    uint16_t					m_TotalMatches{ 0 };	
    uint8_t					    m_PluginType{ 0 };	
    uint8_t					    m_CommError{ 0 };
    uint16_t					m_Res1{ 0 };	
    // 80 bytes to here
};

class PktPluginSettingReply : public VxPktHdr
{
public:
    PktPluginSettingReply();

	void                        calcPktLen( void );

    void                        setHostType( enum EHostType hostType )                      { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }
    void						setPluginId( PluginId& pluginId )						    { m_PluginId = pluginId; }
    PluginId&					getPluginId( void )								            { return m_PluginId; }
    void						setPluginType( enum EPluginType pluginType )				{ m_PluginType = (uint8_t)pluginType; }
    EPluginType					getPluginType( void )								        { return (EPluginType)m_PluginType; }
    void						setSessionId( VxGUID& guid )						        { m_SessionId = guid; }
    VxGUID&					    getSessionId( void )								        { return m_SessionId; }
    void                        setCommError( enum ECommErr commError )                     { m_CommError = (uint8_t)commError; }
    ECommErr                    getCommError( void ) const                                  { return (ECommErr)m_CommError; }

    void                        setTotalMatches( uint16_t matchCnt );
    uint16_t                    getTotalMatches( void ) const;

    bool                        addPluginId( const PluginId& pluginId );
    PktBlobEntry&               getBlobEntry( void )                                        { return m_BlobEntry; }

private:
    //=== vars ===//
    // VxPktHdr 40 bytes 
    VxGUID                      m_SessionId;        // 16 bytes
    PluginId                    m_PluginId;         // 17 bytes 
    uint8_t					    m_HostType{ 0 };    // 1 byte
                                                    // 74 bytes to here	
    uint16_t					m_TotalMatches{ 0 };	
    uint8_t					    m_PluginType{ 0 };	
    uint8_t					    m_CommError{ 0 };
    uint16_t					m_Res1{ 0 };	
    // 80 bytes to here
    PktBlobEntry                m_BlobEntry;	//size 14352
};

#pragma pack(pop)





