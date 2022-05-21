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
#include "GroupieId.h"

#pragma pack(push)
#pragma pack(1)

#define MAX_HOST_OFFER_MSG_LEN 2048

class PktHostOfferReq : public VxPktHdr
{
public:
    PktHostOfferReq();

    void                        setHostType( enum EHostType hostType )          { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                             { return (EHostType)m_HostType; }
    void                        setPluginType( enum EPluginType pluginType )    { m_PluginType = (uint8_t)pluginType; }
    EPluginType                 getPluginType( void )                           { return (EPluginType)m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ){ m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void )                          { return (EPluginAccess)m_AccessState; }

    void						setOfferMsg( const char * msg );
    const char *				getOfferMsg( void );

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint8_t					    m_AccessState{ 0 };			
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_StrLen{ 0 };					
    uint16_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    uint8_t						m_au8Data[ MAX_HOST_OFFER_MSG_LEN + 16 ];								
};

class PktHostOfferReply : public VxPktHdr
{
public:
    PktHostOfferReply();

    void                        setHostType( enum EHostType hostType )          { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                             { return (EHostType)m_HostType; }
    void                        setPluginType( enum EPluginType pluginType )    { m_PluginType = (uint8_t)pluginType; }
    EPluginType                 getPluginType( void )                           { return (EPluginType)m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ){ m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void )                          { return (EPluginAccess)m_AccessState; }

    void						setOfferMsg( const char * msg );
    const char *				getOfferMsg( void );

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };	
    uint8_t					    m_AccessState{ 0 };		
    uint8_t					    m_Res1{ 0 };
    uint16_t					m_StrLen{ 0 };					
    uint16_t					m_Res2{ 0 };		
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    uint8_t						m_au8Data[ MAX_HOST_OFFER_MSG_LEN + 16 ];	
};

class PktHostJoinReq : public VxPktHdr
{
public:
    PktHostJoinReq();

