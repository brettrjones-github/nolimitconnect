#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones 
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

#include "RcScanMatchedConnection.h"
#include "RcScanAction.h"

#include <GuiInterface/IScan.h>
#include <PktLib/PktsScan.h>
#include <PktLib/PktsImAlive.h>
#include <CoreLib/VxGUIDList.h>

class PktFindFileReq;
class PktFindFileReply;
class P2PEngine;
class P2PConnectList;

class RcScan : public IScan
{
public:
	RcScan( P2PEngine& engine, P2PConnectList& connectList );
	virtual ~RcScan() = default;

	RcScanAction&				getScanAction( void )		{ return m_ScanAction; }
	EScanType					getScanType( void )			{ return m_ScanAction.getScanType(); }

	void						scanShutdown( void );

	void						fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern = "" );
	void						fromGuiNextScan( EScanType eScanType );
	void						fromGuiStopScan( EScanType eScanType );

	void						onOncePer30Seconds( void );
	void						onOncePerMinute( void );

	void						onIdentDelete( VxNetIdent * netIdent );

	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );	
	//! called when new better connection from user
	void						replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	void						onPktScanReq( VxNetIdent * netIdent, VxSktBase * sktBase, PktScanReq * poPkt );
	void						onPktScanReply( VxNetIdent * netIdent, VxSktBase * sktBase, PktScanReply * poPkt );
	void						onPktFindFileReq( VxNetIdent * netIdent, VxSktBase * sktBase, PktFindFileReq * poPkt );
	void						onPktFindFileReply( VxNetIdent * netIdent, VxSktBase * sktBase, PktFindFileReply * poPkt );

	virtual void				onScanResultProfilePic(	VxNetIdent *	netIdent, 
															VxSktBase *		sktBase, 
															uint8_t *			pu8JpgData, 
															uint32_t				u32JpgDataLen );

	virtual void				onScanResultError(	EScanType			eScanType,
														VxNetIdent *		netIdent, 
														VxSktBase *			sktBase, 
														uint32_t					errCode ); 

	void						scanComplete( void );

	void						actionThreadRunning( bool isRunning );

	bool						isLocalSearchMatch( VxNetIdent * netIdent );
	bool						isRemoteSearchMatch( VxNetIdent * netIdent, PktScanReq * poPkt );


protected:
	void						searchMsgToUser( const char * msgToUser, ... );


	P2PEngine&					m_Engine;
	P2PConnectList&				m_ConnectList;

	bool						m_bIsScanning;
	bool						m_bActionThreadRunning;

	EScanType					m_eScanType;

	RcScanAction				m_ScanAction;

	PktScanReq				m_SearchPkt;

	VxGUIDList					m_IdentsReqConnectList; 
	VxGUIDList					m_IdentsSentSearchPktList; 
	VxGUIDList					m_IdentsWithSearchMatchList; 
	VxMutex						m_ScanMutex;

	uint32_t					m_u32ToGuiSendPicTime;
	uint32_t					m_u32SearchActionComplete;
	int64_t						m_s64LastActionTimeMs;
};

