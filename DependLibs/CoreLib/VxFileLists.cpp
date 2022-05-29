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

#include "VxFileLists.h"
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
RCODE VxFileFinder::FindFilesByExtension(	std::string				csPath,					//start path to search in
											std::vector<std::string> &acsExtensionList,		//Extensions
											std::vector<VxFileInfo> &aoFileList,			//return FileInfo in array
											bool					bRecurse,				//recurse subdirectories if true
											bool					bUseFilterListToExclude	//if true dont return files matching filter else return files that do
											)
{
	m_bAbort = false;
    VxFileInfo oCurFileNode;
    RCODE rc = 0;
#ifdef TARGET_OS_WINDOWS
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW sFindData;
    if( 0 != csPath.size ( ) )
    {
        if ( '\\' == csPath[ csPath.size() - 1 ] )
        {
			std::string newPath = csPath + "*.*";
            oCurFileNode.setFileName( newPath.c_str() );
        }
        else
        {
			std::string newPath = csPath  + '\\' + "*.*";
			oCurFileNode.setFileName( newPath.c_str() );
        }

        hFind = FindFirstFileW( Utf8ToWide( oCurFileNode.getFileName() ).c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE != hFind )
        {
            do
            {
                if ( '\\' == csPath[ csPath.size( ) - 1 ] )
                {
					std::string newFile = csPath + WideToUtf8( sFindData.cFileName );
					oCurFileNode.setFileName( newFile.c_str() );
                }
                else
                {
                    oCurFileNode.setFileName( csPath + '\\' + WideToUtf8( sFindData.cFileName ) );
                }
                if ( ( 0 != (sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) )
                {
				    if( bRecurse 
						&& ( false == VxFileUtil::isDotDotDirectory( sFindData.cFileName ) ) ) 
				    {
					    FindFilesByExtension(   oCurFileNode.getFileName(),
												acsExtensionList,
												aoFileList,
												bRecurse,								//recurse subdirectories if true
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
							oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
							oCurFileNode.setFileLength( ((uint64_t)sFindData.nFileSizeHigh << 32) | sFindData.nFileSizeLow );
    						aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
						if( HasSameExtension( oCurFileNode.getFileName(), acsExtensionList ) )
						{
							oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
							oCurFileNode.setFileLength( ((uint64_t)sFindData.nFileSizeHigh << 32) | sFindData.nFileSizeLow );
							aoFileList.push_back( oCurFileNode );
						}
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
    if( VxFileUtil::directoryExists( csPath.c_str() ) )
	{
		//LogMsg( LOG_INFO, "FindFilesByExtension: directory %s exists.. opening dir\n", csPath.c_str() );
		//ok directory exists!
		if(!(NULL == (pDir = opendir( csPath.c_str() ))))
		{
			//pDir is open
			while( 0 != ( pFileEnt = readdir(pDir) ))
			{
				LogMsg( LOG_INFO, "FindFilesByExtension: found file %s\n", pFileEnt->d_name );
				if ( '/' == csPath[ csPath.size( ) - 1 ] )
				{
                    oCurFileNode.setFileName( csPath + pFileEnt->d_name );
				}
				else
				{
                    oCurFileNode.setFileName( csPath + '/' + pFileEnt->d_name );
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
                        FindFilesByExtension(   oCurFileNode.getFileName(),
                                                acsExtensionList,
                                                aoFileList,
                                                bRecurse,					// recurse subdirectories if true
                                                bUseFilterListToExclude
                                                );
					}
				}
				else
				{
					// its a file
					//include or exclude on the give list of extensions
					if( bUseFilterListToExclude )
					{
                        if( ! HasSameExtension( oCurFileNode.getFileName(), acsExtensionList ) )
						{
                            oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                            oCurFileNode.setFileLength( oStat.st_size );
							aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
                        if( HasSameExtension( oCurFileNode.getFileName(), acsExtensionList ) )
						{
                            oCurFileNode.setFileType(  VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                            oCurFileNode.setFileLength( oStat.st_size );
							aoFileList.push_back( oCurFileNode );
						}
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
			LogMsg( LOG_INFO, "VxListSubDirectories: could not open directory %s \n", csPath.c_str() );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "VxListSubDirectories: directory %s does not exist \n", csPath.c_str() );
	}

	return 0;
#endif // TARGET_OS_WINDOWS
    return rc;
}

//============================================================================
bool VxFileFinder::HasSameExtension(	std::string csCurrentNode,
										std::vector<std::string> &acsExtensionList )
{
    std::string csFilterExt;
	std::string csNodeExt;
	//LogMsg( LOG_INFO,  "TotalLen %d\n", iLen );
    int iExtIdx = StdStringReverseFind( csCurrentNode, '.' );
	//LogMsg( LOG_INFO,  "Index %d\n", iExtIdx );
	if( iExtIdx < 0 )
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
RCODE VxFileFinder::FindFilesByName(	std::string csPath,	//start path to search in
										std::vector<std::string> &acsWildNameList,	//Name match strings with wildcards
										std::vector<VxFileInfo> &aoFileList,//return FileInfo in array
										bool bRecurse,								//recurse subdirectories if true
										bool bUseFilterListToExclude				//if true dont return files matching filter else return files that do
										)
{
	m_bAbort = false;
    VxFileInfo oCurFileNode;
    RCODE rc = 0;

#ifdef TARGET_OS_WINDOWS
	// windows version of find files
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW sFindData;
    if( 0 != csPath.size ( ) )
    {
        if ( '\\' == csPath[ csPath.size() - 1 ] )
        {
            oCurFileNode.setFileName( csPath + "*.*" );
        }
        else
        {
            oCurFileNode.setFileName(  csPath + "\\*.*" );
        }
        hFind = FindFirstFileW( Utf8ToWide( oCurFileNode.getFileName() ).c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE != hFind )
        {
            do
            {
                if( '\\' == csPath[ csPath.size( ) - 1 ] )
                {
                    oCurFileNode.setFileName(  csPath + WideToUtf8( sFindData.cFileName ) );
                }
                else
                {
                    oCurFileNode.setFileName( csPath + '\\' + WideToUtf8( sFindData.cFileName ) );
                }
                if ( 0 != (sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
				    if ( ( bRecurse ) 
						&& ( false == VxFileUtil::isDotDotDirectory( sFindData.cFileName ) ) )
				    {
					    FindFilesByName(	oCurFileNode.getFileName(),
											acsWildNameList,
											aoFileList,
											bRecurse,								//recurse subdirectories if true
											bUseFilterListToExclude
											);
				    }
                }
			    else
			    {
					//include or exclude on the given list of wild Name Match Strings
					if( bUseFilterListToExclude )
					{
						if( ! HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
							oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
							oCurFileNode.setFileLength( ((uint64_t)sFindData.nFileSizeHigh << 32) | sFindData.nFileSizeLow );
                            VxFileUtil::makeForwardSlashPath( oCurFileNode.getFileName() );
    						aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
						if( HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
							oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
							oCurFileNode.setFileLength( ((uint64_t)sFindData.nFileSizeHigh << 32) | sFindData.nFileSizeLow );
                            VxFileUtil::makeForwardSlashPath( oCurFileNode.getFileName() );
							aoFileList.push_back( oCurFileNode );
						}
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
	char as8SrcDir[ VX_MAX_PATH * 2 ];
	strcpy( as8SrcDir, csPath.c_str() );

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
				//LogMsg( LOG_INFO, "FindFilesByExtension: found file %s\n", pFileEnt->d_name );
				if ( '/' == csPath[ csPath.size( ) - 1 ] )
				{
                    oCurFileNode.setFileName( csPath + pFileEnt->d_name );
				}
				else
				{
                    oCurFileNode.setFileName( csPath + '/' + pFileEnt->d_name );
				}
				struct stat oStat;
                if( 0 != stat( oCurFileNode.getFileName().c_str(), &oStat ) )
				{
					///ERROR how do we handle
                    LogMsg( LOG_INFO, "FindFilesByExtension: Unable to stat file %s\n", oCurFileNode.getFileName().c_str() );
					continue;
				}

				if( S_IFDIR & oStat.st_mode  )
				{
					if ( ( bRecurse ) &&
                        ( !VxFileUtil::isDotDotDirectory( pFileEnt->d_name ) ) )
					{
                        FindFilesByName(    oCurFileNode.getFileName(),
                                            acsWildNameList,
                                            aoFileList,
                                            bRecurse,								//recurse subdirectories if true
                                            bUseFilterListToExclude
                                            );
					}
				}
				else
				{
					//include or exclude on the given list of wild Name Match Strings
					if( bUseFilterListToExclude )
					{
                        if( ! HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
                            oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                            oCurFileNode.setFileLength( oStat.st_size );
							aoFileList.push_back( oCurFileNode );
						}
					}
					else
					{
                        if( HasMatchingName( oCurFileNode.getFileName(), acsWildNameList ) )
						{
                            oCurFileNode.setFileType( VxFileUtil::fileExtensionToFileTypeFlag( oCurFileNode.getFileName().c_str() ) );
                            oCurFileNode.setFileLength( oStat.st_size );
							aoFileList.push_back( oCurFileNode );
						}
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
bool VxFileFinder::HasMatchingName( std::string csCurrentNode,
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
RCODE VxFindFilesByExtension(std::string csPath,				//start path to search in
                std::vector<std::string> &acsExtensionList,//Extensions ( file extentions )
                std::vector<VxFileInfo> &aoFileList,//return FileInfo in array
				bool bRecurse,						//recurse subdirectories if true
				bool bUseFilterListToExclude		//if true dont return files matching filter else return files that do
				)
{
	VxFileFinder oFileFinder;
	return oFileFinder.FindFilesByExtension( csPath,//start path to search in
                acsExtensionList,				//Extensions of files to find
                aoFileList,						//return FileInfo in array
				bRecurse,						//recurse subdirectories if true
				bUseFilterListToExclude			//if true dont return files matching filter else return files that do
				);
}
RCODE VxFindFilesByName(std::string csPath,				//start path to search in
                std::vector<std::string> &acsWildNameList,//Extensions ( file extentions )
                std::vector<VxFileInfo> &aoFileList,//return FileInfo in array
				bool bRecurse,						//recurse subdirectories if true
				bool bUseFilterListToExclude		//if true dont return files matching filter else return files that do
				)
{
	VxFileFinder oFileFinder;
	return oFileFinder.FindFilesByName( csPath,//start path to search in
                acsWildNameList,				//Extensions of files to find
                aoFileList,						//return FileInfo in array
				bRecurse,						//recurse subdirectories if true
				bUseFilterListToExclude			//if true dont return files matching filter else return files that do
				);
}

