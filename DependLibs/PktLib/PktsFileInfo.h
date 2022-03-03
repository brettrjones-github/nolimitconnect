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
class PktFileInfoInfoReq : public VxPktHdr
{
public:
    PktFileInfoInfoReq();

    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

    void						setFileInfoOnlineId( VxGUID& guid )  { m_FileInfoOnlineId = guid; }
    VxGUID&                     getFileInfoOnlineId( void )          { return m_FileInfoOnlineId; }

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
    VxGUID                      m_FileInfoOnlineId;  // 16 bytes
    VxGUID                      m_HostOnlineId;  // 16 bytes
};

class PktFileInfoInfoReply : public VxPktHdr
{
public:
    PktFileInfoInfoReply();

    void                        calcPktLen( void );

    void						setSessionId( VxGUID& guid )    { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )            { return m_SessionId; }

    void                        setHostType( EHostType hostType ) { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const       { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid ) { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )         { return m_HostOnlineId; }

    void                        setCommError( ECommErr commError ) { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const { return ( ECommErr )m_CommError; }

    bool                        setFileInfoUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );
    bool                        getFileInfoUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

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


class PktFileInfoAnnounceReq : public VxPktHdr
{
public:
    PktFileInfoAnnounceReq();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )     { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )             { return m_HostOnlineId; }

    void						setSessionId( VxGUID& guid )        { m_SessionId = guid; }
    VxGUID&                     getSessionId( void )                { return m_SessionId; }

    bool                        setFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );
    bool                        getFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    PktBlobEntry&               getBlobEntry( void ) { return m_BlobEntry; }

    void                        calcPktLen( void );

    PktFileInfoAnnounceReq*      makeFileInfoAnnCopy( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint16_t                    m_Res2{ 0 };
    uint32_t                    m_Res3{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_HostOnlineId;  // 16 bytes
    PktBlobEntry                m_BlobEntry;
};

class PktFileInfoAnnounceReply : public VxPktHdr
{
public:
    PktFileInfoAnnounceReply();

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

class PktFileInfoSearchReq : public VxPktHdr
{
public:
    PktFileInfoSearchReq();

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

class PktFileInfoSearchReply : public VxPktHdr
{
public:
    PktFileInfoSearchReply();

    void                        setHostType( EHostType hostType )       { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const               { return ( EHostType )m_HostType; }

    void						setHostOnlineId( VxGUID& guid )         { m_HostOnlineId = guid; }
    VxGUID&                     getHostOnlineId( void )                 { return m_HostOnlineId; }

    void						setSearchSessionId( VxGUID& guid )      { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )              { return m_SearchSessionId; }

    void                        setCommError( ECommErr commError )      { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const              { return ( ECommErr )m_CommError; }

    bool                        addFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    void						setFileInfoCountThisPkt( uint16_t inviteCnt ) { m_FileInfoThisPktCount = inviteCnt; }
    uint16_t&                   getFileInfoCountThisPkt( void )           { return m_FileInfoThisPktCount; }
    void                        incrementFileInfoCount( void )            { m_FileInfoThisPktCount++; }

    void						setMoreFileInfosExist( bool moreExist)    { m_MoreFileInfosExist = moreExist; }
    bool                        getMoreFileInfosExist( void )             { return m_MoreFileInfosExist; }

    void						setNextSearchOnlineId( VxGUID& guid )   { m_NextSearchOnlineId = guid; }
    VxGUID&                     getNextSearchOnlineId( void )           { return m_NextSearchOnlineId; }

    PktBlobEntry&               getBlobEntry( void )                    { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreFileInfosExist{ 0 };
    uint8_t                     m_CommError{ 0 };
    uint8_t                     m_Res1{ 0 };
    uint16_t                    m_FileInfoThisPktCount{ 0 };
    uint16_t                    m_Res2{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_HostOnlineId;
    VxGUID                      m_NextSearchOnlineId;
    PktBlobEntry                m_BlobEntry;
};


class PktFileInfoMoreReq : public VxPktHdr
{
public:
    PktFileInfoMoreReq();

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

class PktFileInfoMoreReply : public VxPktHdr
{
public:
    PktFileInfoMoreReply();

    void                        setHostType( EHostType hostType )   { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void ) const           { return ( EHostType )m_HostType; }

    void						setSearchSessionId( VxGUID& guid )  { m_SearchSessionId = guid; }
    VxGUID&                     getSearchSessionId( void )          { return m_SearchSessionId; }

    void                        setCommError( ECommErr commError )  { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const          { return ( ECommErr )m_CommError; }

    void						setFileInfoCountThisPkt( uint16_t inviteCnt ) { m_FileInfoThisPktCount = inviteCnt; }
    uint16_t&                   getFileInfoCountThisPkt( void )       { return m_FileInfoThisPktCount; }
    void                        incrementFileInfoCount( void )        { m_FileInfoThisPktCount++; }

    void						setMoreFileInfosExist( bool moreExist ) { m_MoreFileInfosExist = moreExist; }
    bool                        getMoreFileInfosExist( void )         { return m_MoreFileInfosExist; }

    void						setNextSearchOnlineId( VxGUID& guid ) { m_NextSearchOnlineId = guid; }
    VxGUID&                     getNextSearchOnlineId( void )       { return m_NextSearchOnlineId; }

    PktBlobEntry&               getBlobEntry( void )                { return m_BlobEntry; }

    void                        calcPktLen( void );

protected:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_MoreFileInfosExist{ 0 };
    uint8_t                     m_CommError{ 0 };
    uint8_t                     m_Res1{ 0 };
    uint16_t                    m_FileInfoThisPktCount{ 0 };
    uint16_t                    m_Res2{ 0 };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_NextSearchOnlineId;
    PktBlobEntry                m_BlobEntry;
};

#pragma pack()

