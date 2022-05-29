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


#include "VxSktBuf.h"

#include <memory.h>

#define VxSKT_ENABLE_BUF_LOCKS 1

//============================================================================
VxSktBuf::VxSktBuf( int iBufSize )
: m_u32RxDecryptedLen(0)	// amount of receive data decrypted
, m_pau8SktBuf(0)			// socket buffer
, m_iSktBufSize(iBufSize)	// total size of buffer
, m_iSktBufIdx(0)			// current index into buffer
, m_bFirstRxPacket(true)		// app can set if is first packet or not
{
	vx_assert( iBufSize );
	m_pau8SktBuf = new uint8_t[ iBufSize + 24 ];
}

//============================================================================
VxSktBuf::~VxSktBuf()
{
    delete m_pau8SktBuf;
    m_pau8SktBuf = nullptr;
}

//============================================================================
//=== read from buf functions called by application ===//
//============================================================================
//============================================================================
//! get amount of date available
int VxSktBuf::getSktBufDataLen( void )
{
	return m_iSktBufIdx;
}

//============================================================================
// app should get the buffer ( this also locks it from being modified by threads )
// then read the data then call Amount read
unsigned char * VxSktBuf::getSktReadBuf( void )
{
#ifdef VxSKT_ENABLE_BUF_LOCKS
	m_Mutex.lock(__FILE__, __LINE__);
#endif // VxSKT_ENABLE_BUF_LOCKS
	return (unsigned char *)m_pau8SktBuf;
}
//============================================================================
//! tell buffer how much was read.. also releases lock.. return -1 if larger amount than possible
int VxSktBuf::sktBufAmountRead( int iAmountRead )
{
	if( iAmountRead > m_iSktBufIdx )
	{
		#ifdef VxSKT_ENABLE_BUF_LOCKS
			m_Mutex.unlock(__FILE__, __LINE__);
		#endif // VxSKT_ENABLE_BUF_LOCKS
		vx_assert( false );
		return -1;
	}

	if( iAmountRead )
	{
		if( m_u32RxDecryptedLen )
		{
			vx_assert( (int)m_u32RxDecryptedLen >= iAmountRead );
			m_u32RxDecryptedLen -= iAmountRead;
		}

		if( iAmountRead < m_iSktBufIdx )
		{
			//got some left over
			int iAmountLeft = m_iSktBufIdx - iAmountRead;
			memmove( m_pau8SktBuf, &m_pau8SktBuf[ iAmountRead ], iAmountLeft );
			m_iSktBufIdx = iAmountLeft;
		}
		else
		{
			m_iSktBufIdx = 0;
		}
	}

	#ifdef VxSKT_ENABLE_BUF_LOCKS
		m_Mutex.unlock(__FILE__, __LINE__);
	#endif // VxSKT_ENABLE_BUF_LOCKS
	return 0;
}


//============================================================================
//=== write to buf functions called by socket threads ===//
//============================================================================
//============================================================================
//! get amount of space available
int VxSktBuf::getSktBufFreeSpace( void )
{
	return m_iSktBufSize - m_iSktBufIdx;
}

//============================================================================
//! get the buffer to write into.. this also locks it
unsigned char * VxSktBuf::getSktWriteBuf( void )
{
	#ifdef VxSKT_ENABLE_BUF_LOCKS
		m_Mutex.lock(__FILE__, __LINE__);
	#endif // VxSKT_ENABLE_BUF_LOCKS
	return &m_pau8SktBuf[ m_iSktBufIdx ];
}

//============================================================================
//! tell buffer how much was written.. also releases lock.. return -1 if larger amount than possible
int VxSktBuf::sktBufAmountWrote( int iAmountWrote )
{
	if( iAmountWrote > getSktBufFreeSpace() )
	{
		#ifdef VxSKT_ENABLE_BUF_LOCKS
			m_Mutex.unlock(__FILE__, __LINE__);
		#endif // VxSKT_ENABLE_BUF_LOCKS
		vx_assert( false );
		return -1;
	}

	m_iSktBufIdx += iAmountWrote;
	// null terminate just in case
	m_pau8SktBuf[ m_iSktBufIdx ] = 0;
	#ifdef VxSKT_ENABLE_BUF_LOCKS
		m_Mutex.unlock( __FILE__, __LINE__);
	#endif // VxSKT_ENABLE_BUF_LOCKS
	return 0;
}

