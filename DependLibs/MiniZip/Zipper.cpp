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

#include "Zipper.h"
#include "MiniUnzip.h"
#include "MiniZip.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxMutex.h>

#include <string.h>

//#define DEBUG_ZIPPER 1
//#define DEBUG_UNZIP  1

namespace
{
	const int		g_defaultCompressionRate	=	-1;

	int32_t DoZipThreadFunc(  void * pvContext )
	{
#ifdef DEBUG_ZIPPER
		LogMsg( LOG_INFO, "Zipper::zipFiles compress in thread DoZipThreadFunc start\n" );
#endif // DEBUG_ZIPPER
		VxThread * poVxThread = (VxThread *)pvContext;
		poVxThread->setIsThreadRunning( true );
		Zipper * zipper = (Zipper *)poVxThread->getThreadUserParam();
        if( zipper )
        {
            zipper->doZipOperation();
    #ifdef DEBUG_ZIPPER
            LogMsg( LOG_INFO, "Zipper::zipFiles compress in thread DoZipThreadFunc done\n" );
    #endif // DEBUG_ZIPPER
        }

		poVxThread->threadAboutToExit();
		return 0;
	}

	int32_t DoUnzipThreadFunc(  void * pvContext )
	{
		VxThread * poVxThread = (VxThread *)pvContext;
		poVxThread->setIsThreadRunning( true );
		Zipper * zipper = (Zipper *)poVxThread->getThreadUserParam();
        if( zipper )
        {
    #ifdef DEBUG_UNZIP
            LogMsg( LOG_INFO, "DoUnzipThreadFunc start\n" );
    #endif // DEBUG_UNZIP
            zipper->doUnzipOperation();
    #ifdef DEBUG_UNZIP
            LogMsg( LOG_INFO, "DoUnzipThreadFunc about to exit\n" );
    #endif // DEBUG_UNZIP
        }

		poVxThread->threadAboutToExit();
		return 0;
	}

	VxMutex				g_oZipperMutex;
	volatile bool		operationInProgess = false;

	void setOperationInProgress( bool inProgress = true )
	{
		if( inProgress )
		{
			g_oZipperMutex.lock();
			while( operationInProgess )
			{
				VxSleep( 100 );
			}
			operationInProgess = true;

			g_oZipperMutex.unlock();
		}
		else
			operationInProgess = false;
	}
}


