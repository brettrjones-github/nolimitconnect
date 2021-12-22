#pragma once
//============================================================================
// Copyright (C) 2011 Brett R. Jones 
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

#include <VxUtilLib/VxDefs.h>

class RcSktBase;
class P2PEngine;
class VxGUID;
class RcWebSkt;
class VxNetIdent;

class RcHttpConnection
{
public:
	//=== vars ===//
	P2PEngine *					m_Engine;

	//=== constructor ===//
	RcHttpConnection();
	//=== destructor ===//
	~RcHttpConnection();

	void				httpConnectionStartup( P2PEngine * poEngine );
	void				httpConnectionShutdown( void );

	void				handlePtopConnection( RcSktBase * sktBase );

	RCODE				sendHttpErr( VxNetIdent * netIdent, RcSktBase * sktBase, int iError ); 

	RCODE				sendWebServerData(	bool			bIsGetData,				// if true use PktWebServerGetChunkTx else use PktWebServerPutChunkTx
											VxNetIdent *	netIdent, 
											RcSktBase *		sktBase,					// web socket
											const char *	pData,					// data to send
											int				iDataLen,				// length of data	
											bool			bDisconnectAfterSend,	// if true disconnect after data is sent
											int				iSendTimeoutSeconds );	// timeout attempt to send

private:
	bool				parseOnlineId( RcSktBase * sktBase, VxGUID * poOnlineId );

};

extern RcHttpConnection g_oHttpConnection;