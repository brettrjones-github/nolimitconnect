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
#include <CoreLib/config_corelib.h>

class VxBuffer
{
public:
	VxBuffer( int iPreAllocSize = 512, int iReallocSize = 1024 );
	virtual ~VxBuffer();

	int			                length( );
    int			                freeSpace( );
    void			            resetUsedLen( ) { m_iBufUsedLen = 0; }

	char *		                getDataPtr( void );

	void		                addData( char * pData, int iLen, bool nullTerminate = true );
	int			                readData( char * pData, int iLenToRead, int iReadOffs = 0, bool nullTerminate = true );
	int			                readAndRemoveData( char * pData, int iLenToRead, int iReadOffs = 0, bool nullTerminate = true );
	int			                removeData( int iLen, int iOffs = 0 );

	char *		                getWriteBuffer( int iBufferLen );
	void		                bytesWritten( int iBytesWritten, bool nullTerminate = true );
	
	void		                clear();
	void		                resize( int iAllocLenRequired );

private:
    char *				        m_pBuf;
    int					        m_iBufUsedLen;
    int					        m_iBufAllocTotalLen;
    int					        m_iBufAllocBlockLen;
};