#ifdef __cplusplus
extern "C" {
#endif

void Zip_GetNextFileName( void * zipper )
{
	if( zipper )
	{
		((Zipper*)zipper)->getNextFileToZip();
	}
}

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////
Zipper::ToZipFile::ToZipFile( const char * fromDir, const char * filePathAndName )
: m_FromDirectory( fromDir )
, m_ToZipFile( filePathAndName )
{
}

Zipper::ToZipFile::ToZipFile(const ToZipFile& rhs)
{
	*this = rhs;
}

Zipper::ToZipFile& Zipper::ToZipFile::operator=(const ToZipFile& rhs)
{
	m_FromDirectory		= rhs.m_FromDirectory;
	m_ToZipFile			= rhs.m_ToZipFile; 
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
Zipper::Zipper( IZipperCallback& toCaller )
: m_ToCaller( toCaller )
, m_iCompressionRate( Z_DEFAULT_COMPRESSION )
, m_uiZipFilesIndex( 0 )
{
}

Zipper::~Zipper()
{
}

void Zipper::clearZipFileList( void )
{
	m_ToZipFiles.clear();
}

void Zipper::addToZipDirectory( const char * dir  )
{
	std::vector<std::string> fileList;
	listFilesInDirectory( dir, fileList );

	std::vector<std::string>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		addToZipFile( (*iter).c_str(), dir );
	}
}

void Zipper::addToZipFile( const char * file, const char * dirToTruncate )
{
	// strip off directory files are actually in.. this will be the working dir
	char buf[ MAX_PATH ];
	strcpy( buf, dirToTruncate );
	char * pTemp = strrchr( buf, '/');
	if( pTemp )
	{
		* pTemp = 0;
	}
	pTemp = strrchr( buf, '/');
	if( pTemp )
	{
		pTemp++;
		* pTemp = 0;
	}
	m_ToZipFiles.push_back( ToZipFile( buf, &file[strlen( buf ) ] ) );
}

int Zipper::getToZipFileCount( void )
{
	return (int)m_ToZipFiles.size();
}

void Zipper::setCompressionRate( int rate0to9_0faster_9better  )
{
	m_iCompressionRate = rate0to9_0faster_9better;	
}

void Zipper::zipFiles( const char * zipfileName, bool compressInThread )
{
#ifdef DEBUG_ZIPPER
	LogMsg( LOG_INFO, "Zipper::zipFiles start filename %s\n", m_ZipfileName.c_str() );
#endif // DEBUG_ZIPPER
	if( 0 == m_ToZipFiles.size() )
	{
		LogMsg( LOG_ERROR, "Zipper::zipFiles: NO files to zip\n" );
		return;
	}

	m_ZipfileName = zipfileName;

	if( false == compressInThread )
	{
#ifdef DEBUG_ZIPPER
		LogMsg( LOG_INFO, "Zipper::zipFiles compress no thread\n" );
#endif // DEBUG_ZIPPER
		doZipOperation();
	}
	else
	{
#ifdef DEBUG_ZIPPER
		LogMsg( LOG_INFO, "Zipper::zipFiles compress in thread\n" );
#endif // DEBUG_ZIPPER
		m_ZipThread.abortThreadRun( true );
		m_ZipThread.startThread( (VX_THREAD_FUNCTION_T)DoZipThreadFunc, this, "ZIPTHREAD" );
	}
}

void Zipper::doZipOperation( void )
{
	setOperationInProgress();
	
	m_ToCaller.zipOperationBegan( m_ZipfileName.c_str() );
	deleteFile( m_ZipfileName.c_str() );

	m_uiZipFilesIndex = 0;
	Zip_ResetOptions();
	Zip_SetCompressionRate( m_iCompressionRate );
	Zip_SetFileToZipInto( m_ZipfileName.c_str() );
	Zip_SetFileCount( (int)m_ToZipFiles.size() );

	Zip_ExecuteZip( this );
	setOperationInProgress(false);

#ifdef DEBUG_ZIPPER
	LogMsg( LOG_INFO, "Zipper::doZipOperation complete filename %s\n", m_ZipfileName.c_str() );
#endif // DEBUG_ZIPPER
	m_ToCaller.zipOperationCompleted( m_ZipfileName.c_str() );
}


void Zipper::getNextFileToZip( void )
{
#ifdef DEBUG_ZIPPER
	LogMsg( LOG_INFO, "Zipper::getNextFileToZip \n" );
#endif // DEBUG_ZIPPER

	if( m_uiZipFilesIndex < m_ToZipFiles.size() )
	{
		ToZipFile& toZipFile = m_ToZipFiles[ m_uiZipFilesIndex ];
		setCurrentWorkingDirectory( toZipFile.m_FromDirectory.c_str() );

		Zip_SetFileNameInZip( toZipFile.m_ToZipFile.c_str() );
#ifdef DEBUG_ZIPPER
		LogMsg( LOG_INFO, "Zipper::getNextFileToZip work dir %s, filename %s\n", toZipFile.m_FromDirectory.c_str(), toZipFile.m_ToZipFile.c_str() );
#endif // DEBUG_ZIPPER
		m_uiZipFilesIndex++;
	}
}


void Zipper::unzipFile( const char * unzipfileName, const char * directoryToUnzipTo, bool unzipUsingThread  )
{
	if( NULL == unzipfileName )
	{
		LogMsg( LOG_ERROR, "Zipper::unzipFile: NULL file name\n" );
		return;
	}

	if( 0 == fileExists( unzipfileName  ) )
	{
		LogMsg( LOG_ERROR, "Zipper::unzipFile: File %s does not exist\n", unzipfileName );
		return;
	}

	m_UnzipfileName = unzipfileName;
	m_UnzipDirectory = directoryToUnzipTo;

	if( false == unzipUsingThread )
	{
		doUnzipOperation();
	}
	else
	{
		m_UnzipThread.abortThreadRun( true );
		m_UnzipThread.startThread( (VX_THREAD_FUNCTION_T)DoUnzipThreadFunc, this, "UNZIPTHREAD" );
	}
}

void Zipper::doUnzipOperation( void )
{
	setOperationInProgress();

	m_ToCaller.unzipOperationBegan( m_UnzipfileName.c_str() );
	Unzip_ResetOptions();
	Unzip_SetDirectoryToExtractTo( m_UnzipDirectory.c_str() );
	Unzip_SetZipFileName( m_UnzipfileName.c_str() );
#ifdef DEBUG_UNZIP
	LogMsg( LOG_ERROR, "Zipper::unzipFile: unzipping %s to %s\n", m_UnzipfileName.c_str(), m_UnzipDirectory.c_str() );
#endif // DEBUG_UNZIP

	Unzip_ExecuteExtraction();

	setOperationInProgress(false);

	m_ToCaller.unzipOperationCompleted( m_UnzipfileName.c_str() );
}

void Zipper::abortOperation( void )
{
	m_ZipThread.abortThreadRun( true );
	m_UnzipThread.abortThreadRun( true );
}


int Zipper::getUnzipProgress()
{
	return 50;
}

