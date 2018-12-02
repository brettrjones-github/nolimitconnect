#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

enum ENetActionType
{
	eNetActionUnknown			= 0,
	eNetActionIdle				= 1,
	eNetActionQueryMyIp			= 2,
	eNetActionAnnounce			= 3,
	eNetActionIsPortOpen		= 4,
	eNetActionPhoneShake		= 5,

	eMaxNetAction
};

class P2PEngine;
class NetServicesMgr;
class NetServiceUtils;
class VxGUID;

class NetActionBase
{
public:
	NetActionBase( NetServicesMgr& netServicesMgr );
	virtual ~NetActionBase();

	virtual ENetActionType		getNetActionType( void )			{ return eNetActionUnknown; }
	virtual VxGUID&			getMyOnlineId( void );

	virtual void				enterAction( void )					{};
	virtual void				doAction( void )					{};
	virtual void				exitAction( void )					{};

protected:
	NetServicesMgr&				m_NetServicesMgr;
	NetServiceUtils&			m_NetServiceUtils;
	P2PEngine&					m_Engine;
};

