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

#include <NetLib/VxSktBase.h>

class P2PEngine;

class VxSktLoopback : public VxSktBase
{
public:
    VxSktLoopback() = delete;	
    VxSktLoopback( P2PEngine& engine );	
	virtual ~VxSktLoopback() = default;

    virtual bool				isConnected( void ) override                { return true; }

    virtual RCODE				txPacketWithDestId(	VxPktHdr * pktHdr, bool	bDisconnect = false ) override;	

    //! return true if transmit encryption key is set
    virtual bool				isTxEncryptionKeySet( void ) override        { return true; }
    //! return true if receive encryption key is set
    virtual bool				isRxEncryptionKeySet( void ) override        { return true; }

protected:
    P2PEngine&                  m_Engine;
};
