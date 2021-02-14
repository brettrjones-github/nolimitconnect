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
#include <GoTvInterface/IDefs.h>

#pragma pack(1)

#define MAX_HOST_SEARCH_MSG_LEN 512

class PktHostSearchReq : public VxPktHdr
{
public:
    PktHostSearchReq();

    PktBlobEntry&               getBlobEntry( void )                             { return m_BlobEntry; }

    void                        calculateLength();

protected:
    PktBlobEntry                m_BlobEntry;							
};

class PktHostSearchReply : public VxPktHdr
{
public:
    PktHostSearchReply();

    void                        setHostType( EHostType hostType )                           { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }
    void						setSearchSessionId( VxGUID& guid )						    { m_SearchSessionId = guid; }
    VxGUID&					    getSearchSessionId( void )								    { return m_SearchSessionId; }

    void                        setAccessState( EPluginAccess accessState )                 { m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void ) const                                { return (EPluginAccess)m_AccessState; }
    void                        setCommError( ECommErr commError )                          { m_CommError = (uint8_t)commError; }
    ECommErr                    getCommError( void ) const                                  { return (ECommErr)m_CommError; }

    void                        setMatchesInThisPkt( uint8_t matchCnt )                     { m_MatchesInThisPkt = matchCnt; }
    uint8_t                     getMatchesInThisPkt( void ) const                           { return m_MatchesInThisPkt; }

    void                        setTotalMatches( uint16_t matchCnt );
    uint16_t                    getTotalMatches( void ) const;

    void                        setTotalBlobLen( uint16_t totalBlobLen );
    uint16_t                    getTotalBlobLen( void ) const;
    uint16_t                    getRemainingBlobStorageLen( void ) const;

private:
    VxGUID                      m_SearchSessionId;
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_CommError{ 0 };		
    uint8_t					    m_MatchesInThisPkt{ 0 };	
    uint16_t					m_TotalMatches{ 0 };	
    uint16_t                    m_TotalBlobLen{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res2{ 0 };	
    PktBlobEntry                m_PktBlob;	
};

#pragma pack()

