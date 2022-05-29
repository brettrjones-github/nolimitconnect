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

#include "VxFileXferInfo.h"

#include <vector>

class VxSktBase;
class VxNetIdentBase;

class VxBaseSktInfo
{
public:
	// base class for per connection info.. contains
	// whats needed for file transfers

	VxBaseSktInfo();
	virtual ~VxBaseSktInfo();

	//! return true if file is already in que to be sent or has been sent
	bool isFileQuedOrSent( const char * pFileName );
	//! que a file to send
	void queFileToSend( const char * pLclFileName, const char * pRmtFileName );

protected:
	VxSktBase *					m_Skt;			// socket
	uint32_t							m_u32PeerIp;		// peer's ip in binary form
	char						m_as8PeerIp[16];	// peer's ip in ascii form
	uint32_t							m_u32LocalIp;		// our ip in binary form
	char						m_as8LocalIp[16];	// our ip in ascii form
	uint32_t							m_u32LastRxTime;	// last time received a packet
	//=== identity vars ===//
	VxNetIdentBase *			m_Ident;			// network identity
	std::string					m_csUserName;		// users login name

	//=== file tx/rx vars ===//
	VxFileXferInfo				m_TxFileInfo;		// file being transmitted
	VxFileXferInfo				m_RxFileInfo;		// file being received
	std::vector<std::string>	m_astrFilesToSend;		// list of files to send
	std::vector<std::string>	m_astrFilesSent;		// list of files sent
	std::vector<std::string>	m_astrFilesReceived;	// list of files received

};
