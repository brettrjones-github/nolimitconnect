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
#include <GoTvInterface/IDefs.h>

#pragma pack(1)

#define MAX_HOST_OFFER_MSG_LEN 2048

class PktHostOfferReq : public VxPktHdr
{
public:
    PktHostOfferReq();

    void                        setHostType( EHostType hostType )   { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                 { return (EHostType)m_HostType; }

    void						setOfferMsg( const char * msg );
    const char *				getOfferMsg( void );

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_StrLen{ 0 };					
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    uint8_t						m_au8Data[ MAX_HOST_OFFER_MSG_LEN + 16 ];								
};

class PktHostOfferReply : public VxPktHdr
{
public:
    PktHostOfferReply();

    void                        setHostType( EHostType hostType )   { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                 { return (EHostType)m_HostType; }

    void						setOfferMsg( const char * msg );
    const char *				getOfferMsg( void );

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_StrLen{ 0 };					
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    uint8_t						m_au8Data[ MAX_HOST_OFFER_MSG_LEN + 16 ];	
};

class PktHostJoinReq : public VxPktHdr
{
public:
    PktHostJoinReq();

    void                        setHostType( EHostType hostType )   { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                 { return (EHostType)m_HostType; }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_Res2{ 0 };					
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };		
};

class PktHostJoinReply : public VxPktHdr
{
public:
    PktHostJoinReply();

    void                        setHostType( EHostType hostType )   { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                 { return (EHostType)m_HostType; }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_Res2{ 0 };					
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };	
};

#pragma pack()

