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

#include "VxFileCopier.h"
#include "VxFileUtil.h"
#include "VxParse.h"
#include "VxFileIsTypeFunctions.h"
#include "VxDebug.h"

#include <string.h>

#ifndef TARGET_OS_WINDOWS
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h> 
#else
    #pragma warning( disable : 4244)
#endif


//============================================================================
RCODE VxFileCopier::copyDirectory(  std::string				 curPath,               // file or path to copy
                                    std::string&			 srcPath,			    // start path to search in
									std::string&             destPath,		        // destination directory
                                    std::vector<VxFileInfo>& aoFileList,			// return FileInfo in array
                                    bool					 bRecurse				// recurse subdirectories if true
									)
{
	m_bAbort = false;
    VxFileInfo oCurFileNode;
    RCODE rc = createDestDirectoryIfNeeded( curPath, srcPath, destPath );

#ifdef TARGET_OS_WINDOWS
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW sFindData;
    if( 0 != curPath.size ( ) )
    {
        if ( ( '\\' == curPath[ curPath.size() - 1 ] ) || ( '/' == curPath[ curPath.size() - 1 ] ) )
        {
			std::string newPath = curPath + "*.*";
            oCurFileNode.setFileName( newPath.c_str() );
        }
        else
        {
			std::string newPath = curPath + '/' + "*.*";
			oCurFileNode.setFileName( newPath.c_str() );
        }

        hFind = FindFirstFileW( Utf8ToWide( oCurFileNode.getFileName() ).c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE != hFind )
        {
            do
            {
                if ( ( '\\' == curPath[ curPath.size( ) - 1 ] ) || ( '/' == curPath[ curPath.size() - 1 ] ) )
                {
					std::string newFile = curPath + WideToUtf8( sFindData.cFileName );
					oCurFileNode.setFileName( newFile.c_str() );
                }
                else
                {
                    oCurFileNode.setFileName( curPath + '/' + WideToUtf8( sFindData.cFileName ) );
                }
                if ( ( sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
                {
				    if( bRecurse 
						&& ( false == VxFileUtil::isDotDotDirectory( sFindData.cFileName ) ) ) 
				    {

                        LogMsg( LOG_VERBOSE, "Recurse Directory %s", oCurFileNode.getFileName().c_str() );

                        rc = createDestDirectoryIfNeeded( oCurFileNode.getFileName(), srcPath, destPath );
                        copyDirectory( oCurFileNode.getFileName(),
                                        srcPath,
                                        destPath,
										aoFileList,
										bRecurse
										);
				    }
                }
			    else
			    {
                    rc = copyFileToDestDir( oCurFileNode.getFileName(), srcPath, destPath );
                    if( !rc )
                    {
                        LogMsg( LOG_VERBOSE, "File Copied %s", oCurFileNode.getFileName().c_str() );
                        oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                        oCurFileNode.setFileLength( ( (uint64_t)sFindData.nFileSizeHigh << 32 ) | sFindData.nFileSizeLow );
                        aoFileList.push_back( oCurFileNode );
                    }
                    else
                    {
                        LogMsg( LOG_ERROR, "ERROR %d copying file %s", rc, oCurFileNode.getFileName().c_str() );
                    }
			    }
            }
            while( ( false == m_bAbort ) &&
                   ( FindNextFileW( hFind,
                                   &sFindData ) ) );
            if( true == m_bAbort )
            {
                rc = ERROR_OPERATION_ABORTED;
            }
            FindClose ( hFind );
        }
    }
#else 
	// linux version of find files
	// find the files in the directory
	DIR *pDir;
	struct dirent *pFileEnt;
    if( VxFileUtil::directoryExists( curPath.c_str() ) )
	{
		//LogMsg( LOG_INFO, "FindFilesByExtension: directory %s exists.. opening dir\n", csPath.c_str() );
		//ok directory exists!
        if( !( nullptr == ( pDir = opendir( curPath.c_str() ) ) ) )
		{
			//pDir is open
            while( nullptr != ( pFileEnt = readdir(pDir) ) )
			{
				LogMsg( LOG_INFO, "FindFilesByExtension: found file %s\n", pFileEnt->d_name );
                if ( '/' == curPath[ curPath.size( ) - 1 ] )
				{
                    oCurFileNode.setFileName( curPath + pFileEnt->d_name );
				}
				else
				{
                    oCurFileNode.setFileName( curPath + '/' + pFileEnt->d_name );
				}
				struct stat oStat;
                if( 0 != stat( oCurFileNode.getFileName().c_str(), &oStat ) )
				{
					///ERROR how do we handle
                    LogMsg( LOG_INFO, "FindFilesByExtension: Unable to stat file %s\n", oCurFileNode.getFileName().c_str() );
					continue;
				}

				if( pFileEnt->d_type == DT_DIR )
				{
					if( bRecurse  
						&& ( ! VxFileUtil::isDotDotDirectory( pFileEnt->d_name ) ) )
					{
                        copyDirectory( oCurFileNode.getFileName(),
                                       srcPath,
                                       destPath,
                                       aoFileList,
                                       bRecurse								//recurse subdirectories if true
                                        );
					}
				}
				else
				{
					// its a file
                    rc = copyFileToDestDir( oCurFileNode.getFileName(), srcPath, destPath );
                    if( !rc )
                    {
                        LogMsg( LOG_VERBOSE, "File Copied %s", oCurFileNode.getFileName().c_str() );
                        oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                        oCurFileNode.setFileLength( VxFileUtil::getFileLen( oCurFileNode.getFileName().c_str() ) );
                        aoFileList.push_back( oCurFileNode );
                    }
                    else
                    {
                        LogMsg( LOG_ERROR, "ERROR %d copying file %s", rc, oCurFileNode.getFileName().c_str() );
                    }
                }
			}

			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
            LogMsg( LOG_INFO, "VxListSubDirectories: could not open directory %s \n", curPath.c_str() );
		}
	}
	else
	{
        LogMsg( LOG_INFO, "VxListSubDirectories: directory %s does not exist \n", curPath.c_str() );
	}

	return 0;
#endif // TARGET_OS_WINDOWS
    return rc;
}

//============================================================================
RCODE VxFileCopier::createDestDirectoryIfNeeded( std::string&		curPath,
                                                 std::string&		srcPath,
                                                 std::string&		destPath )
{
    std::string destDirectory;
    RCODE rc = substitutePath( curPath, srcPath, destPath, destDirectory );
    if( rc )
    {
        LogMsg( LOG_ERROR, "Invalid Path %s", curPath.c_str() );
        return EACCES;
    }

    VxFileUtil::assureTrailingDirectorySlash( destDirectory );
    if( !VxFileUtil::directoryExists( destDirectory.c_str() ) )
    {
        VxFileUtil::makeDirectory( destDirectory );
        if( !VxFileUtil::directoryExists( destDirectory.c_str() ) )
        {
            LogMsg( LOG_ERROR, "Could Not create directory %s", destDirectory.c_str() );
            return EACCES;
        }
    }

    return 0;
}


//============================================================================
RCODE VxFileCopier::substitutePath( std::string&		curPath,
                                     std::string&		srcPath,
                                     std::string&		destPath,
                                     std::string&		resultPath )
{
    resultPath = curPath;
    if( resultPath.length() >= srcPath.length() )
    {
        VxFileUtil::makeForwardSlashPath( resultPath );
        resultPath.replace( 0, srcPath.length(), destPath );
        return 0;
    }

    return E2BIG;
}

//============================================================================
RCODE VxFileCopier::copyFileToDestDir( std::string& srcFileName, std::string& srcPath, std::string& destPath )
{
    std::string destFileName;
    RCODE rc = substitutePath( srcFileName, srcPath, destPath, destFileName );
    if( !rc )
    {
        rc = VxFileUtil::copyFile( srcFileName.c_str(), destFileName.c_str() );
    }

    return rc;
}
