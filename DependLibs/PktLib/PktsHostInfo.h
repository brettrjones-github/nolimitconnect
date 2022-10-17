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
#include "PktBlobEntry.h"
#include "PluginId.h"

#pragma pack(push)
#pragma pack(1)
class PktHostInfoReq : public VxPktHdr
{
public:
    PktHostInfoReq();

    void						setSessionId( VxGUID& guid )						        { m_SessionId = guid; }
    VxGUID&					    getSessionId( void )								        { return m_SessionId; }

    void                        setHostType( enum EHostType hostType )                      { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const                                   { return ( EHostType )m_HostType; }

    void                        setCommError(enum  ECommErr commError )                     { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const                                  { return ( ECommErr )m_CommError; }

private:
	//=== vars ===//
    // VxPktHdr 40 bytes 
    VxGUID                      m_SessionId;        // 16 bytes
    uint8_t					    m_HostType{ 0 };    // 1 byte
    uint8_t					    m_CommError{ 0 };   // 1 byte
    uint16_t                    m_Res1{ 0 };        // fill to 16 byte boundry
    uint32_t                    m_Res2{ 0 };        // fill to 16 byte boundry
};

class PktHostInfoReply : public VxPktHdr
{
public:
    PktHostInfoReply();

	void                        calcPktLen( void );

    void						setSessionId( VxGUID& guid )                                { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                                        { return m_SessionId; }

    void                        setHostType( enum EHostType hostType )                      { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void ) const                                   { return (EHostType)m_HostType; }

    void                        setCommError( enum ECommErr commError )                     { m_CommError = (uint8_t)commError; }
    ECommErr                    getCommError( void ) const                                  { return (ECommErr)m_CommError; }

    bool                        setHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime );
    bool                        getHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime );

    PktBlobEntry&               getBlobEntry( void )                                        { return m_BlobEntry; }

private:
    //=== vars ===//
    // VxPktHdr 40 bytes 
    VxGUID                      m_SessionId;            // 16 bytes
    uint8_t					    m_HostType{ 0 };        // 1 byte
    uint8_t					    m_CommError{ 0 };       // 1 byte
    uint16_t					m_Res1{ 0 };	        // 2 bytes
    uint32_t					m_Res2{ 0 };	        // 4 bytes
    // 64 bytes to here
    PktBlobEntry                m_BlobEntry;	        // size 14352
};

#pragma pack(pop)






