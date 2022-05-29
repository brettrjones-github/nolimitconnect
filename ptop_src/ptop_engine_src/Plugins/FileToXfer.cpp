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
FileToXfer::FileToXfer(std::string		strFileName, 
						uint64_t		u64FileOffset,  
						VxGUID&			lclSessionId, 
						VxGUID&			rmtSessionId,
						VxGUID&			fileAssetId,
						VxSha1Hash		fileHashId,
						int				userData )
: m_strFileName(strFileName)
, m_u64FileOffset( u64FileOffset )
, m_UserData( userData )
, m_LclSessionId( lclSessionId )
, m_RmtSessionId( rmtSessionId )
, m_FileAssetId( fileAssetId )
, m_FileHashId( fileHashId )
{
}

//============================================================================
FileToXfer& FileToXfer::operator=(const FileToXfer& rhs) 
{	
	m_strFileName				= rhs.m_strFileName;
	m_u64FileOffset				= rhs.m_u64FileOffset;
	m_UserData					= rhs.m_UserData;
	m_LclSessionId				= rhs.m_LclSessionId;
	m_RmtSessionId				= rhs.m_RmtSessionId;
	m_FileAssetId				= rhs.m_FileAssetId;
	m_FileHashId				= rhs.m_FileHashId;
	return *this;
}

