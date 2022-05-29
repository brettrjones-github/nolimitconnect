//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "SharedFilesFinder.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxDebug.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef TARGET_OS_WINDOWS
    #pragma warning( disable : 4244)
#else
    //#include <sys/statvfs.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
    #define FILE_ATTRIBUTE_DIRECTORY 10
#endif

//============================================================================
RCODE SharedFileFinder::FindFilesByExtension(	std::string					csPath,				//start path to search in
												std::vector<std::string>	&acsExtensionList,	//Extensions
												std::vector<SharedFileInfo> &aoFileList,		//return FileInfo in array
												bool						bRecurse,			//recurse subdirectories if true
												bool						bUseFilterListToExclude	//if true don't return files matching filter else return files that do
												)
{
	m_bAbort = false;
    SharedFileInfo oCurFileNode;
    RCODE rc = 0;
#ifdef TARGET_OS_WINDOWS
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW sFindData;
    if( 0 != csPath.size ( ) )
    {
        if ( '\\' == csPath[ csPath.size() - 1 ] )
        {
            oCurFileNode = csPath + "*.*";
        }
        else
        {
            oCurFileNode = csPath + '\\' + "*.*";
        }

        hFind = FindFirstFileW( Utf8ToWide( oCurFileNode.getFileName() ).c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE != hFind )
        {
            do
            {
                if ( '\\' == csPath[ csPath.size( ) - 1 ] )
                {
                    oCurFileNode.setFileName( csPath + WideToUtf8( sFindData.cFileName ) );
                }
                else
                {
                    oCurFileNode.setFileName(  csPath + '\\' + WideToUtf8( sFindData.cFileName ) );
                }
                if ( ( 0 != (sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) )
                {
				    if ( ( bRecurse ) 
						&& ( false == VxFileUtil::isDotDotDirectory( sFindData.cFileName ) ) ) //recurse or not
				    {
					    FindFilesByExtension(	oCurFileNode.getFileName(),
												acsExtensionList,
												aoFileList,
												bRecurse,								//recurse subdirectories if TRUE
												bUseFilterListToExclude
												);
				    }
                }
			    else
			    {
					//include or exclude on the give list of extensions
					if( bUseFilterListToExclude )
					{
						if( ! HasSameExtension( oCurFileNode.getFileName(), acsExtensionList ) )
						{
							//oCurFileNode.m_u32Attributes = sFindData.dwFileAttributes;
							oCurFileNode.m_s64FileLen = sFindData.nFileSizeHigh;
							oCurFileNode.m_s64FileLen = oCurFileNode.m_s64FileLen << 32;
							oCurFileNode.m_s64FileLen += sFindData.nFileSizeLow;
    						aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
						if( HasSameExtension( oCurFileNode.getFileName(), acsExtensionList ) )
						{
							//oCurFileNode.m_u32Attributes = sFindData.dwFileAttributes;
							oCurFileNode.m_s64FileLen = sFindData.nFileSizeHigh;
							oCurFileNode.m_s64FileLen = oCurFileNode.m_s64FileLen << 32;
							oCurFileNode.m_s64FileLen += sFindData.nFileSizeLow;
							aoFileList.push_back( oCurFileNode );
						}
					}
			    }
            }
            while( ( false == m_bAbort ) &&
                    ( FindNextFileW ( hFind,
                                   &sFindData ) ) );
            if( true == m_bAbort )
            {
                rc = ERROR_OPERATION_ABORTED;
            }

            FindClose( hFind );
        }
    }
#else 
    char as8SrcDir[ VX_MAX_PATH * 2 ];
    strcpy( as8SrcDir, csPath.c_str() );
	// linux version of find files
	// find the files in the directory
    char as8SrcFile[ VX_MAX_PATH * 2 ];
    strcpy( as8SrcFile, csPath.c_str() );

	DIR *pDir;
	struct dirent *pFileEnt;
	if( VxFileUtil::directoryExists( as8SrcDir ) )
	{
		//LogMsg( LOG_INFO, "VxListSubDirectories: directory %s exists.. opening dir\n", as8SrcDir );
		//ok directory exists!
		if(!(NULL == (pDir = opendir(as8SrcDir))))
		{
			//pDir is open
			while( 0 != (pFileEnt = readdir(pDir)))
			{
				LogMsg( LOG_INFO, "VxListSubDirectories: found file %s\n", pFileEnt->d_name );
				//got a file or directory
				if( '.' == pFileEnt->d_name[ 0 ] )//||
			//			(DT_LNK == pFileEnt->d_type )  ||
			//			(DT_UNKNOWN == pFileEnt->d_type ) )
				{
					// skip . and ..
					LogMsg( LOG_INFO, "VxListSubDirectories: skipping file %s\n", pFileEnt->d_name );
					continue;
				}
				// valid directory entry
				// make source file name
				strcpy( as8SrcFile, as8SrcDir );
				if( '/' != as8SrcFile[ strlen( as8SrcFile ) - 1 ] )
				{
					strcat( as8SrcFile, "/" );
				}
				strcat( as8SrcFile, pFileEnt->d_name );
				oCurFileNode = as8SrcFile;

				#ifdef TARGET_OS_WINDOWS
					struct __stat64 oStat;
					// Get data associated with the file
					int statResult = _stat64( as8SrcFile, &oStat );
				#else
					struct stat64 oStat;
					// Get data associated with the file
					int statResult = stat64( as8SrcFile, &oStat );
				#endif //TARGET_OS_WINDOWS
				if( 0 != statResult )
				{
					// ERROR how do we handle
					LogMsg( LOG_INFO, "VxListSubDirectories: Unable to stat file %s\n", as8SrcFile );
					continue;
				}
				if( pFileEnt->d_type == DT_DIR )
				{
					// its a directory
					LogMsg( LOG_INFO, "VxListSubDirectories: file %s is directory\n", as8SrcFile );
					char * pName = new char[ strlen( pFileEnt->d_name ) + 1 ];
					vx_assert( pName );
					strcpy( pName, pFileEnt->d_name );
				    if ( ( bRecurse ) &&
						( 0 != ( strcmp ( ".", pName ) ) ) &&
				         ( 0 != ( strcmp ( "..", pName ) ) ) ) //recurse or not
				    {
					    FindFilesByExtension(	oCurFileNode.getLocalFileName(),
												acsExtensionList,
												aoFileList,
												bRecurse,								//recurse subdirectories if TRUE
												bUseFilterListToExclude
												);
				    }

				    delete[] pName;
				}
				else
				{
					// its a file
					oCurFileNode.m_s64FileLen = oStat.st_size;
					aoFileList.push_back( oCurFileNode );
					continue;
				}
			}
			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
			LogMsg( LOG_INFO, "VxListSubDirectories: could not open directory %s \n", as8SrcDir );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "VxListSubDirectories: directory %s does not exist \n", as8SrcDir );
	}
	return 0;
#endif // TARGET_OS_WINDOWS
    return rc;
}
//============================================================================
bool SharedFileFinder::HasSameExtension(	std::string csCurrentNode,
										std::vector<std::string> &acsExtensionList )
{
    std::string csFilterExt;
	std::string csNodeExt;
    //size_t iLen = csCurrentNode.size ( );
	//LogMsg( LOG_INFO,  "TotalLen %d\n", iLen );
    int iExtIdx = StdStringReverseFind( csCurrentNode, '.' );
	//LogMsg( LOG_INFO,  "Index %d\n", iExtIdx );
    if( 0 > iExtIdx )
	{
		// could not find a .
		return false;
	}

	csNodeExt = StdStringRight( csCurrentNode, ((int)csCurrentNode.size ( ) - StdStringReverseFind( csCurrentNode, '.' )) );
	StdStringMakeUpper( csNodeExt );

	size_t iListCnt = acsExtensionList.size();
	for( unsigned int iIdx = 0; iIdx < iListCnt; iIdx++ )
	{
        csFilterExt = acsExtensionList[ iIdx ];
		StdStringMakeUpper( csFilterExt );

        if( csFilterExt == csNodeExt )
        {
            return true;
        }
    }
    return false;
}

//============================================================================
RCODE SharedFileFinder::FindFilesByName(	VxThread *					poThread,
											std::string					csPath,					//start path to search in
											std::vector<std::string>	&acsWildNameList,		//Name match strings with wildcards
											std::vector<SharedFileInfo>	&aoFileList,			//return FileInfo in array
											bool						bRecurse,				//recurse subdirectories if TRUE
											bool						bUseFilterListToExclude	//if TRUE dont return files matching filter else return files that do
											)
{
    SharedFileInfo oCurFileNode;
    RCODE rc = 0;

#ifdef TARGET_OS_WINDOWS
	// windows version of find files
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW sFindData;
    if( 0 != csPath.size ( ) )
    {
        if( ('\\' == csPath[ csPath.size() - 1 ] ) ||
			('/' == csPath[ csPath.size() - 1 ] ) )

        {
            oCurFileNode.setFileName( csPath + "*.*" );
        }
        else
        {
            oCurFileNode.setFileName(  csPath + "/*.*" );
        }
        hFind = FindFirstFileW( Utf8ToWide( oCurFileNode.getFileName() ).c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE != hFind )
        {
            do
            {
				if( poThread->isAborted() )
				{
					rc = ERROR_OPERATION_ABORTED;
					FindClose ( hFind );
					return rc;
				}
                if( ( '\\' == csPath[ csPath.size( ) - 1 ] ) ||
					( '/' == csPath[ csPath.size( ) - 1 ] ) )
                {
                    oCurFileNode = csPath + WideToUtf8( sFindData.cFileName );
                }
                else
                {
                    oCurFileNode = csPath + '/' + WideToUtf8( sFindData.cFileName );
                }
                if ( ( 0 != (FILE_ATTRIBUTE_DIRECTORY & sFindData.dwFileAttributes ) ) )
                {
					if( false == VxFileUtil::isDotDotDirectory( sFindData.cFileName ) )
				    {
						oCurFileNode.m_u32Attributes = sFindData.dwFileAttributes;
						oCurFileNode.m_s64FileLen = sFindData.nFileSizeHigh;
						oCurFileNode.m_s64FileLen = oCurFileNode.m_s64FileLen << 32;
						oCurFileNode.m_s64FileLen += sFindData.nFileSizeLow;
						VxFileUtil::makeForwardSlashPath( oCurFileNode.getFileName() );
						aoFileList.push_back( oCurFileNode );

						if( bRecurse )
						{
							FindFilesByName( 
										poThread,
										oCurFileNode.getFileName(),
										acsWildNameList,
										aoFileList,
										bRecurse,								//recurse subdirectories if TRUE
										bUseFilterListToExclude
										);
						}
				    }
                }
			    else
			    {
					//include or exclude on the given list of wild Name Match Strings
					if( bUseFilterListToExclude )
					{
						if( ! HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
							oCurFileNode.m_u32Attributes = sFindData.dwFileAttributes;
							oCurFileNode.m_s64FileLen = sFindData.nFileSizeHigh;
							oCurFileNode.m_s64FileLen = oCurFileNode.m_s64FileLen << 32;
							oCurFileNode.m_s64FileLen += sFindData.nFileSizeLow;
							VxFileUtil::makeForwardSlashPath( oCurFileNode.getFileName() );
    						aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
						if( HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
							oCurFileNode.m_u32Attributes = sFindData.dwFileAttributes;
							oCurFileNode.m_s64FileLen = sFindData.nFileSizeHigh;
							oCurFileNode.m_s64FileLen = oCurFileNode.m_s64FileLen << 32;
							oCurFileNode.m_s64FileLen += sFindData.nFileSizeLow;
							VxFileUtil::makeForwardSlashPath( oCurFileNode.getFileName() );
							aoFileList.push_back( oCurFileNode );
						}
					}
			    }
            }
            while( FindNextFileW( hFind, &sFindData ) );
            if( poThread->isAborted() )
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
    char as8SrcDir[ VX_MAX_PATH * 2 ];
    strcpy( as8SrcDir, csPath.c_str() );
	// linux version of find files
	// find the files in the directory
    char as8SrcFile[ VX_MAX_PATH * 2 ];
    strcpy( as8SrcFile, csPath.c_str() );

	struct dirent *pFileEnt;
	if( VxFileUtil::directoryExists( as8SrcDir ) )
	{
		//LogMsg( LOG_ERROR, "VxListFiles: directory %s exists.. opening dir\n", as8SrcDir );
		//ok directory exists!
		if(!(NULL == (pDir = opendir(as8SrcDir))))
		{
			//pDir is open
			while( 0 != (pFileEnt = readdir(pDir)))
			{
				//LogMsg( LOG_ERROR, "VxListFiles: found file %s\n", pFileEnt->d_name );
				//got a file or directory
				if( VxFileUtil::isDotDotDirectory( pFileEnt->d_name  ) )
				{
					// skip . and ..
					//LogMsg( LOG_ERROR, "VxListFiles: skipping file %s\n", pFileEnt->d_name );
					continue;
				}

				//make path and file name
				strcpy( as8SrcFile, as8SrcDir );
				if( '/' != as8SrcFile[ strlen( as8SrcFile ) - 1 ] )
				{
					strcat( as8SrcFile, "/" );
				}
				strcat( as8SrcFile, pFileEnt->d_name );
				oCurFileNode = as8SrcFile;

				// valid directory entry
				#ifdef TARGET_OS_WINDOWS
					struct __stat64 oStat;
					// Get data associated with the file
					int statResult = _stat64( as8SrcFile, &oStat );
				#else
					struct stat64 oStat;
					// Get data associated with the file
					int statResult = stat64( as8SrcFile, &oStat );
				#endif //TARGET_OS_WINDOWS
				if( 0 != statResult )
				{
					///ERROR how do we handle
					LogMsg( LOG_ERROR, "Unable to stat file %s\n", as8SrcFile );
					continue;
				}
				if( S_IFDIR & oStat.st_mode )
				{
					// its a directory
					LogMsg( LOG_ERROR, "file %s is directory\n", as8SrcFile );
					oCurFileNode.m_u32Attributes = FILE_ATTRIBUTE_DIRECTORY;
					oCurFileNode.m_s64FileLen = 0;
					aoFileList.push_back( oCurFileNode );
					continue;
				}
				else
				{
					// its a file
					oCurFileNode.m_s64FileLen = oStat.st_size;
					aoFileList.push_back( oCurFileNode );
				}

			}
			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
			LogMsg( LOG_ERROR, "VxListFiles: could not open directory %s \n", as8SrcDir );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "VxListFiles: directory %s does not exist \n", as8SrcDir );
	}
	return 0;

#endif // TARGET_OS_WINDOWS
    return rc;
}

//============================================================================
bool SharedFileFinder::HasMatchingName( std::string csCurrentNode,
									std::vector<std::string> &acsWildNameList )
{
	int iCnt = (int)acsWildNameList.size();
	for( int i = 0; i < iCnt; i++ )
	{
		if( VxFileUtil::fileNameWildMatch( csCurrentNode.c_str(), acsWildNameList[ i ].c_str() ) )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
RCODE FindSharedFilesByExtension(std::string csPath,				//start path to search in
                std::vector<std::string> &acsExtensionList,//Extensions ( file extentions )
                std::vector<SharedFileInfo> &aoFileList,//return FileInfo in array
				bool bRecurse,						//recurse subdirectories if TRUE
				bool bUseFilterListToExclude		//if TRUE dont return files matching filter else return files that do
				)
{
	SharedFileFinder oFileFinder;
	return oFileFinder.FindFilesByExtension( csPath,//start path to search in
                acsExtensionList,				//Extensions of files to find
                aoFileList,						//return FileInfo in array
				bRecurse,						//recurse subdirectories if TRUE
				bUseFilterListToExclude			//if TRUE dont return files matching filter else return files that do
				);
}
RCODE FindSharedFilesByName(	VxThread *						poThread,
								std::string csPath,				//start path to search in
								std::vector<std::string> &acsWildNameList,//Extensions ( file extentions )
								std::vector<SharedFileInfo> &aoFileList,//return FileInfo in array
								bool bRecurse,						//recurse subdirectories if TRUE
								bool bUseFilterListToExclude		//if TRUE dont return files matching filter else return files that do
								)
{
	SharedFileFinder oFileFinder;
	return oFileFinder.FindFilesByName( 
				poThread,
				csPath,//start path to search in
                acsWildNameList,				//Extensions of files to find
                aoFileList,						//return FileInfo in array
				bRecurse,						//recurse subdirectories if TRUE
				bUseFilterListToExclude			//if TRUE dont return files matching filter else return files that do
				);
}

