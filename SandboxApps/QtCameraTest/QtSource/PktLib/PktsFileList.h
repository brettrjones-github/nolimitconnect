#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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

#include "VxPktHdr.h"

#include <CoreLib/VxFileInfo.h>

#include <vector>

#pragma pack(1)

#define MAX_SHARED_FILE_LIST_LEN			12288
#define ERR_NO_SHARED_FILES					0x0001
#define ERR_FILE_LIST_IDX_OUT_OF_RANGE		0x0002

class PktFileListReq : public VxPktHdr
{
public:
	PktFileListReq();

	void						setListIndex( uint32_t index );
	uint32_t					getListIndex( void );

private:
	//=== vars ===//
	uint32_t					m_u16Res;	
	uint32_t					m_u32Index;			
};

class PktFileListReply : public VxPktHdr
{
public:
	PktFileListReply();

	void						calcPktLen( uint16_t dataLen );

	void						setIsListCompleted( bool isCompleted )			{ m_bListComplete = isCompleted ? 1 : 0; }
	bool						getIsListCompleted( void )						{ return m_bListComplete ? true : false; }

	void						setError( uint32_t u32Error );
	uint32_t					getError( void );

	void						setListDataLen( uint16_t dataLen );
	uint16_t					getListDataLen( void );
	void						setFileCount( uint16_t fileCount );
	uint16_t					getFileCount( void );
	void						setListIndex( uint32_t index );
	uint32_t					getListIndex( void );

	void						getFileList( std::vector<VxFileInfo>& retList );
	bool						canAddFile( int fileNameLenIncludingZero );
	void						addFile( VxSha1Hash& fileHashId, uint64_t fileLen, uint8_t fileTypeFlags, const char * fileName );

private:
	//=== vars ===//
	uint8_t						m_bListComplete;		// if 1 then list has been completed
	uint8_t						m_u8Res1;
	uint16_t					m_u16Res;				
	uint16_t					m_u16DataLen;			// length of data
	uint16_t					m_u16FileCnt;			// number of files in this pkt
	uint32_t					m_u32Error;	
	uint32_t					m_u32ListIndex;
	char						m_as8FileList[ MAX_SHARED_FILE_LIST_LEN ];
};

#pragma pack()

