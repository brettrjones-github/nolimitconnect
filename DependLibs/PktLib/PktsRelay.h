#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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

#include "PktTypes.h"
#include "VxCommon.h"

#include <GuiInterface/IDefs.h>

#pragma pack(push)
#pragma pack(1)

class PktRelayServiceReq : public VxPktHdr
{
public:
	PktRelayServiceReq();

	void						setLclSessionId( VxGUID& lclId )					{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )								{ return m_LclSessionId; }

	uint8_t						m_u8CancelService{ 0 }; // 0 = request access 1 = cancel access
private:
	//=== vars ===//
	uint8_t						m_u8Res1{ 0 };
	uint16_t					m_u16Res{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
};

class PktRelayServiceReply : public VxPktHdr
{
public:
	PktRelayServiceReply();

	void						setLclSessionId( VxGUID& lclId )					{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )								{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )					{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )								{ return m_RmtSessionId; }

    void						setAccessState( enum EPluginAccess ePluginAccess )  { m_u32Error = (uint32_t)ePluginAccess; }
	EPluginAccess				getAccessState( void )								{ return (EPluginAccess)m_u32Error; }
    void						setRelayStatus( enum ERelayStatus eStatus )				{ m_u8RelayStatus = (uint8_t)eStatus; }
	ERelayStatus				getRelayStatus( void )								{ return (ERelayStatus)m_u8RelayStatus; }

private:
	//=== vars ===//
	uint8_t						m_u8RelayStatus{ 0 };
	uint8_t						m_u8AccessState{ 0 };
	uint16_t					m_u16Res{ 0 };
	uint32_t					m_u32Error{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

// request connection through our relay because this node is behind firewall
class PktRelayConnectToUserReq : public VxPktHdr
{
public:
	PktRelayConnectToUserReq();

	//=== vars ===//
	VxConnectId					m_ConnectId;
private:
	uint16_t					m_u16Res{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	uint32_t					m_u32Res3{ 0 };
	uint32_t					m_u32Res4{ 0 };
};

class PktRelayConnectToUserReply : public VxPktHdr
{
public:
	PktRelayConnectToUserReply();

	void						setRelayError( ERelayErr relayErr )			{ m_RelayError = (uint8_t)relayErr; }
	ERelayErr					getRelayError( void )						{ return (ERelayErr)m_RelayError; }

	void						setConnectFailed( uint8_t failCode )		{ m_ConnectFailed = failCode; }
	uint8_t						getConnectFailed( void )					{ return m_ConnectFailed; }

	//=== vars ===//
	VxConnectId					m_ConnectId;
private:
	uint8_t						m_RelayError{ 0 };
	uint8_t						m_ConnectFailed{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	uint32_t					m_u32Res3{ 0 };
	uint32_t					m_u32Res4{ 0 };
};

class PktRelayUserDisconnect : public VxPktHdr
{
public:
	PktRelayUserDisconnect();

	//=== vars ===//
	VxGUID						m_UserId;
private:
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

class PktRelaySessionReq : public VxPktHdr
{
public:
	PktRelaySessionReq();

    void						setPluginAccess( enum EPluginAccess eAccess );
    EPluginAccess               getPluginAccess();
	void						setTestOnly( uint8_t test )							{ m_u8TestOnly = test; }
	uint8_t						getTestOnly( void )									{ return m_u8TestOnly; }

	void						setLclSessionId( VxGUID& rmtId )					{ m_LclSessionId = rmtId; }
	VxGUID&						getLclSessionId( void )								{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& lclId )					{ m_RmtSessionId = lclId; }
	VxGUID&						getRmtSessionId( void )								{ return m_RmtSessionId; }


private:
	//=== vars ===//
	uint8_t						m_u8PluginAccess{ 0 };
	uint8_t						m_u8TestOnly{ 0 };
	uint16_t					m_u16Res2{ 0 };
	uint32_t					m_u32Res3{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktRelaySessionReply : public VxPktHdr
{
public:
	PktRelaySessionReply();

    void						setPluginAccess( enum EPluginAccess eAccess )	{ m_u8PluginAccess = (EPluginAccess)eAccess; }
    EPluginAccess               getPluginAccess()								{ return (EPluginAccess)m_u8PluginAccess; }
	void						setTestOnly( uint8_t test )						{ m_u8TestOnly = test; }
	uint8_t						getTestOnly( void )								{ return m_u8TestOnly; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

private:
	//=== vars ===//
	uint8_t						m_u8PluginAccess{ 0 };
	uint8_t						m_u8TestOnly{ 0 };
	uint16_t					m_u16Res2{ 0 };
	uint32_t					m_u32Res3{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

// request connection through remote user's relay
class PktRelayConnectReq : public VxPktHdr
{
public:
	PktRelayConnectReq();
	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setPort( uint16_t port )						{ m_u16Port = htons( port ); }
	uint16_t					getPort( void )									{ return ntohs( m_u16Port ); }

	//=== vars ===//
	VxGUID						m_DestOnlineId; // who connection is intended for
	InetAddress					m_OnlineIp;
private:
	uint16_t					m_u16Port{ 0 };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktRelayConnectReply : public VxPktHdr
{
public:
	PktRelayConnectReply();
	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

private:
	//=== vars ===//
	VxGUID						m_OnlineId;
	uint16_t					m_u16Port{ 0 };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_OnlineIp;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktRelayTestReq : public VxPktHdr
{
public:
	PktRelayTestReq();

    void						setPluginAccess( enum EPluginAccess eAccess );
    EPluginAccess               getPluginAccess();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setTimeStampMs( int64_t timeStamp );
	int64_t	    				getTimeStampMs( void );

private:
	//=== vars ===//
	uint8_t						m_u8Version{ 1 };
	uint8_t						m_u8PluginAccess{ 0 };
	uint8_t						m_u8TestOnly{ 0 };
	uint8_t						m_u8Res1{ 0 };
	uint32_t					m_u32Res3{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	int64_t						m_TimeStampMs{ 0 };
	uint32_t					m_Res4{ 0 };
	uint32_t					m_Res5{ 0 };
};

class PktRelayTestReply : public VxPktHdr
{
public:
	PktRelayTestReply();
    void						setPluginAccess( enum EPluginAccess eAccess );
    EPluginAccess               getPluginAccess();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setTimeStampMs( int64_t timeStamp );
	int64_t					    getTimeStampMs( void );

	void						setMaxRelayUsers( uint32_t maxUsers );
	uint32_t					getMaxRelayUsers( void );

	void						setCurRelayUsers( uint32_t curUsers );
	uint32_t					getCurRelayUsers( void );

private:
	//=== vars ===//
	uint8_t						m_u8Version{ 1 };
	uint8_t						m_u8PluginAccess{ 0 };
	uint8_t						m_u8TestOnly{ 0 };
	uint8_t						m_u8Res1{ 0 };
	uint32_t					m_u32Res3{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	int64_t					    m_TimeStampMs{ 0 };
	uint32_t					m_MaxRelayUsers{ 0 };
	uint32_t					m_CurRelayUsers{ 0 };
};

#pragma pack(pop)

