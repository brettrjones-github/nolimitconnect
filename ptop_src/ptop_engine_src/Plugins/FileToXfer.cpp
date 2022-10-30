//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "FileToXfer.h"

//============================================================================
FileToXfer::FileToXfer( FileInfo& fileInfo, VxGUID& lclSessionId, VxGUID& rmtSessionId )
: FileInfo( fileInfo )
, m_LclSessionId( lclSessionId )
, m_RmtSessionId( rmtSessionId )
{
}

//============================================================================
FileToXfer::FileToXfer( const FileToXfer& rhs )
    : FileInfo( rhs )
    , m_LclSessionId( rhs.m_LclSessionId )
    , m_RmtSessionId( rhs.m_RmtSessionId )
    , m_FileOffset( rhs.m_FileOffset )
{

}

//============================================================================
FileToXfer& FileToXfer::operator=(const FileToXfer& rhs) 
{	
	if( &rhs != this )
	{
		*((FileInfo*)this) = rhs;

		m_LclSessionId = rhs.m_LclSessionId;
		m_RmtSessionId = rhs.m_RmtSessionId;
		m_FileOffset = rhs.m_FileOffset;
	}

	return *this;
}

