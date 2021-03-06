#ifndef VX_SKT_CRYPTO_H
#define VX_SKT_CRYPTO_H

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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxDefs.h>

#include <string>

class VxConnectId;
class VxKey;
class VxSktBase;
class VxSktConnectSimple;
class VxGUID;

//! generate key from net identity and connection data and place int sockets m_RxKey and initialize its crypto
RCODE GenerateRxConnectionKey(	VxSktBase *				sktBase,			// pointer to VxSktBase
								VxConnectId *			poConnectId, 		// network identity
								const char *			networkName );
                             
//! generate key from net identity and connection data and place int sockets m_TxKey and initialize its crypto
RCODE GenerateTxConnectionKey(	VxSktBase *				sktBase,			// pointer to VxSktBase
								VxConnectId *			poConnectId,		// announce packet
								const char *			networkName );

RCODE GenerateTxConnectionKey(  VxSktBase *				sktBase,
                                std::string             ipAddr,
                                uint16_t                port,
                                VxGUID                  onlineId,
                                std::string			    networkName );
                          
unsigned char * GetVxSktRandData( int iRandDataIdx );

#endif // VX_SKT_CRYPTO_H
