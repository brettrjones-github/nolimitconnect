#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones 
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

#pragma pack(1)

class PktHostInviteAnnounceReq : public VxPktHdr
{
public:
    PktHostInviteAnnounceReq();

    void                        setHostType( EHostType hostType ) { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const { return ( EHostType )m_HostType; }
    void						setSessionId( VxGUID& guid ) { m_SessionId = guid; }
    VxGUID& getSessionId( void ) { return m_SessionId; }

    PktBlobEntry& getBlobEntry( void ) { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SessionId;
    PktBlobEntry                m_BlobEntry;
};

class PktHostInviteAnnounceReply : public VxPktHdr
{
public:
    PktHostInviteAnnounceReply();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }
    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SessionId;
};

class PktHostInviteSearchReq : public VxPktHdr
{
public:
    PktHostInviteSearchReq();

    void                        setHostType( EHostType hostType )       { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const               { return ( EHostType )m_HostType; }
    void						setSearchSessionId( VxGUID& guid )      { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )              { return m_SearchSessionId; }

    void						setSpecificOnlineId( VxGUID& guid )     { m_SpecificOnlineId = guid; }
    VxGUID&                     getSpecificOnlineId( void )             { return m_SpecificOnlineId; }

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_SpecificOnlineId;
};

class PktHostInviteSearchReply : public VxPktHdr
{
public:
    PktHostInviteSearchReply();

    void                        setHostType( EHostType hostType )       { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const               { return ( EHostType )m_HostType; }

    void						setSearchSessionId( VxGUID& guid )      { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )              { return m_SearchSessionId; }

    void						setInviteCountThisPkt( uint16_t inviteCnt ) { m_InviteThisPktCount = inviteCnt; }
    uint16_t&                   setInviteCountThisPkt( void )           { return m_InviteThisPktCount; }

    void						setMoreInvitesExist( bool moreExist)    { m_MoreInvitesExist = moreExist; }
    bool                        getMoreInvitesExist( void )             { return m_MoreInvitesExist; }

    void						setLastOnlineIdThisPkt( VxGUID& guid )  { m_LastOnlineIdThisPkt = guid; }
    VxGUID&                     getLastOnlineIdThisPkt( void )          { return m_LastOnlineIdThisPkt; }

    PktBlobEntry&               getBlobEntry( void )                    { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreInvitesExist{ 0 };
    uint16_t                    m_InviteThisPktCount{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_LastOnlineIdThisPkt;
    PktBlobEntry                m_BlobEntry;
};


class PktHostInviteMoreReq : public VxPktHdr
{
public:
    PktHostInviteMoreReq();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }
    void						setSearchSessionId( VxGUID& guid )  { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )          { return m_SearchSessionId; }

    void						setLastOnlineIdGiven( VxGUID& guid ) { m_LastOnlineIdGiven = guid; }
    VxGUID&                     getLastOnlineIdGiven( void )        { return m_LastOnlineIdGiven; }

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_LastOnlineIdGiven;
};

class PktHostInviteMoreReply : public VxPktHdr
{
public:
    PktHostInviteMoreReply();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setSearchSessionId( VxGUID& guid )  { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )          { return m_SearchSessionId; }

    void						setInviteCountThisPkt( uint16_t inviteCnt ) { m_InviteThisPktCount = inviteCnt; }
    uint16_t&                   setInviteCountThisPkt( void )       { return m_InviteThisPktCount; }

    void						setMoreInvitesExist( bool moreExist ) { m_MoreInvitesExist = moreExist; }
    bool                        getMoreInvitesExist( void )         { return m_MoreInvitesExist; }

    void						setLastOnlineIdThisPkt( VxGUID& guid ) { m_LastOnlineIdThisPkt = guid; }
    VxGUID&                     getLastOnlineIdThisPkt( void )      { return m_LastOnlineIdThisPkt; }

    PktBlobEntry&               getBlobEntry( void )                { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreInvitesExist{ 0 };
    uint16_t                    m_InviteThisPktCount{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_LastOnlineIdThisPkt;
    PktBlobEntry                m_BlobEntry;
};

#pragma pack()

