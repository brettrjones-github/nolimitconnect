#pragma once
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

#include "FileInfo.h"

class FileToXfer : public FileInfo
{
public:
	FileToXfer( FileInfo& fileInfo, VxGUID&	lclSessionId, VxGUID& rmtSessionId );				

	FileToXfer& operator=( const FileToXfer& fileXferInfo );

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setFileOffset( uint64_t fileOffset )		{ m_FileOffset = fileOffset; }
	uint64_t					getFileOffset( void )						{ return m_FileOffset; }

protected:
	//=== vars ===//
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint64_t					m_FileOffset{ 0 };
};
