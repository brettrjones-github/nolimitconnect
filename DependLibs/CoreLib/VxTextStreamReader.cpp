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
#include "config_corelib.h"

#include "VxTextStreamReader.h"
namespace
{
	const char LF = '\n';
	const char CR = '\r';
}

//============================================================================
VxTextStreamReader::VxTextStreamReader()
: m_iReadIdx(0)
{

}

//============================================================================
void VxTextStreamReader::setStreamData( const char* pData )
{
	m_strData = pData;
	m_iReadIdx = 0;
}

//============================================================================
const char* VxTextStreamReader::getStreamData( void )
{
	return m_strData.c_str();
}

//============================================================================
int VxTextStreamReader::skipStreamData( int iSkipLen )
{
	m_iReadIdx += iSkipLen;
	return iSkipLen;
}


bool VxTextStreamReader::readStreamLine( std::string& strRetLine )
{
	char readChar = 0;
	strRetLine = "";

	// Read line until CR & LF
	int	readLen = readStreamData( strRetLine, 1 );
	if( 0 >= readLen )
	{
		return false;
	}

	while( 0 < readLen ) 
	{
		int lastPos = strRetLine.size() - 1;
		readChar = strRetLine.at( lastPos );
		if (readChar == CR || readChar == LF) 
		{
			strRetLine = strRetLine.substr(0, lastPos);
			break;
		}
		readLen = readStreamData( strRetLine, 1 );
	}

	if (readChar == CR) 
	{
		std::string skipChar;
		readLen = readStreamData( skipChar, 1 );
		if( 0 < readLen ) 
		{
			readChar = skipChar.at(0);
			if (readChar != LF)
			{
				unwindStreamReadPosition(1);
			}
		}
	}

	return ( 0 < strRetLine.length() ) ? true : false;
}

//============================================================================
int VxTextStreamReader::readStreamData( std::string& strRetData, int iLenToRead )
{
	int iBufLen = m_strData.length() - m_iReadIdx;
	if( iBufLen <= 0 )
	{
		return 0;
	}
	int iCopyLen = ( iLenToRead < iBufLen ) ? iLenToRead : iBufLen;
	strRetData.append( m_strData, m_iReadIdx, iCopyLen );
	m_iReadIdx += iCopyLen;
	return iCopyLen;
}

//============================================================================
void VxTextStreamReader::resetStreamReadPosition( void )
{
	m_iReadIdx = 0;
}

//============================================================================
void VxTextStreamReader::unwindStreamReadPosition( int iLen )
{
	if( iLen <= m_iReadIdx )
	{
		m_iReadIdx -= iLen;
	}
	else
	{
		// TODO Error handling
	}
}

