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

#pragma pack(push)
#pragma pack(1)
class PktGroupieInfoReq : public VxPktHdr
{
public:
    PktGroupieInfoReq();

    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

    void						setGroupieOnlineId( VxGUID& guid )  { m_GroupieOnlineId = guid; }
    VxGUID&                     getGroupieOnlineId( void )          { return m_GroupieOnlineId; }

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )     { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )             { return m_HostOnlineId; }

    void                        setCommError( ECommErr commError )  { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const          { return ( ECommErr )m_CommError; }

private:
    //=== vars ===//
    // VxPktHdr 40 bytes 
    uint8_t					    m_HostType{ 0 };    // 1 byte
    uint8_t					    m_CommError{ 0 };   // 1 byte
    uint16_t                    m_Res1{ 0 };        // fill to 16 byte boundry
    uint32_t                    m_Res2{ 0 };        // fill to 16 byte boundry
    VxGUID                      m_SessionId;        // 16 bytes
    VxGUID                      m_GroupieOnlineId;  // 16 bytes
    VxGUID                      m_HostOnlineId;  // 16 bytes
};

class PktGroupieInfoReply : public VxPktHdr
{
public:
    PktGroupieInfoReply();

    void                        calcPktLen( void );

    void						setSessionId( VxGUID& guid )    { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )            { return m_SessionId; }

    void                        setHostType( EHostType hostType ) { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const       { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid ) { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )         { return m_HostOnlineId; }

    void                        setCommError( ECommErr commError ) { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const { return ( ECommErr )m_CommError; }

    bool                        setGroupieUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );
    bool                        getGroupieUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    PktBlobEntry&               getBlobEntry( void ) { return m_BlobEntry; }

private:
    //=== vars ===//
    // VxPktHdr 40 bytes 
    VxGUID                      m_SessionId;            // 16 bytes
    uint8_t					    m_HostType{ 0 };        // 1 byte
    uint8_t					    m_CommError{ 0 };       // 1 byte
    uint16_t					m_Res1{ 0 };	        // 2 bytes
    VxGUID                      m_HostOnlineId;  // 16 bytes
    // 60 bytes to here
    PktBlobEntry                m_BlobEntry;	//size 14352
};


class PktGroupieAnnounceReq : public VxPktHdr
{
public:
    PktGroupieAnnounceReq();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )     { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )             { return m_HostOnlineId; }

    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

    bool                        setGroupieInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );
    bool                        getGroupieInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    PktBlobEntry&               getBlobEntry( void ) { return m_BlobEntry; }

    void                        calcPktLen( void );

    PktGroupieAnnounceReq*      makeGroupieAnnCopy( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_HostOnlineId;  // 16 bytes
    PktBlobEntry                m_BlobEntry;
};

class PktGroupieAnnounceReply : public VxPktHdr
{
public:
    PktGroupieAnnounceReply();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )     { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )             { return m_HostOnlineId; }

    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

    void                        setCommError( ECommErr commError )  { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const          { return ( ECommErr )m_CommError; }

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_CommError{ 0 };
    uint16_t                    m_Res1{ 0 };
    uint32_t                    m_Res2{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_HostOnlineId;  // 16 bytes
};

class PktGroupieSearchReq : public VxPktHdr
{
public:
    PktGroupieSearchReq();

    void                        setHostType( EHostType hostType )       { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const               { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )         { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )                 { return m_HostOnlineId; }

    void						setSearchSessionId( VxGUID& guid )      { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )              { return m_SearchSessionId; }

    void						setSpecificOnlineId( VxGUID& guid )     { m_SpecificOnlineId = guid; }
    VxGUID&                     getSpecificOnlineId( void )             { return m_SpecificOnlineId; }

    bool						setSearchText( std::string& searchText );
    bool						getSearchText( std::string& searchText );

    PktBlobEntry&               getBlobEntry( void ) { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_HostOnlineId;  // 16 bytes
    VxGUID                      m_SpecificOnlineId;
    PktBlobEntry                m_BlobEntry;	
};

class PktGroupieSearchReply : public VxPktHdr
{
public:
    PktGroupieSearchReply();

    void                        setHostType( EHostType hostType )       { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const               { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )         { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )                 { return m_HostOnlineId; }

    void						setSearchSessionId( VxGUID& guid )      { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )              { return m_SearchSessionId; }

    void                        setCommError( ECommErr commError )      { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const              { return ( ECommErr )m_CommError; }

    bool                        addGroupieInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    void						setGroupieCountThisPkt( uint16_t inviteCnt ) { m_GroupieThisPktCount = inviteCnt; }
    uint16_t&                   getGroupieCountThisPkt( void )           { return m_GroupieThisPktCount; }
    void                        incrementGroupieCount( void )            { m_GroupieThisPktCount++; }

    void						setMoreGroupiesExist( bool moreExist)    { m_MoreGroupiesExist = moreExist; }
    bool                        getMoreGroupiesExist( void )             { return m_MoreGroupiesExist; }

    void						setNextSearchOnlineId( VxGUID& guid )   { m_NextSearchOnlineId = guid; }
    VxGUID&                     getNextSearchOnlineId( void )           { return m_NextSearchOnlineId; }

    PktBlobEntry&               getBlobEntry( void )                    { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreGroupiesExist{ 0 };
    uint8_t                     m_CommError{ 0 };
    uint8_t                     m_Res1{ 0 };
    uint16_t                    m_GroupieThisPktCount{ 0 };
    uint16_t                    m_Res2{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_HostOnlineId;
    VxGUID                      m_NextSearchOnlineId;
    PktBlobEntry                m_BlobEntry;
};


class PktGroupieMoreReq : public VxPktHdr
{
public:
    PktGroupieMoreReq();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )     { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )             { return m_HostOnlineId; }

    void						setSearchSessionId( VxGUID& guid )  { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )          { return m_SearchSessionId; }

    void						setNextSearchOnlineId( VxGUID& guid ) { m_NextSearchOnlineId = guid; }
    VxGUID&                     getNextSearchOnlineId( void )        { return m_NextSearchOnlineId; }

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_HostOnlineId;
    VxGUID                      m_NextSearchOnlineId;
};

class PktGroupieMoreReply : public VxPktHdr
{
public:
    PktGroupieMoreReply();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setSearchSessionId( VxGUID& guid )  { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )          { return m_SearchSessionId; }

    void                        setCommError( ECommErr commError )  { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const          { return ( ECommErr )m_CommError; }

    void						setGroupieCountThisPkt( uint16_t inviteCnt ) { m_GroupieThisPktCount = inviteCnt; }
    uint16_t&                   getGroupieCountThisPkt( void )       { return m_GroupieThisPktCount; }
    void                        incrementGroupieCount( void )        { m_GroupieThisPktCount++; }

    void						setMoreGroupiesExist( bool moreExist ) { m_MoreGroupiesExist = moreExist; }
    bool                        getMoreGroupiesExist( void )         { return m_MoreGroupiesExist; }

    void						setNextSearchOnlineId( VxGUID& guid ) { m_NextSearchOnlineId = guid; }
    VxGUID&                     getNextSearchOnlineId( void )       { return m_NextSearchOnlineId; }

    PktBlobEntry&               getBlobEntry( void )                { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreGroupiesExist{ 0 };
    uint8_t                     m_CommError{ 0 };
    uint8_t                     m_Res1{ 0 };
    uint16_t                    m_GroupieThisPktCount{ 0 };
    uint16_t                    m_Res2{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_NextSearchOnlineId;
    PktBlobEntry                m_BlobEntry;
};

#pragma pack()
#pragma pack(pop)