    void                        setHostType( enum EHostType hostType )          { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                             { return (EHostType)m_HostType; }
    void                        setPluginType( enum EPluginType pluginType )    { m_PluginType = (uint8_t)pluginType; }
    EPluginType                 getPluginType( void )                           { return (EPluginType)m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState )     { m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void )                          { return (EPluginAccess)m_AccessState; }
    void                        setSessionId( VxGUID& sessionId )               { m_SessionId = sessionId; }
    VxGUID                      getSessionId( void )                            { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )              { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                         { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )              { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                         { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )            { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                            { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };	
    uint8_t					    m_AccessState{ 0 };	
    uint8_t					    m_Res1{ 0 };
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

class PktHostJoinReply : public VxPktHdr
{
public:
    PktHostJoinReply();

    void                        setHostType( enum EHostType hostType )          { m_HostType = (uint8_t)hostType; }
    EHostType                   getHostType( void )                             { return (EHostType)m_HostType; }
    void                        setPluginType( enum EPluginType pluginType )    { m_PluginType = (uint8_t)pluginType; }
    EPluginType                 getPluginType( void )                           { return (EPluginType)m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ){ m_AccessState = (uint8_t)accessState; }
    EPluginAccess               getAccessState( void )                          { return (EPluginAccess)m_AccessState; }
    void                        setSessionId( VxGUID sessionId )                { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                            { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )              { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                         { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )              { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                         { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )            { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                            { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }
    void                        setCommError( ECommErr commError )              { m_CommError = (uint8_t)commError; }
    ECommErr                    getCommError( void ) const                      { return (ECommErr)m_CommError; }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };	
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_CommError{ 0 };
    uint32_t					m_Res3{ 0 };	
    uint64_t					m_TimeRequestedMs{ 0 };		
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

class PktHostLeaveReq : public VxPktHdr
{
public:
    PktHostLeaveReq();

    void                        setHostType( enum EHostType hostType )  { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void )                     { return ( EHostType )m_HostType; }
    void                        setPluginType( enum EPluginType pluginType ) { m_PluginType = ( uint8_t )pluginType; }
    EPluginType                 getPluginType( void )                   { return ( EPluginType )m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ) { m_AccessState = ( uint8_t )accessState; }
    EPluginAccess               getAccessState( void )                  { return ( EPluginAccess )m_AccessState; }
    void                        setSessionId( VxGUID& sessionId )       { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )      { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                 { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )      { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                 { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )    { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                    { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint32_t					m_Res3{ 0 };
    uint64_t					m_TimeRequestedMs{ 0 };
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

class PktHostLeaveReply : public VxPktHdr
{
public:
    PktHostLeaveReply();

    void                        setHostType( enum EHostType hostType )          { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void )                             { return ( EHostType )m_HostType; }
    void                        setPluginType( enum EPluginType pluginType )    { m_PluginType = ( uint8_t )pluginType; }
    EPluginType                 getPluginType( void )                           { return ( EPluginType )m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ) { m_AccessState = ( uint8_t )accessState; }
    EPluginAccess               getAccessState( void )                          { return ( EPluginAccess )m_AccessState; }
    void                        setSessionId( VxGUID sessionId )                { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                            { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )              { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                         { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )              { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                         { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )            { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                            { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }
    void                        setCommError( ECommErr commError )              { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const                      { return ( ECommErr )m_CommError; }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_CommError{ 0 };
    uint32_t					m_Res3{ 0 };
    uint64_t					m_TimeRequestedMs{ 0 };
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

class PktHostUnJoinReq : public VxPktHdr
{
public:
    PktHostUnJoinReq();

    void                        setHostType( enum EHostType hostType )      { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void )                          { return ( EHostType )m_HostType; }
    void                        setPluginType( enum EPluginType pluginType ) { m_PluginType = ( uint8_t )pluginType; }
    EPluginType                 getPluginType( void ) { return ( EPluginType )m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ) { m_AccessState = ( uint8_t )accessState; }
    EPluginAccess               getAccessState( void )                      { return ( EPluginAccess )m_AccessState; }
    void                        setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                        { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )          { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                     { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )          { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                     { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )        { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                        { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }


private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_Res1{ 0 };
    uint32_t					m_Res3{ 0 };
    uint64_t					m_TimeRequestedMs{ 0 };
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

class PktHostUnJoinReply : public VxPktHdr
{
public:
    PktHostUnJoinReply();

    void                        setHostType( enum EHostType hostType ) { m_HostType = ( uint8_t )hostType; }
    EHostType                   getHostType( void )                     { return ( EHostType )m_HostType; }
    void                        setPluginType( enum EPluginType pluginType ) { m_PluginType = ( uint8_t )pluginType; }
    EPluginType                 getPluginType( void )                   { return ( EPluginType )m_PluginType; }

    void                        setAccessState( enum EPluginAccess accessState ) { m_AccessState = ( uint8_t )accessState; }
    EPluginAccess               getAccessState( void )                  { return ( EPluginAccess )m_AccessState; }
    void                        setSessionId( VxGUID sessionId )        { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }
    void                        setUserOnlineId( VxGUID onlineId )      { m_UserOnlineId = onlineId; }
    VxGUID&                     getUserOnlineId( void )                 { return m_UserOnlineId; }
    void                        setHostOnlineId( VxGUID onlineId )      { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                 { return m_HostOnlineId; }
    void                        setGroupieId( GroupieId& groupieId )    { m_UserOnlineId = groupieId.getGroupieOnlineId(); m_HostOnlineId = groupieId.getHostedOnlineId(); m_HostType = (uint8_t)groupieId.getHostType(); }
    GroupieId                   getGroupieId( void )                    { return GroupieId( m_UserOnlineId, m_HostOnlineId, (EHostType)m_HostType ); }
    void                        setCommError( ECommErr commError )      { m_CommError = ( uint8_t )commError; }
    ECommErr                    getCommError( void ) const              { return ( ECommErr )m_CommError; }

private:
    uint8_t					    m_HostType{ 0 };
    uint8_t					    m_PluginType{ 0 };
    uint8_t					    m_AccessState{ 0 };
    uint8_t					    m_CommError{ 0 };
    uint32_t					m_Res3{ 0 };
    uint64_t					m_TimeRequestedMs{ 0 };
    uint64_t					m_Res4{ 0 };
    VxGUID                      m_SessionId;
    VxGUID                      m_UserOnlineId;
    VxGUID                      m_HostOnlineId;
};

#pragma pack(pop)


