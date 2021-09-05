#ifndef VX_FILE_XFER_INFO_H
#define VX_FILE_XFER_INFO_H

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

#include "VxXferDefs.h"

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

//#define FILE_XFER_OK							0x0001
//#define FILE_XFER_ERR_ALREADY_DOWNLOADING		0x0002
//#define FILE_XFER_ERR_ALREADY_DOWNLOADED		0x0004
//#define FILE_XFER_ERR_FAILED_TO_CREATE			0x0008
//#define FILE_XFER_ERR_BUSY						0x0010
//#define FILE_XFER_ERR_FILE_NOT_FOUND			0x0020
//#define FILE_XFER_ERR_PERMISSION				0x0040

class VxFileXferInfo
{
public:
	VxFileXferInfo();
	VxFileXferInfo( VxGUID& lclSessionId );
	virtual ~VxFileXferInfo();

	bool						calcProgress( void ); // returns true if has changed since last calcProgress was called
	void						setProgress( int percentProgress )			{ m_PercentProgress = percentProgress; }
	int							getProgress( void )							{ return ( m_PercentProgress < 0 ) ? 0 : m_PercentProgress; }

	void						setFileHashId( uint8_t * fileHashId )		{ m_FileHashId.setHashData( fileHashId ); }
	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setAssetId( VxGUID& fileHashId )			{ m_AssetId = fileHashId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileOffset( uint64_t fileOffs )			{ m_u64FileOffs = fileOffs; }
	uint64_t					getFileOffset( void )						{ return m_u64FileOffs; }
	void						setFileLength( uint64_t fileLen )			{ m_u64FileLen = fileLen; }
	uint64_t					getFileLength( void )						{ return m_u64FileLen; }

	void						setXferDirection( EXferDirection dir )		{ m_XferDirection = dir; }
	EXferDirection				getXferDirection( void )					{ return m_XferDirection; }

	void						setLclFileName( const char * fileName )		{ m_strLocalFileName = fileName; }
	std::string&				getLclFileName( void )						{ return m_strLocalFileName; }
	void						setRmtFileName( const char * fileName )		{ m_strRemoteFileName = fileName; }
	std::string&				getRmtFileName( void )						{ return m_strRemoteFileName; }

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	std::string					getDownloadIncompleteFileName( void );
	std::string					getDownloadCompleteFileName( void );

	//=== vars ===//
	FILE *						m_hFile;						
	uint64_t					m_u64FileOffs;					// current offset into file we are at
	uint64_t					m_u64FileLen;					// total file length
	VxGUID						m_AssetId;

protected:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_FileHashId;
	std::string					m_strRemoteFileName;		
	std::string					m_strLocalFileName;				
	EXferDirection				m_XferDirection;
	int							m_PercentProgress;
};

#endif // VX_FILE_XFER_INFO_H
