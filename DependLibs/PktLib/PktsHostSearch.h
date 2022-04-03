#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include "PktTypes.h"
#include "PktBlobEntry.h"

#include <GuiInterface/IDefs.h>

#pragma pack(push)
#pragma pack(1)

#define MAX_HOST_SEARCH_MSG_LEN 512

class PktHostSearchReq : public VxPktHdr
{
public:
    PktHostSearchReq();

    void                        setHostType( enum EHostType hostType )                      { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }
    void						setSearchSessionId( VxGUID& guid )						    { m_SearchSessionId = guid; }
    VxGUID&					    getSearchSessionId( void )								    { return m_SearchSessionId; }
    void                        setPluginType( enum EPluginType hostType )                  { m_PluginType = (uint8_t)hostType; }
    EPluginType                 getPluginType( void ) const                                 { return (EPluginType)m_PluginType; }

    PktBlobEntry&               getBlobEntry( void )                                        { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    VxGUID                      m_SearchSessionId;
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };	

    PktBlobEntry                m_BlobEntry;							
};

class PluginId;
class PktHostSearchReply : public VxPktHdr
{
public:
    PktHostSearchReply();

    void                        calcPktLen( void );

    void                        setHostType( enum EHostType hostType )                      { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }
    void						setSearchSessionId( VxGUID& guid )						    { m_SearchSessionId = guid; }
    VxGUID&					    getSearchSessionId( void )								    { return m_SearchSessionId; }

    void                        setAccessState( enum EPluginAccess accessState )            { m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void ) const                                { return (EPluginAccess)m_AccessState; }
    void                        setCommError( enum ECommErr commError )                     { m_CommError = (uint8_t)commError; }
    ECommErr                    getCommError( void ) const                                  { return (ECommErr)m_CommError; }

    void                        setIsGuidPluginTypePairs( bool isPair )                     { m_IsGuidPluginTypePairs = (uint8_t)isPair; }
    uint8_t                     getIsGuidPluginTypePairs( void ) const                      { return m_IsGuidPluginTypePairs; }

    PktBlobEntry&               getBlobEntry( void )                                        { return m_BlobEntry; }

    void                        setTotalMatches( uint16_t matchCnt );
    uint16_t                    getTotalMatches( void ) const;

    // add guid to blob and increment total match count
    bool                        addMatchOnlineId( VxGUID& onlineId );
    bool                        addPluginId( const PluginId& pluginId );
    uint16_t                    getRemainingBlobStorageLen( void ) const;

private:
    VxGUID                      m_SearchSessionId;
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_CommError{ 0 };		
    uint8_t					    m_IsGuidPluginTypePairs{ 0 };	

    uint16_t					m_TotalMatches{ 0 };	
    uint16_t                    m_Res1{ 0 };	
    uint64_t					m_Res2{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    PktBlobEntry                m_BlobEntry;	
};

#pragma pack(pop)


