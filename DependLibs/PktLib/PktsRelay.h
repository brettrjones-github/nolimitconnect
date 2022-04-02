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

#pragma pack(push) 
#pragma pack(1)

class PktRelayServiceReq : public VxPktHdr
{
public:
	PktRelayServiceReq();

	void						setLclSessionId( VxGUID& lclId )					{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )								{ return m_LclSessionId; }

	uint8_t						m_u8CancelService; // 0 = request access 1 = cancel access
private:
	//=== vars ===//
	uint8_t						m_u8Res1;
	uint16_t					m_u16Res;
	uint32_t					m_u32Res2;
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

	void						setAccessState( EPluginAccess ePluginAccess )  { m_u32Error = (uint32_t)ePluginAccess; }
	EPluginAccess			getAccessState( void )								{ return (EPluginAccess)m_u32Error; }
	void						setRelayStatus( ERelayStatus eStatus )				{ m_u8RelayStatus = (uint8_t)eStatus; }
	ERelayStatus				getRelayStatus( void )								{ return (ERelayStatus)m_u8RelayStatus; }

private:
	//=== vars ===//
	uint8_t						m_u8RelayStatus;
	uint8_t						m_u8AccessState;
	uint16_t					m_u16Res;
	uint32_t					m_u32Error;
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
	uint16_t					m_u16Res;
	uint32_t					m_u32Res1;
	uint32_t					m_u32Res2;
	uint32_t					m_u32Res3;
	uint32_t					m_u32Res4;
};

class PktRelayConnectToUserReply : public VxPktHdr
{
public:
	PktRelayConnectToUserReply();

	void						setConnectFailed( uint16_t failCode )	{ m_u16ConnectFailed = htons( failCode ); }
	uint16_t					getConnectFailed( void )			{ return ntohs( m_u16ConnectFailed ); }

	//=== vars ===//
	VxConnectId					m_ConnectId;
private:
	uint16_t					m_u16ConnectFailed;
	uint32_t					m_u32Res1;
	uint32_t					m_u32Res2;
	uint32_t					m_u32Res3;
	uint32_t					m_u32Res4;
};

class PktRelayUserDisconnect : public VxPktHdr
{
public:
	PktRelayUserDisconnect();

	//=== vars ===//
	VxGUID						m_UserId;
private:
	uint32_t					m_u32Res1;
	uint32_t					m_u32Res2;
};

class PktRelaySessionReq : public VxPktHdr
{
public:
	PktRelaySessionReq();

	void						setPluginAccess( EPluginAccess eAccess );
	EPluginAccess			getPluginAccess();
	void						setTestOnly( uint8_t test )							{ m_u8TestOnly = test; }
	uint8_t						getTestOnly( void )									{ return m_u8TestOnly; }

	void						setLclSessionId( VxGUID& rmtId )					{ m_LclSessionId = rmtId; }
	VxGUID&						getLclSessionId( void )								{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& lclId )					{ m_RmtSessionId = lclId; }
	VxGUID&						getRmtSessionId( void )								{ return m_RmtSessionId; }


private:
	//=== vars ===//
	uint8_t						m_u8PluginAccess;
	uint8_t						m_u8TestOnly;
	uint16_t					m_u16Res2;
	uint32_t					m_u32Res3;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktRelaySessionReply : public VxPktHdr
{
public:
	PktRelaySessionReply();

	void						setPluginAccess( EPluginAccess eAccess )	{ m_u8PluginAccess = (EPluginAccess)eAccess; }
	EPluginAccess			getPluginAccess()								{ return (EPluginAccess)m_u8PluginAccess; }
	void						setTestOnly( uint8_t test )						{ m_u8TestOnly = test; }
	uint8_t						getTestOnly( void )								{ return m_u8TestOnly; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

private:
	//=== vars ===//
	uint8_t						m_u8PluginAccess;
	uint8_t						m_u8TestOnly;
	uint16_t					m_u16Res2;
	uint32_t					m_u32Res3;
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
	uint16_t					m_u16Port;
	uint16_t					m_u16Res1;
	uint32_t					m_u32Res2;
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
	uint16_t					m_u16Port;
	uint16_t					m_u16Res1;
	uint32_t					m_OnlineIp;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktRelayTestReq : public VxPktHdr
{
public:
	PktRelayTestReq();

	void						setPluginAccess( EPluginAccess eAccess );
	EPluginAccess			getPluginAccess();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setTimeStampMs( int64_t timeStamp );
	int64_t	    				getTimeStampMs( void );

private:
	//=== vars ===//
	uint8_t						m_u8Version;
	uint8_t						m_u8PluginAccess;
	uint8_t						m_u8TestOnly;
	uint8_t						m_u8Res1;
	uint32_t					m_u32Res3;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	int64_t						m_TimeStampMs;
	uint32_t					m_Res4;
	uint32_t					m_Res5;
};

class PktRelayTestReply : public VxPktHdr
{
public:
	PktRelayTestReply();
	void						setPluginAccess( EPluginAccess eAccess );
	EPluginAccess			getPluginAccess();

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
	uint8_t						m_u8Version;
	uint8_t						m_u8PluginAccess;
	uint8_t						m_u8TestOnly;
	uint8_t						m_u8Res1;
	uint32_t					m_u32Res3;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	int64_t					    m_TimeStampMs;
	uint32_t					m_MaxRelayUsers;
	uint32_t					m_CurRelayUsers;
};

#pragma pack()
#pragma pack(pop)
