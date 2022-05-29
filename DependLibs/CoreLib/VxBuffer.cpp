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
// brett.jones@engineer.com
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "VxDefs.h"
#include "VxBuffer.h"
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <algorithm>    // std::min

//============================================================================
VxBuffer::VxBuffer( int iPreAllocSize, int iReallocSize )
: m_pBuf(NULL)
, m_iBufUsedLen(0)
, m_iBufAllocTotalLen( 0 )
, m_iBufAllocBlockLen(iReallocSize)
{
	resize( iPreAllocSize );
}

//============================================================================
VxBuffer::~VxBuffer()
{
	clear();
}

//============================================================================
int VxBuffer::length( void )
{
	return m_iBufUsedLen;
}

//============================================================================
int VxBuffer::freeSpace( )
{
    return m_iBufAllocTotalLen - m_iBufUsedLen;
}

//============================================================================
char * VxBuffer::getDataPtr( void )
{
	return m_pBuf;
}

//============================================================================
void VxBuffer::addData( char * pData, int iLen, bool nullTerminate )
{
	int iAllocLenRequired = m_iBufUsedLen + iLen + (nullTerminate ? 1 : 0);
	if( iAllocLenRequired > m_iBufAllocTotalLen )
	{
		resize( iAllocLenRequired );
	}

	memcpy( &m_pBuf[ m_iBufUsedLen ], pData, iLen );
	m_iBufUsedLen += iLen;
	if( nullTerminate )
	{
		m_pBuf[ m_iBufUsedLen ] = 0;
	}
}

//============================================================================
int VxBuffer::readData( char * pData, int iLenToRead, int iReadOffs, bool nullTerminate )
{
	int iBytesAvail = m_iBufUsedLen - iReadOffs; 
    int iCopyLen = std::min( iLenToRead, iBytesAvail );
	if( 0 < iCopyLen )
	{
		memcpy( pData, &m_pBuf[ iReadOffs ], iCopyLen );
		if( nullTerminate )
		{
			pData[ iCopyLen ] = 0;
		}
	}
	return iCopyLen;
}

//============================================================================
int VxBuffer::readAndRemoveData( char * pData, int iLenToRead, int iReadOffs, bool nullTerminate )
{
	int iRemoveLen = readData( pData, iLenToRead, iReadOffs, nullTerminate );
	if( 0 < iRemoveLen )
	{
		removeData( iRemoveLen, iReadOffs );
	}
	return iRemoveLen;
}

//============================================================================
int VxBuffer::removeData( int iLen, int iOffs )
{
	int iRemoveLen = 0;
	int iBytesAvail = m_iBufUsedLen - iOffs; 
	if( 0 < iBytesAvail )
	{
        iRemoveLen = std::min( iBytesAvail, iLen );
		if( 0 < iRemoveLen )
		{
			int iLenToMove = m_iBufUsedLen - ( iRemoveLen + iOffs );
			if( 0 < iLenToMove )
			{
				memcpy( &m_pBuf[ iOffs ], &m_pBuf[ iOffs + iLen ], iLenToMove );
			}
			m_iBufUsedLen -= iRemoveLen;
		}
	}
	return iRemoveLen;
}

//============================================================================
char * VxBuffer::getWriteBuffer( int iBufferLen )
{
	int iBufLenRequired =  iBufferLen + 1 + m_iBufUsedLen;
	if( m_iBufAllocTotalLen < iBufLenRequired )
	{
		resize( iBufLenRequired );
	}

	return &m_pBuf[ m_iBufUsedLen ];
}

//============================================================================
void VxBuffer::bytesWritten( int iBytesWritten, bool nullTerminate )
{
	m_iBufUsedLen += iBytesWritten;
	if( nullTerminate )
	{
		m_pBuf[ m_iBufUsedLen ] = 0;
	}
}

//============================================================================
void VxBuffer::clear()
{
	m_iBufUsedLen = 0;
	m_iBufAllocTotalLen = 0;

	free( m_pBuf );
	m_pBuf = NULL;
}

//============================================================================
void VxBuffer::resize( int iAllocLenRequired )
{
	iAllocLenRequired = ( ( iAllocLenRequired / m_iBufAllocBlockLen ) + ((iAllocLenRequired & m_iBufAllocBlockLen) ? 2 : 1) ) * m_iBufAllocBlockLen;
	if( 0 < iAllocLenRequired )
	{
		if( m_pBuf )
		{
			m_pBuf = (char *)realloc( m_pBuf, iAllocLenRequired );
		}
		else
		{
			m_pBuf = (char *)malloc( iAllocLenRequired );
			*m_pBuf = 0;
		}
	}

	m_iBufAllocTotalLen = iAllocLenRequired;
}
