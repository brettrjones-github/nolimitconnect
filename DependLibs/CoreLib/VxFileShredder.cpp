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
// http://www.nolimitconnect.com
//============================================================================

#include "config_corelib.h"

#include "VxFileShredder.h"
#include "VxFileUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace
{
	//============================================================================
	static uint32_t FileShredderThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		VxFileShredder * shredder = (VxFileShredder *)poThread->getThreadUserParam();
		shredder->shredFiles();
		poThread->threadAboutToExit();
		return 0;
	}

	VxFileShredder * g_FileShredder = 0;
}

//============================================================================
VxFileShredder& GetVxFileShredder( void )
{
	if( 0 == g_FileShredder )
	{
		g_FileShredder = new VxFileShredder();
	}

	return *g_FileShredder;
}

//============================================================================
VxFileShredder::VxFileShredder()
: m_ShredderDb()
, m_ShredListMutex()
, m_ShredThread()
, m_ShredList()
{
}

//============================================================================
VxFileShredder::~VxFileShredder()
{
}

//============================================================================
void VxFileShredder::initShredder( std::string& dataDirectory )
{
	m_ShredListMutex.lock();
	m_ShredderDb.initShredderDb( dataDirectory );
	m_ShredderDb.getShredList( m_ShredList );
	if( m_ShredList.size() )
	{
		startThreadIfNotStarted();
	}

	m_ShredListMutex.unlock();	
}

//============================================================================
void VxFileShredder::shredFile( std::string& fileName )
{
	m_ShredListMutex.lock();
	std::vector<std::string>::iterator iter;
	for( iter = m_ShredList.begin(); iter != m_ShredList.end(); ++iter )
	{
		if( *iter == fileName )
		{
			// already in list to shred
			m_ShredListMutex.unlock();
			return;
		}
	}

	m_ShredderDb.addFileToShred( fileName );
	m_ShredList.push_back( fileName );
	startThreadIfNotStarted();
	m_ShredListMutex.unlock();	
}

//============================================================================
void VxFileShredder::startThreadIfNotStarted( void )
{
	if( false == m_ShredThread.isThreadRunning() )
	{
		m_ShredThread.startThread( (VX_THREAD_FUNCTION_T)FileShredderThreadFunc, this, "ShredFilesThread" );
	}
}

#define SHRED_BUF_SIZE 1024 * 16
//============================================================================
void VxFileShredder::shredFiles( void )
{
	while( false == m_ShredThread.isAborted() )
	{
		std::string fileName ;
		if( 0 != m_ShredList.size() )
		{
			m_ShredListMutex.lock();
			fileName = m_ShredList[0];
			m_ShredList.erase( m_ShredList.begin() );
			m_ShredListMutex.unlock();	

			uint64_t len = VxFileUtil::fileExists( fileName.c_str() );
			if( len )
			{
				char randBuf[ SHRED_BUF_SIZE ];
				int * intPtr = (int *)randBuf;
				int randIdxCnt = SHRED_BUF_SIZE / sizeof(int);
				FILE * fileHandle = fopen( fileName.c_str(), "rb+" );
				if( fileHandle )
				{
					while( len )
					{
						// fill buffer with random data
						for( int i = 0; i < randIdxCnt; i++ )
						{
							intPtr[i] = rand();
						}

						uint32_t lenToWrite = len > SHRED_BUF_SIZE ? SHRED_BUF_SIZE : (uint32_t)len;
						fwrite( randBuf, 1, lenToWrite, fileHandle );
						len -= lenToWrite;

						if( m_ShredThread.isAborted() )
						{
							fclose( fileHandle );
							m_ShredThread.threadAboutToExit();
							return;
						}

						// this can be done slowly
						VxSleep( 10 );
					}

					fclose( fileHandle );
					VxFileUtil::deleteFile( fileName.c_str() );
				}
				else
				{
					LogMsg( LOG_ERROR, "FileShredder::shredFiles could not open file %s\n", fileName.c_str() );
					VxFileUtil::deleteFile( fileName.c_str() );
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "FileShredder::shredFiles 0 len file %s\n", fileName.c_str() );
				VxFileUtil::deleteFile( fileName.c_str() );
			}

			m_ShredListMutex.lock();
			m_ShredderDb.removeFileToShred( fileName );
			m_ShredListMutex.unlock();	
		}

		m_ShredListMutex.lock();	
		if( 0 == m_ShredList.size() )
		{
			m_ShredListMutex.unlock();	
			break;
		}
		else
		{
			m_ShredListMutex.unlock();	
		}
	}
}
