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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>

#include <string>

class FileToXfer
{
public:
	FileToXfer( std::string		strFileName, 
				uint64_t		u64FileOffset, 
				VxGUID&			lclSessionId,
				VxGUID&			rmtSessionId, 
				VxGUID&			fileAssetId,
				VxSha1Hash		fileHashId,
				int				userData );

	FileToXfer& operator=( const FileToXfer& oInfo );

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& rmtId )					{ m_FileAssetId = rmtId; }
	VxGUID&						getAssetId( void )							{ return m_FileAssetId; }

	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	//=== vars ===//
    std::string					m_strFileName;
	uint64_t					m_u64FileOffset;
	int							m_UserData;

protected:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
};
