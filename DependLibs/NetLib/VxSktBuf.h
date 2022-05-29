#pragma once
//============================================================================
// Copyright (C) 2008 Brett R. Jones 
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

#include <CoreLib/VxMutex.h>

class VxSktBuf
{
public:
	
	VxSktBuf( int iBufSize = 16384 );
	
    virtual ~VxSktBuf();

	//=== read from buf functions called by application ===//
	// return amount of data in buffer
	virtual int					getSktBufDataLen( void );
	// app should get the buffer ( this also locks it from being modified by threads )
	// then read the data then call Amount read
	virtual unsigned char *		getSktReadBuf( void );
	//! tell buffer how much was read.. also releases lock.. return -1 if larger amount than possible
	virtual int					sktBufAmountRead( int iAmountRead );
	//! Get the size of the socket buffer
	virtual int					getSktBufSize( void )		{ return m_iSktBufSize; }
	virtual uint32_t					getRxDecryptedLen( void )	{ return m_u32RxDecryptedLen; }	// amount of receive data decrypted
	
	//=== write to buf functions called by socket threads ===//
	//! get amount of space available
	virtual int					getSktBufFreeSpace( void );
	//! get the buffer to write into.. this also locks it
	virtual unsigned char *		getSktWriteBuf( void );
	//! tell buffer how much was written.. also releases lock.. return -1 if larger amount than possible
	virtual int					sktBufAmountWrote( int iAmountWrote );

	//=== first packet ===//
	virtual bool				isFirstRxPacket( void )						{ return m_bFirstRxPacket; }
	virtual void				setIsFirstRxPacket( bool bIsFirstPacket )	{ m_bFirstRxPacket = bIsFirstPacket; }

protected:	
	virtual void				doLock( void )		{ m_Mutex.lock();}
	virtual void				doUnlock( void )	{ m_Mutex.unlock();}

	uint32_t							m_u32RxDecryptedLen;	// amount of receive data decrypted
	unsigned char *				m_pau8SktBuf;			// socket buffer
	int							m_iSktBufSize;			// total size of buffer

	int							m_iSktBufIdx;			// current index into buffer
	bool						m_bFirstRxPacket;		// app can set if is first packet or not

	VxMutex						m_Mutex;
};
