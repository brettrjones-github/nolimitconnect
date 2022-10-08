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

#include "VxCrypto.h"
#include "VxFileCopier.h"
#include "VxFileUtil.h"
#include "VxFileIsTypeFunctions.h"
#include "VxGlobals.h"
#include "VxParse.h"
#include "VxDebug.h"
#include "VxUrl.h"

#include "SHA1.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#ifdef TARGET_OS_WINDOWS
	#include "shlobj.h" // for VxGetMyDocumentsDir
	#include <direct.h>
#else
	#include <dirent.h> // for searching directories
	#include <ctype.h>
	#include <unistd.h> 
	#include <sys/vfs.h>    
	#include <sys/statfs.h> 
#endif

namespace
{
#define PATH_SEP_CHAR   '/'
#define PATH_ALT_SEP_CHAR   '\\'

    char GetPathSeperator( void )
    {
#ifdef TARGET_OS_WINDOWS
        return PATH_ALT_SEP_CHAR;
#else
        return PATH_SEP_CHAR;
#endif // TARGET_OS_WINDOWS
    }
}
//============================================================================
size_t FindLastPathSeperator( std::string& path )
{
    return path.find_last_of( GetPathSeperator() );
}

//============================================================================
bool AddExtraLongPathPrefix( std::string& path )
{
    const char* str = path.c_str();
    if( path.length() < 4 || str[ 0 ] != '\\' || str[ 1 ] != '\\' || str[ 3 ] != '\\' || str[ 2 ] != '?' )
    {
        path.insert( 0, "\\\\?\\" );
        return true;
    }

    return false;
}

//============================================================================
bool RemoveExtraLongPathPrefix( std::string& path )
{
    const char* str = path.c_str();
    if( path.length() >= 4 && str[ 0 ] == '\\' && str[ 1 ] == '\\' && str[ 3 ] == '\\' && str[ 2 ] == '?' )
    {
        path.erase( 0, 4 );
        return true;
    }

    return false;
}

//============================================================================
#ifdef TARGET_OS_WINDOWS
std::string WindowsRelativeToAbsolutePath( std::string& path )
{
    std::string resultPath = path;
    char pathBuf[ MAX_PATH * 2];
    if( path.length() && path.length() <= MAX_PATH )
    {
        strcpy( pathBuf, path.c_str() );
        if( WindowsRelativeToAbsolutePath( pathBuf, sizeof( pathBuf ) ) )
        {
            resultPath = pathBuf;
        }
    }

    return resultPath;
}

extern "C" bool WindowsRelativeToAbsolutePath( char * pathBuf, int bufLen )
{
    if( !pathBuf || bufLen < 1 )
    {
        LogMsg( LOG_ERROR, "WindowsRelativeToAbsolutePath invalid path" );
        return false;
    }

    std::string fullPath = pathBuf;
    if( fullPath.size() < 3 )
    {
        // not long enough to have relative path but is not a failure
        return true;
    }

    std::string justPath = fullPath;
    std::string fileName;
    char pathSeperator = GetPathSeperator();
    bool removedRelative = false;
    bool endsWithSlash = false;

    if( !VxFileUtil::directoryExists( justPath.c_str() ) )
    {
        // remove file name
        size_t lastSep = FindLastPathSeperator( fullPath );
        if( lastSep != std::string::npos )
        {
             pathSeperator = fullPath.at( lastSep );

            if( lastSep == fullPath.size() - 1 )
            {
                endsWithSlash = true;
                // remove slash at end of path
                justPath = fullPath.substr( 0, lastSep );
            }
           
            justPath = fullPath.substr( 0, lastSep );
            fileName = fullPath.substr( lastSep + 1, fullPath.size() );
        }
    }

    bool hasRelativePath = true;
    for( int i = 0; hasRelativePath && i < 4; i++ )
    {
        hasRelativePath = false;
        // make into full path by removing relative ..
        if( justPath.find( ".." ) != std::string::npos )
        {
            // expand relative path to full path
            AddExtraLongPathPrefix( justPath );
            const unsigned int bufSize = GetFullPathNameA( justPath.c_str(), 0, NULL, NULL );
            if( bufSize != 0 )
            {
                char * buf = new char[ bufSize ];
                if( GetFullPathNameA( justPath.c_str(), bufSize, buf, NULL ) <= bufSize - 1 )
                {
                    std::string absolutePath( buf );
                    RemoveExtraLongPathPrefix( absolutePath );
                    justPath = absolutePath;
                    hasRelativePath = true;
                    removedRelative = true;
                }

                delete[] buf;
            }
        }
    }

    if( removedRelative )
    {
        fullPath = justPath;
        fullPath += pathSeperator;
        if( fileName.size() )
        {
            // add back the file name to path
            fullPath += fileName;
        }

        if( endsWithSlash )
        {
            fullPath += pathSeperator;
        }

        if( fullPath.size() >= bufLen )
        {
            // too long for buffer
            LogMsg( LOG_ERROR, "WindowsRelativeToAbsolutePath Buffer too short" );
            return false;
        }

        strcpy( pathBuf, fullPath.c_str() );
    }

    return true;
}
#endif // TARGET_OS_WINDOWS

//========================================================================
std::string VxFileUtil::makeKodiPath( const char* path )
{
	std::string kodiPath = path;
#if defined(TARGET_OS_WINDOWS)
	makeBackwardSlashPath( kodiPath );
#else
	makeForwardSlashPath( kodiPath );
#endif // defined(TARGET_OS_WINDOWS)

	removeTrailingDirectorySlash( kodiPath );

	return kodiPath;
}

//============================================================================
bool VxFileUtil::isDotDotDirectory( const char* fileName )
{
	int nameLen = strlen( fileName );
	if( ( ( 1 == nameLen ) && ( '.' == fileName[0] ) )
		|| ( ( 2 == nameLen ) && ( '.' == fileName[0] ) && ( '.' == fileName[1] ) ) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool VxFileUtil::isDotDotDirectory( const wchar_t * fileName )
{
	int nameLen = wstrlen( fileName );
	if( ( ( 1 == nameLen ) && ( '.' == fileName[0] ) )
		|| ( ( 2 == nameLen ) && ( '.' == fileName[0] ) && ( '.' == fileName[1] ) ) )
	{
		return true;
	}

	return false;
}

//============================================================================
// append file name to path.. account for url etc
std::string VxFileUtil::addFileToFolder( std::string& strFolder,  std::string& strFile)
{
	if( VxUrl::isURL( strFolder ) )
	{
		VxUrl url( strFolder );
		if( url.getFileName() != strFolder )
		{
            url.setFileName( strFile );
			return url.getUrl();
		}
	}

	std::string strResult = strFolder;
	if (!strResult.empty())
	{
		assureTrailingDirectorySlash( strResult );
	}

	// Remove any slash at the start of the file
	if( strFile.size() && ( ( '/' == strFile[0] )  || ( '\\' == strFile[0] ) ) )
		strResult += strFile.substr(1);
	else
		strResult += strFile;

	makeForwardSlashPath( strResult );

	return strResult;
}

//============================================================================
RCODE VxFileUtil::getCurrentWorkingDirectory( std::string strRetDir )
{
	char* buffer;
#ifdef TARGET_OS_WINDOWS
	if( (buffer = _getcwd( NULL, 0 )) == NULL )
#else
	if( (buffer = getcwd( NULL, 0 )) == NULL )
#endif
	{
		strRetDir = "";
		LogMsg( LOG_INFO, "getCurrentWorkingDirectory: getcwd error\n" );
		return -1;
	}
	else
	{
		strRetDir = buffer;
        free(buffer);
		return 0;
	}
}

//============================================================================
RCODE VxFileUtil::setCurrentWorkingDirectory( const char* pDir )
{
#ifdef TARGET_OS_WINDOWS
	return _chdir(pDir);
#else
	return chdir(pDir);
#endif
}

//============================================================================
//! returns file size or 0 if doesn't exist or zero length
uint64_t VxFileUtil::fileExists( const char* pFileName )
{
	int result;
#ifdef TARGET_OS_WINDOWS
	struct __stat64 gStat;
	// Get data associated with the file
	result = _wstat64( Utf8ToWide( pFileName ).c_str(), &gStat );
#else
	struct stat64 gStat;
	// Get data associated with the file
	result = stat64( pFileName, &gStat );
#endif //TARGET_OS_WINDOWS

	// Check if statistics are valid:
	if( result != 0 )
	{
		//error getting file info
#if defined(DEBUG)
        int errCode = VxGetLastError();
        LogMsg( LOG_DEBUG, "File Exists Error %d %s", errCode, pFileName );
#endif // defined(DEBUG)
		return 0;
	}
	else
	{
		//return file size
		return gStat.st_size;
	}
}

//============================================================================
//! return false if no longer exists
bool  VxFileUtil::getFileTypeAndLength( const char* pFileName, uint64_t& retFileLen, uint8_t& retFileType )
{
    retFileLen = 0;
    retFileType = 0;
    if( 0 == pFileName )
    {
        LogMsg( LOG_DEBUG, "VxFileUtil::getFileTypeAndLength: NULL File Name" );
        return false;
    }

    if( directoryExists( pFileName ) )
    {
        retFileType = VXFILE_TYPE_DIRECTORY;
        return true;
    }
    
    int result;
#ifdef TARGET_OS_WINDOWS
    struct __stat64 gStat;
    // Get data associated with the file
    result = _wstat64( Utf8ToWide( pFileName ).c_str(), &gStat );
#else
    struct stat64 gStat;
    // Get data associated with the file
    result = stat64( pFileName, &gStat );
#endif //TARGET_OS_WINDOWS

    // Check if statistics are valid:
    if( result != 0 )
    {
        //error getting file info
        int errCode = VxGetLastError();
        LogMsg( LOG_DEBUG, "File Exists Error %d %s", errCode, pFileName );
        return false;
    }
    else
    {
        //return file size
        retFileLen =  gStat.st_size;
        retFileType = fileExtensionToFileTypeFlag( pFileName );
        return true;
    }
}

//============================================================================
//! return true if directory exists
bool VxFileUtil::directoryExists( const char* pDir )
{
	char acBuf[ VX_MAX_PATH ];
	strcpy( acBuf, pDir );
	bool bIsDir = true;
	struct stat oFileStat;

	if( strlen( acBuf ) > 3 )
	{
		//if not root of drive remove the trailing backslash
		if( ('/' == acBuf[ strlen( acBuf ) - 1 ]) ||
			('\\' == acBuf[ strlen( acBuf ) - 1 ]) )
		{
			acBuf[ strlen( acBuf ) - 1 ] = 0;
		}
	}
	memset( &oFileStat, 0, sizeof( struct stat ) );
#ifdef TARGET_OS_WINDOWS
	oFileStat.st_mode = _S_IFDIR; //check for dir not file
	if( 0 == stat( acBuf, &oFileStat ) )
	{
		if( false == ( oFileStat.st_mode & _S_IFDIR ))
		{
			//path is not valid directory
			bIsDir = false;
		}
	}
	else
	{
		bIsDir = false;
	}
#else // LINUX or android
	oFileStat.st_mode = S_IFDIR; //check for dir not file
	if( 0 == stat( acBuf, &oFileStat ) )
	{
		if( false == ( oFileStat.st_mode & S_IFDIR ))
		{
			//path is not valid directory
			bIsDir = false;
		}
	}
    else
	{
		bIsDir = false;
	}
#endif // LINUX
	return bIsDir;
}

//============================================================================
uint64_t VxFileUtil::getFileLen( const char* pFileName )			
{ 
	return fileExists(pFileName); 
}

//============================================================================
RCODE VxFileUtil::makeDirectory( std::string& strDirectoryPath )	
{ 
	return makeDirectory( strDirectoryPath.c_str()); 
}

//============================================================================
void VxFileUtil::assureTrailingDirectorySlash( std::string& strDirectoryPath )
{
	if( strDirectoryPath.length() )
	{
		const char* name = strDirectoryPath.c_str();
		if( '/' != name[ strDirectoryPath.length() - 1 ] )
		{
			strDirectoryPath += "/";
		}
	}
}

//============================================================================
void VxFileUtil::removeTrailingDirectorySlash( std::string& strDirectoryPath )
{
	if( strDirectoryPath.length() )
	{
		const char* name = strDirectoryPath.c_str();
		if( ( '/' == name[strDirectoryPath.length() - 1] ) || ( '\\' == name[strDirectoryPath.length() - 1] ) )
		{
			strDirectoryPath = strDirectoryPath.substr( 0, strDirectoryPath.length() - 1 );
		}
	}
}

//============================================================================
std::string VxFileUtil::makeUniqueFileName( const char* fileName )
{
	std::string retFileName			= fileName;
	std::string fileNamePart		= "";
	std::string fileExtensionPart	= "";

	seperateFileNameAndExtension( retFileName, fileNamePart, fileExtensionPart );
	//if( 0 != fileExtensionPart.length() )
	{
		int fileIncBinary = 1;
		char fileIncBuf[16];
		while( fileExists( retFileName.c_str() ) )
		{
			sprintf( fileIncBuf, "(%d)", fileIncBinary );
			fileIncBinary++;
			retFileName = fileNamePart;
			retFileName += fileIncBuf;
			retFileName += fileExtensionPart;
		}
	}

	return retFileName;
}

//============================================================================
//! Make all directories that don't exist in a given path
RCODE VxFileUtil::makeDirectory( const char* pDirectoryPath )
{
    vx_assert( pDirectoryPath );
    if( !pDirectoryPath )
    {
        LogMsg( LOG_ERROR, "makeDirectory null path");
        return -1;
    }

    int pathLen = strlen(pDirectoryPath);
    vx_assert( pathLen < VX_MAX_PATH );
    if( ( pathLen >= VX_MAX_PATH ) || ( pathLen < 2 ) )
    {
        LogMsg( LOG_ERROR, "makeDirectory invalid path");
        return -2;
    }

    bool createdDirectories = false;
    char tempDir[VX_MAX_PATH * 2];
    //make a copy
    strcpy(tempDir, pDirectoryPath);
    for(int i = 0; i < pathLen; i++)
    {
        if( tempDir[i] == '\\' )
        {
             tempDir[i] = '/';
        }
    }

    //if no '/' at end put it there
    if( '/' != tempDir[strlen(tempDir) - 1] )
    {
        strcat(tempDir , "/");
    }

    //make the path
    char * pTemp = tempDir;
    #ifdef TARGET_OS_WINDOWS
		// for windows .. if root drive skip it
		if( ':' == pTemp[1] )
		{
			pTemp = strchr( pTemp, '/' );
			if( pTemp )
			{
				pTemp++;
			}
		}
	#endif //TARGET_OS_WINDOWS
    if( pTemp && strlen( pTemp ) )
    {
		while((pTemp = strtok(pTemp, "/" )))
		{
            //look for drive letter or root path
            if(0 == strlen(tempDir))
            {
                continue;
            }

            if(!directoryExists(tempDir))
            {
                //make directory
                if( ! (0 == VxMkDir( tempDir, S_IRWXU | S_IRWXG | S_IRWXO )))
                {
                    LogMsg( LOG_INFO, "CoreLib:FailedToMakeDir %s", tempDir );
                    return -3;
                }
                else
                {
                    // LogModule( eLogStorage, LOG_VERBOSE, "created directory %s", tempDir);
                    createdDirectories = true;
                }
            }

            //move pTemp up
            pTemp = tempDir + strlen(tempDir);
            //put the '/' back
            tempDir[strlen(tempDir)] = '/';
		}
   }

   if( createdDirectories )
   {
       LogModule( eLogStorage, LOG_VERBOSE, "created directory %s", tempDir);
   }

   return 0;
}

//============================================================================
//! read a line from file into buffer and null terminate it
RCODE VxFileUtil::readLine( FILE *pgFile, char *pBuf, int iBufLen )
{
    int c;
    int i= 0;

    //=== Read a line  ===//
	// skip over \n or \r if is the first character
	c = getc( pgFile );
	if( c == '\n' || c == '\r' )
	{
		c = getc( pgFile );
	}

    while( c != '\n' && c != '\r' )
    {
        if( c == EOF )
		{
			pBuf[i] = 0;
            return EOF;
		}
        if( i < iBufLen )
		{
            pBuf[i++] = (char)c;
		}
		else
		{
			LogMsg( LOG_INFO, "Read Line Buffer Overflow" );
			return EOF; // not enough room in buffer
		}

		c = getc( pgFile );
    }

    pBuf[i] = '\0';
    return 0;
}

//============================================================================
//! open a file and log error message if fails
FILE * VxFileUtil::fileOpen( const char* pFileName, const char* pFileMode )
{
    FILE * retval;
    retval = fopen( pFileName, pFileMode );
	if( NULL == retval )
	{
		LogMsg( LOG_INFO, "fileOpen:Could not open file %s", pFileName );
	}

    return retval;
}

//============================================================================
//! File seek..NOTE: only seeks from beginning of file
RCODE VxFileUtil::fileSeek ( FILE * poFile, uint32_t u32Pos )
{
	return fseek( poFile, u32Pos, SEEK_SET );
}
//============================================================================
//! File seek..NOTE: only seeks from beginning of file
RCODE VxFileUtil::fileSeek ( FILE * poFile, uint64_t u64Pos )
{
#ifdef TARGET_OS_WINDOWS
	return _fseeki64( poFile, u64Pos, SEEK_SET );
#else
	return fseek( poFile, u64Pos, SEEK_SET );
#endif// TARGET_OS_WINDOWS
}

//============================================================================
RCODE VxFileUtil::copyFile( const char* pOldPath, const char* pNewPath )
{
	#ifdef TARGET_OS_WINDOWS
		if( CopyFileW( Utf8ToWide( pOldPath ).c_str(), Utf8ToWide( pNewPath ).c_str(), false ) )
		{
			return 0;
		}
		return VxGetLastError();
	#else // LINUX
		deleteFile( pNewPath );
		char as8Buf[ VX_MAX_PATH * 2 ];
		sprintf( as8Buf, "cp %s %s\n", pOldPath, pNewPath );
		system( as8Buf );
		unlink( pOldPath );
		return 0;
	#endif // LINUX
}
//============================================================================
//! copy all files and directories to destination directory
RCODE VxFileUtil::recursiveCopyDirectory( const char* pSrcDir, const char* pDestDir, int64_t& totalCopied )
{
    RCODE rc = 0;
    totalCopied = 0;
    if( pDestDir && pSrcDir )
    {
        LogMsg( LOG_INFO, "recursive copy files from %s to %s", pSrcDir, pDestDir );

        std::string curPath = pSrcDir;
        std::string srcDir = pSrcDir;
        std::string destDir = pDestDir;
        std::vector<VxFileInfo> retFileList;

        if( !srcDir.empty() && !destDir.empty() )
        {
            //makeForwardSlashPath( srcDir );
            //makeForwardSlashPath( destDir );

            //assureTrailingDirectorySlash( srcDir );
            //assureTrailingDirectorySlash( destDir );

            if( directoryExists( srcDir.c_str() ) && directoryExists( destDir.c_str() ) )
            {
                VxFileCopier dirCopier;
                rc = dirCopier.copyDirectory( curPath, srcDir, destDir, retFileList, true );
                if( !rc )
                {
                    for( VxFileInfo& fileInfo : retFileList )
                    {
                        totalCopied += fileInfo.getFileLength();
                    }
                }
            }
            else
            {
                rc = EBADF;
                LogMsg( LOG_ERROR, "recursiveCopyDirectory invalid directory" );
            }
        }
        else
        {
            rc = EBADF;
            LogMsg( LOG_ERROR, "recursiveCopyDirectory empty directory path" );
        }
    }
    else
    {
        rc = EBADF;
        LogMsg( LOG_ERROR, "recursiveCopyDirectory null param" );
    }

    return rc;
}

//============================================================================
RCODE VxFileUtil::deleteFile( const char* pFileName )
{
	#ifdef TARGET_OS_WINDOWS
		return _unlink( pFileName );
	#else // LINUX
		return unlink( pFileName );
	#endif // LINUX
}

//============================================================================
RCODE VxFileUtil::renameFile( const char* pFileOldName, const char* pFileNewName )
{
	#ifdef TARGET_OS_WINDOWS
		return rename( pFileOldName, pFileNewName );
	#else // LINUX
		return rename( pFileOldName, pFileNewName );
	#endif // LINUX
}

//============================================================================
//! copy files to destination directory then delete the source files
RCODE VxFileUtil::moveFiles( char * pDestDir, char * pSrcDir )
{
	// make directory if doesn't exits
	makeDirectory( pDestDir );
	// loop through files in source directory and move them

	LogMsg( LOG_INFO, "VxMoveFile: moving files from %s to %s\n", pSrcDir, pDestDir );

	#ifdef TARGET_OS_WINDOWS
		std::wstring strSrcDir = Utf8ToWide( pSrcDir );
		std::wstring strDestDir = Utf8ToWide( pDestDir );

		wchar_t as8DestDir[ VX_MAX_PATH * 2 ];
		wchar_t destFile[ VX_MAX_PATH * 2 ];
		wstrcpy( as8DestDir, strDestDir.c_str() );

		// build path and wild card
		wchar_t srcDir[ VX_MAX_PATH * 2 ];
		wchar_t srcFile[ VX_MAX_PATH * 2 ];
		wstrcpy( srcDir, Utf8ToWide( pSrcDir ).c_str() );
		wstrcat( srcDir, L"\\*.*" );
		// start working for files
		WIN32_FIND_DATAW FindFileData;

		HANDLE hFind = FindFirstFileW( srcDir, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			// done with listing
			return 0;
		}

		bool bFinished = false;
		while ( false == bFinished)
		{
			// skip . and .. files; otherwise, we'd
			// recur infinitely!
			if( isDotDotDirectory( FindFileData.cFileName ) )
			{
				if( false == FindNextFileW(hFind, &FindFileData)  )
					break;
				continue;
			}

			// make source file name
			wstrcpy( srcFile, strSrcDir.c_str()  );
			wstrcat( srcFile, L"\\" );
			wstrcat( srcFile, FindFileData.cFileName );
			// make destination file name
			wstrcpy( destFile, strDestDir.c_str() );
			wstrcat( destFile, L"\\" );
			wstrcat( destFile, FindFileData.cFileName );

			struct _stat m;
			if( 0 != _wstat( srcFile, &m) )
			{
				LogMsg( LOG_ERROR, "VxFileUtil::listFilesAndFolders ERROR %d\n", VxGetLastError() );
			}
			else
			{
				if( _S_IFDIR & m.st_mode )
				{
					// its a directory
				}
				else
				{
					// its a file
					if( 0 == copyFile( WideToUtf8( srcFile ).c_str(), WideToUtf8( destFile ).c_str() ) )
					{
						deleteFile( WideToUtf8( srcFile ).c_str() );
					}
				}
			}

			if( false == FindNextFileW(hFind, &FindFileData)  )
			{
				// done with listing
				FindClose(hFind);
				return 0;
			}
		}
		return 0;
	#else //LINUX
		char as8DestDir[ VX_MAX_PATH * 2 ];
		char as8DestFile[ VX_MAX_PATH * 2 ];
		strcpy( as8DestDir, pDestDir );

		// build path and wild card
		char as8SrcDir[ VX_MAX_PATH * 2 ];
		char as8SrcFile[ VX_MAX_PATH * 2 ];
		strcpy( as8SrcDir, pSrcDir );
		// find the files in the directory
		DIR *pDir;
		struct dirent *pFileEnt;
		if( directoryExists( as8SrcDir ) )
		{
			//LogMsg( LOG_INFO, "VxMoveFile: directory %s exists.. opening dir\n", as8SrcDir );
			//ok directory exists!
			if(!(NULL == (pDir = opendir(as8SrcDir))))
			{
				//pDir is open
				while( 0 != (pFileEnt = readdir(pDir)))
				{
					//LogMsg( LOG_INFO, "VxMoveFile: found file %s\n", pFileEnt->d_name );
					//got a file or directory
					if( isDotDotDirectory(  pFileEnt->d_name ) ) 
					{
						// skip . and ..
						//LogMsg( LOG_INFO, "VxMoveFile: skipping file %s\n", pFileEnt->d_name );
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
					LogMsg( LOG_INFO, "VxMoveFile: made src file %s\n", as8SrcFile );
					// make destination file name
					strcpy( as8DestFile, as8DestDir );
					if( '/' != as8DestFile[ strlen( as8DestFile ) - 1 ] )
					{
						strcat( as8DestFile, "/" );
					}
					strcat( as8DestFile, pFileEnt->d_name );
					LogMsg( LOG_INFO, "VxMoveFile: made dest file %s\n", as8DestFile );
					//=== Last Modification Date ===//
					struct stat m;
					if( 0 != stat( as8SrcFile, &m) )
					{
						///ERROR how do we handle
						LogMsg( LOG_INFO, "Unable to stat file %s\n", as8SrcFile );
						continue;
					}

					if( pFileEnt->d_type == DT_DIR )
					{
						// its a directory
						LogMsg( LOG_INFO, "file %s is directory\n", as8SrcFile );
						continue;
					}

					// its a file
					LogMsg( LOG_INFO, "VxMoveFile: moving file %s to file %s\n", as8SrcFile, as8DestFile );
					if( 0 == copyFile( as8SrcFile, as8DestFile ) )
					{
						deleteFile( as8SrcFile );
					}
					else
					{
						LogMsg( LOG_INFO, "VxMoveFile: FAILED moving file %s to file %s\n", as8SrcFile, as8DestFile );
					}
				}
				// end of listing
				// done with listing
				closedir(pDir);
				return 0;
			}
			else
			{
				LogMsg( LOG_INFO, "moveFiles: could not open directory %s \n", as8SrcDir );
			}
		}
		else
		{
			LogMsg( LOG_INFO, "moveFiles: directory %s does not exist \n", as8SrcDir );
		}
		return 0;
	#endif //LINUX
}

//============================================================================
RCODE VxFileUtil::moveAFile( const char* srcFile, const char* destFile )
{
	int result = rename( srcFile, destFile );
	RCODE rc = 0;
	if( result )
	{
		rc = VxGetLastError();
        if( 0 == rc )
		{
			rc = result;
		}
	}

	// might be the destination is not on the same drive
	if( rc )
	{
		rc = copyFile( srcFile, destFile );
		if( 0 == rc )
		{
			// successful copy.. delete old file
			deleteFile( srcFile );
		}
	}

	return rc;
}

//============================================================================
//! separate file name into file name and extension strings
void VxFileUtil::seperateFileNameAndExtension(	std::string &	fileNameWithExt,		// file name with extension				
												std::string &	strRetFileNamePart,		// return file name part without .ext
												std::string &	strRetExtensionPart )	// return .ext part
{
	strRetFileNamePart		= fileNameWithExt;
	strRetExtensionPart		= "";
	char as8Buf[ VX_MAX_PATH ];
	strcpy( as8Buf, fileNameWithExt.c_str() );
	makeForwardSlashPath( as8Buf );
	char * period = strrchr( as8Buf, '.' );
	if( period )
	{
		char * forwardSlash = strrchr( period, '/' );
		if( 0 == forwardSlash )
		{
			strRetExtensionPart = period;
			period[0]			= 0;
			strRetFileNamePart	= as8Buf;
		}
		else
		{
			LogMsg( LOG_ERROR, "seperateFileNameAndExtension.. no extension..has slash\n" );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "seperateFileNameAndExtension.. no extension\n" );
	}
}


//============================================================================
//! separate Path and file name into separate strings
RCODE VxFileUtil::seperatePathAndFile(	std::string &	strFullPath,	// path and file name			
										std::string &	strRetPath,		// return path to file
										std::string &	strRetFile )	// return file name
{
	return seperatePathAndFile( strFullPath.c_str(), strRetPath, strRetFile );
}

//============================================================================
//! separate Path and file name into separate strings
RCODE	VxFileUtil::seperatePathAndFile(	const char*	pFullPath,		// path and file name			
											std::string &	strRetPath,		// return path to file
											std::string &	strRetFile )	// return file name
{
	char as8Buf[ VX_MAX_PATH ];
	strcpy( as8Buf, pFullPath );
	makeForwardSlashPath( as8Buf );
	char * pForwardSlash = strrchr( as8Buf, '/' );
	if( pForwardSlash )
	{
		// get file name
		strRetFile = &pForwardSlash[1];
		// get path
		pForwardSlash[1] = 0;
		strRetPath = as8Buf;
		return 0;
	}
	else
	{
		strRetPath = "";
		strRetFile = pFullPath;
	}

	//vx_assert( false );
	//return -1;
	return 0;
}

//============================================================================
//! remove the path and return just the file name
void	VxFileUtil::getJustFileName(	const char*	pFullPath,				// file name may be full or just file name
										std::string&	strRetJustFileName )	// return file name
{
	std::string	strRetPath;
	RCODE rc = seperatePathAndFile(	pFullPath,				// path and file name			
									strRetPath,				// return path to file
									strRetJustFileName );	// return file name
	if( rc )
	{
		LogMsg( LOG_ERROR, "getJustFileName: error %d file %s\n", rc, pFullPath );
		strRetJustFileName = pFullPath;
	}
}

//============================================================================
//! remove the file name and return just the path
std::string VxFileUtil::getJustPath( std::string fullPath )	// file name and path
{
    size_t i = fullPath.rfind( '/', fullPath.length() );
    if ( i != std::string::npos )
    {
        return( fullPath.substr( 0, i) );
    }

    return("");
}

//============================================================================
//! get the . extension of file name
void	VxFileUtil::getFileExtension(	std::string&	strFileName,	// file name with extension
										std::string&	strRetExt )		// return extension ( ie "myfile.etm" would return etm"
{
	int iIdx;
	if( -1 != (iIdx = StdStringReverseFind( strFileName, '.') ) )
	{
		strRetExt = &(((const char*)strFileName.c_str())[iIdx + 1]);
	}
	else
	{
		//ErrMsgBox("No File Extension Found %s\n", csFileName.c_str() );
		strRetExt = "";
	}
}

//============================================================================
//! flip back slashes into forward slashes
void VxFileUtil::makeForwardSlashPath( std::string & csFilePath )
{
	makeForwardSlashPath( (char *)csFilePath.c_str() );
}

//============================================================================
//! flip back slashes into forward slashes
void VxFileUtil::makeForwardSlashPath( char * pFilePath )
{
	size_t iLen = strlen( pFilePath );
	for( size_t i = 0; i < iLen; i++ )
	{
		if( '\\' == pFilePath[i] )
			pFilePath[i] = '/';
	}
}

//============================================================================
//! flip back slashes into forward slashes
void VxFileUtil::makeBackwardSlashPath( std::string & csFilePath )
{
	makeBackwardSlashPath( (char *)csFilePath.c_str() );
}

//============================================================================
//! flip back slashes into forward slashes
void VxFileUtil::makeBackwardSlashPath( char * pFilePath )
{
	size_t iLen = strlen( pFilePath );
	for( size_t i = 0; i < iLen; i++ )
	{
		if( '/' == pFilePath[i] )
			pFilePath[i] = '\\';
	}
}

//============================================================================
//! return true if last char is '/' else '\\'
bool VxFileUtil::doesPathEndWithSlash( const char* pFileName )
{
	int iStrLen = strlen( pFileName );
	if( ('/' == pFileName[iStrLen-1]) ||
		('\\' == pFileName[iStrLen-1]) )
	{
		return true;
	}
	return false;
}

//============================================================================
//! append slash if needed
void VxFileUtil::assurePathEndWithSlash( std::string &csFileName )
{
    if (!doesPathEndWithSlash(csFileName.c_str()))
    {
        csFileName += '/';
    }
}

//============================================================================
//! return true if is a root path like C:\dir or /dir
bool VxFileUtil::isFullPath( const char* pFileName )
{
	bool isFullPath = false;
	if( pFileName )
	{
		int iStrLen = strlen( pFileName );
		if( iStrLen )
		{
			if('/' == pFileName[0] )
			{
				// linux full path
				isFullPath = true;
			}
			else if( iStrLen > 1 )
			{
				if( ':' == pFileName[ 1 ] )
				{
					// windows full path
					isFullPath = true;
				}
			}
		}
	}
	return isFullPath;
}

//============================================================================
//! Make full path to execute directory if full path was not specified
//! NOTE: be careful .. assumes pFileName has enough space for full path and file name
void VxFileUtil::makeFullPath( char * pFileName )
{
	if( false == isFullPath(pFileName) )
	{
		// add path to the data directory
		std::string csFullPath;
		getExecuteDirectory( csFullPath );
		csFullPath += pFileName;
		strcpy( pFileName, csFullPath.c_str() );
	}
}

//============================================================================
//! Make full path to given directory if full path was not specified.. make path if does not exist
void VxFileUtil::makeFullPath( const char* pShortFileName, const char* pDownloadDir, std::string & strRetPath )
{
	if( isFullPath( pShortFileName ) )
	{
		strRetPath = pShortFileName;
	}
	else
	{
		strRetPath = pDownloadDir;
		if( false == doesPathEndWithSlash(pDownloadDir))
		{
			strRetPath += "/";
		}

		strRetPath += pShortFileName;
	}
}

//============================================================================
//! Make short FileName.. if pDownloadDir and full path contains pDownloadDir then will be path in that dir else just filename
bool VxFileUtil::makeShortFileName( const char* pFullFileName, std::string & strRetShortName, const char* pDownloadDir )
{
	bool bUsedDownloadDir = false;
	if( isFullPath( pDownloadDir ) )
	{
		int iDirStrLen = strlen( pDownloadDir );
		int iFileStrLen = strlen( pFullFileName );
		if( iFileStrLen > iDirStrLen )
		{
			if( 0 == strncmp( pDownloadDir, pFullFileName, iDirStrLen ) )
			{
				strRetShortName = &pFullFileName[ iDirStrLen ];
				bUsedDownloadDir = true;
			}
			else
			{
				getJustFileName( pFullFileName, strRetShortName);
			}
		}
		else
		{
			// return just file name
			getJustFileName( pFullFileName, strRetShortName);
		}
	}
	else if( isFullPath( pFullFileName ) )
	{
		// return just file name
		std::string strFileName;
		std::string strPath;
		RCODE rc = seperatePathAndFile(	pFullFileName,	// path and file name			
			strPath,		// return path to file
			strFileName );	// return file name
		if( 0 == rc )
		{
			//! separate Path and file name into separate strings
			strRetShortName = strFileName;
		}
		else
		{
			LogMsg( LOG_ERROR, "makeShortFileName: invalid file %s", pFullFileName );
			strRetShortName = pFullFileName; 
		}
	}
	else
	{
		strRetShortName = pFullFileName; 
	}
	return bUsedDownloadDir;
}

//============================================================================
//! Get execution full path
RCODE	VxFileUtil::getExecuteFullPathAndName( std::string& strRetExePathAndFileName )
{
	std::string strRetExeDir;
	std::string strRetExeFileName;
	RCODE rc = getExecutePathAndName( strRetExeDir, strRetExeFileName );
	strRetExePathAndFileName = strRetExeDir + strRetExeFileName;
	return rc;
}

//============================================================================
//! Get directory we execute from
RCODE	VxFileUtil::getExecuteDirectory( std::string& strRetExeDir )
{
	// try cached version first
	strRetExeDir = VxGetAppExeDirectory();
	if (!strRetExeDir.empty())
	{
		return 0;
	}

	std::string strRetExeFileName;
	return getExecutePathAndName( strRetExeDir, strRetExeFileName );
}

//============================================================================
//! Get execution path and file name
RCODE	VxFileUtil::getExecutePathAndName( std::string& strRetExeDir, std::string& strRetExeFileName )
{
#ifdef TARGET_OS_WINDOWS
	wchar_t pRetBuf[ VX_MAX_PATH ];
	int iRetStrLen = GetModuleFileNameW( NULL, pRetBuf, VX_MAX_PATH );
	if( 0 != iRetStrLen )
	{
		// remove file name
		wchar_t * pTemp = wstrrchr( pRetBuf, '\\' );
		if( pTemp )
		{
			* pTemp = 0;
			pTemp++;
			strRetExeFileName = WideToUtf8( pTemp );
		}
		// remove debug path if exists
		pTemp = wstrrchr( pRetBuf, '\\' );
		if( pTemp )
		{
#ifdef _DEBUG
			if( 0 == wstrcmp( pTemp, L"\\DEBUG" ) ||
				0 == wstrcmp( pTemp, L"\\Debug" ) )
			{
				*pTemp = 0;
			}
#endif
		}
		// make sure has the final slash
		if( pRetBuf[ wstrlen( pRetBuf ) - 1 ] != '\\' )
		{
			wstrcat( pRetBuf, L"\\" );
		}
		// flip the slashes
		size_t uiStrLen = wstrlen( pRetBuf );
		for( size_t i = 0; i < uiStrLen; i++ )
		{
			if( L'\\' == pRetBuf[ i ] )
			{
				pRetBuf[ i ] = L'/';
			}
		}

		strRetExeDir = WideToUtf8( pRetBuf );
		return 0;
	}
	else
	{
		LogMsg( LOG_INFO, "Error %d occurred getting module directory\n", VxGetLastError());
		return -1;
	}

#else // LINUX
	char pRetBuf[ VX_MAX_PATH ];
	int iByteCount;
	char* pTempBuf;
	iByteCount = readlink("/proc/self/exe", pRetBuf, VX_MAX_PATH);
	if(-1 == iByteCount)
	{
		LogMsg( LOG_INFO, "Error %d occured getting module directory\n", VxGetLastError());
		return -1;

	}
	pRetBuf[iByteCount] = '\0';

	if(NULL == (pTempBuf = strrchr(pRetBuf,'/')))
	{
		LogMsg( LOG_INFO, "Error %d occured getting module directory\n", VxGetLastError());
		return -1;
	}
	pTempBuf[1] = '\0';

	//const char* dir * name = dirname( pRetBuf );

	strRetExeFileName = &pTempBuf[2];
	strRetExeDir = pRetBuf;
#endif // LINUX
	return 0;
}

//============================================================================
bool VxFileUtil::fileNameWildMatch( const char  * pMatchName, const char* pWildName )
{
//  test if a file name matches a file name pattern.
//   handles * and ? wildcard characters.

    int   bNoMatch = false;
    int   bDone = false;
    while (false == bDone)
    {
        switch (*pWildName)
        {
            case '*':
                // skip to last . or end of Name
				if(strchr(pWildName, '.'))
				{
					while(strchr(pWildName, '.'))
					{
						pWildName = strchr(pWildName, '.');
						pWildName++;
					}
					//pWildName--;
				}
				else
				{
					while (* pWildName)
						pWildName++;
				}
                // skip to last . or end of Name
				if( strchr(pMatchName, '.'))
				{
					while( strchr(pMatchName, '.'))
					{
						pMatchName = strchr(pMatchName, '.');
						pMatchName++;
					}
					//pMatchName--;
				}
				else
				{
					while (* pMatchName)
						pMatchName++;
				}
                break;

            case '?':
                pWildName++;
                pMatchName++;
                break;

            case 0:
                if (*pMatchName != 0)
                    bNoMatch = true;
                bDone = true;
                break;

            default:
                if (toupper(* pWildName) == toupper(* pMatchName))
                {
                    pWildName++;
                    pMatchName++;
                }
                else
                {
                    bNoMatch = true;
                    bDone = true;
                }
                break;
        }
    }
	if( bNoMatch )
		return false ;
	return true;
}

//============================================================================
//! allocate memory and read whole file into memory
//! NOTE: USER MUST DELETE THE RETURED POINTER OR MEMORY LEAK WILL OCCURE
RCODE	VxFileUtil::readWholeFile(	const char*	pFileName,			// file to read	
									void **			ppvRetBuf,			// return allocated buffer it was read into
                                    uint32_t *		pu32RetLenOfFile )	// return length of file
{
	RCODE rc = 0;
	uint32_t u32Len = (uint32_t)getFileLen( pFileName );
	if( 0 == u32Len )
	{
		rc = VxGetLastError();
		if( 0 == rc )
		{
			rc = -1;
		}
		LogMsg( LOG_ERROR, "readWholeFile: Could not open file %s", pFileName );
		return rc;
	}
	else
	{
		char * pTemp = new char[ u32Len + 16 ];
		RCODE rc = readWholeFile( pFileName, pTemp, u32Len );
		if( rc )
		{
			// error occurred so delete so no memory leak
			delete[] pTemp;
			* ppvRetBuf = 0;
			* pu32RetLenOfFile = 0;
		}
		else
		{
			// make sure is null terminated
			pTemp[ u32Len ] = 0;
			* ppvRetBuf = pTemp;
			* pu32RetLenOfFile = u32Len;
		}
		return rc;
	}
}

//============================================================================
//! read whole file of known length into existing buffer
//! NOTE assumes buffer has enough room for the whole file
RCODE VxFileUtil::readWholeFile(	const char*	pFileName,				// file to read
									void *			pvBuf,					// buffer to read into
                                    uint32_t		u32LenToRead,			// length to read ( assumes is same as file length
                                    uint32_t	*	pu32RetAmountRead )		// return length actually read if not null
{
	RCODE rc = 0;
	if( pu32RetAmountRead  )
	{
		* pu32RetAmountRead = 0;
	}
	FILE * poFile = fopen( pFileName, "rb" );
	if( NULL == poFile )
	{
		rc = VxGetLastError();
		LogMsg( LOG_INFO, "readWholeFile: error %d opening file %s\n", rc, pFileName );
		return rc;
	}
	size_t iResult = fread( pvBuf, 1, u32LenToRead, poFile );
	fclose( poFile );
	if( iResult > 0 )
	{
		if( pu32RetAmountRead  )
		{
			* pu32RetAmountRead = (uint32_t)iResult;
		}
		// null terminate it
		if( u32LenToRead > iResult )
		{
			((char *)pvBuf)[ iResult ] = 0;
		}
		return 0;
	}
	rc = VxGetLastError();
	if( 0 == rc )
	{
		rc = -1;
	}
	LogMsg( LOG_INFO, "readWholeFile: error %d reading file %s\n", rc, pFileName );
	return rc;
}

//============================================================================
//! allocate memory and read whole file into memory and decrypt
//! NOTE: USER MUST DELETE THE RETURED POINTER OR MEMORY LEAK WILL OCCURE
RCODE	VxFileUtil::readWholeFile(	VxKey *			poKey,				// key to decrypt with
									const char*	pFileName,			// file to read	
									void **			ppvRetBuf,			// return allocated buffer it was read into
									uint32_t *			pu32RetLenOfFile )	// return length of file
{
	uint32_t		u32FileLen;

	RCODE rc = readWholeFile( pFileName,
		                      ppvRetBuf,
		                      pu32RetLenOfFile );
	if( rc )
	{
		return rc;
	}

	u32FileLen = *pu32RetLenOfFile;
	vx_assert( u32FileLen );
	vx_assert( 0 == (u32FileLen & 0x0f ) );
	VxSymDecrypt( poKey, (char *)*ppvRetBuf, u32FileLen );
	return 0;
}

//============================================================================
//! write all of data to a file
RCODE	VxFileUtil::writeWholeFile(	const char*	pFileName,			// file to write to
									void *			pvBuf,				// data to write
                                    uint32_t		u32LenOfData )		// data length
{
	RCODE rc = 0;
	FILE * poFile = fopen( pFileName, "wb+" );
	if( NULL == poFile )
	{
		rc = VxGetLastError();
		if( 0 == rc )
		{
			rc = -1;
		}
		LogMsg( LOG_ERROR, "writeWholeFile: ERROR %d Could not write file %s %d bytes", rc, pFileName, u32LenOfData );
	}
	else
	{
		uint32_t u32Result = (uint32_t)fwrite( pvBuf, 1, u32LenOfData, poFile );
		if( u32Result != u32LenOfData )
		{
			rc = VxGetLastError();
			if( 0 == rc )
			{
				rc = -1;
			}

			LogMsg( LOG_ERROR, "writeWholeFile: ERROR %d Could not write file %s %d bytes", rc, pFileName, u32LenOfData );
		}

		fclose( poFile );
	}
	return rc;
}

//============================================================================
//! encrypt and write all of data to a file
RCODE VxFileUtil::writeWholeFile(	VxKey *			poKey,				// key to encrypt with
									const char*	pFileName,			// file to write to
									void *			pvBuf,				// data to write
                                    uint32_t		u32LenOfData )		// data length
{
	vx_assert( u32LenOfData );
	vx_assert( VxIsEncryptable( u32LenOfData ) );
	// make copy first
	char * pBuf = new char[ u32LenOfData ];
	memcpy( pBuf, pvBuf, u32LenOfData );
	VxSymEncrypt( poKey, (char *)pBuf, u32LenOfData );
	return writeWholeFile( pFileName, pBuf, u32LenOfData );
}

//============================================================================
RCODE VxFileUtil::listFilesInDirectory(	const char*				pSrcDir,
										std::vector<std::string>&	fileList )
{
	vx_assert( pSrcDir );
#ifdef TARGET_OS_WINDOWS
	std::wstring strSrcDir = Utf8ToWide( pSrcDir );
	// build path and wild card
	wchar_t srcDir[ VX_MAX_PATH * 2 ];
	wchar_t srcFile[ VX_MAX_PATH * 2 ];
	wstrcpy( srcDir, strSrcDir.c_str() );
	wstrcat( srcDir, L"\\*.*" );

	// start working for files
	WIN32_FIND_DATAW FindFileData;

	HANDLE hFind = FindFirstFileW( srcDir, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		// done with listing
		return 0;
	}
	bool bFinished = false;
	while ( false == bFinished)
	{
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if( isDotDotDirectory(  FindFileData.cFileName ) )
		{
			if( false == FindNextFileW(hFind, &FindFileData)  )
				break;
			continue;
		}

		// make source file name
		wstrcpy( srcFile, strSrcDir.c_str()  );
		wstrcat( srcFile, FindFileData.cFileName );

		struct _stat m;
		if( 0 != _wstat( srcFile, &m) )
		{
			LogMsg( LOG_ERROR, "VxFileUtil::listFilesAndFolders ERROR %d", VxGetLastError() );
		}
		else
		{
			if( _S_IFDIR & m.st_mode )
			{
				// its a directory
			}
			else
			{
				// its a file
				fileList.push_back( WideToUtf8( srcFile ).c_str() );
			}
		}

		if( false == FindNextFileW(hFind, &FindFileData)  )
		{
			// done with listing
			FindClose(hFind);
			return 0;
		}
	}
	return 0;
#else //LINUX
	// build path and wild card
	char as8SrcDir[ VX_MAX_PATH * 2 ];
	char as8SrcFile[ VX_MAX_PATH * 2 ];
	strcpy( as8SrcDir, pSrcDir );
	// find the files in the directory
	DIR *pDir;
	struct dirent *pFileEnt;
	if( directoryExists( as8SrcDir ) )
	{
		//LogMsg( LOG_INFO, "listFilesInDirectory:  directory %s exists.. opening dir\n", as8SrcDir );
		//ok directory exists!
		if(!(NULL == (pDir = opendir(as8SrcDir))))
		{
			//pDir is open
			while( 0 != (pFileEnt = readdir(pDir)))
			{
				//LogMsg( LOG_INFO, "listFilesInDirectory: found file %s\n", pFileEnt->d_name );
				//got a file or directory
				if( isDotDotDirectory(  pFileEnt->d_name ) )
				{
					// skip . and ..
					//LogMsg( LOG_INFO, "listFilesInDirectory: skipping file %s\n", pFileEnt->d_name );
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
				//LogMsg( LOG_INFO, "listFilesInDirectory:  found file %s\n", as8SrcFile );
				//=== Last Modification Date ===//
				struct stat m;
				if( 0 != stat( as8SrcFile, &m) )
				{
					///ERROR how do we handle
					LogMsg( LOG_INFO, "listFilesInDirectory: Unable to stat file %s", as8SrcFile );
					continue;
				}
				
				if( pFileEnt->d_type == DT_DIR )
				{
					// its a directory
					LogMsg( LOG_INFO, "listFilesInDirectory: file %s is directory", as8SrcFile );
					continue;
				}

				// its a file
				fileList.push_back( as8SrcFile );
			}
			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
			LogMsg( LOG_INFO, "listFilesInDirectory:  could not open directory %s ", as8SrcDir );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "listFilesInDirectory:  directory %s does not exist ", as8SrcDir );
	}
	return 0;
#endif //LINUX

}


//============================================================================
RCODE VxFileUtil::listFilesAndFolders(	const char*					pSrcDir,
										std::vector<VxFileInfo>&	fileList,
										uint8_t						fileFilterMask )
{
	vx_assert( pSrcDir );
#ifdef TARGET_OS_WINDOWS
	std::wstring strSrcDir = Utf8ToWide( pSrcDir );
	// build path and wild card
	wchar_t srcDir[ VX_MAX_PATH * 2 ];
	wchar_t srcFile[ VX_MAX_PATH * 2 ];
	wstrcpy( srcDir, strSrcDir.c_str() );
	wstrcat( srcDir, L"\\*.*" );

	// start working for files
	WIN32_FIND_DATAW FindFileData;

	HANDLE hFind = FindFirstFileW( srcDir, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		// done with listing
		return 0;
	}
	bool bFinished = false;
	while( false == bFinished )
	{
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if( isDotDotDirectory(  FindFileData.cFileName ) )
		{
			if( false == FindNextFileW(hFind, &FindFileData)  )
				break;
			continue;
		}

		// make source file name
		wstrcpy( srcFile, strSrcDir.c_str()  );
		wstrcat( srcFile, FindFileData.cFileName );

		struct _stat oStat;
		if( 0 != _wstat( srcFile, &oStat) )
		{
			//LogMsg( LOG_ERROR, "VxFileUtil::listFilesAndFolders ERROR %d\n", VxGetLastError() );
		}
		else
		{
			VxFileInfo fileInfo( WideToUtf8( srcFile ).c_str() );
			fileInfo.setFileLength( oStat.st_size );

			if( _S_IFDIR & oStat.st_mode )
			{
				// its a directory
				fileInfo.setFileType( VXFILE_TYPE_DIRECTORY );
				fileInfo.assureTrailingDirectorySlash();
			}
			else
			{
				// its a file
				fileInfo.setFileType( fileExtensionToFileTypeFlag( fileInfo.getFileName().c_str() ) );
			}

			if( 0 != ( fileInfo.getFileType() & fileFilterMask ) )
			{
				fileList.push_back( fileInfo );
			}
		}

		if( false == FindNextFileW(hFind, &FindFileData)  )
		{
			// done with listing
			FindClose(hFind);
			return 0;
		}
	}

	return 0;
#else //LINUX
	// build path and wild card
	char as8SrcDir[ VX_MAX_PATH * 2 ];
	char as8SrcFile[ VX_MAX_PATH * 2 ];
	strcpy( as8SrcDir, pSrcDir );
	// find the files in the directory
	DIR *pDir;
	struct dirent *pFileEnt;
	if( directoryExists( as8SrcDir ) )
	{
		//LogMsg( LOG_INFO, "listFilesAndFolders:  directory %s exists.. opening dir\n", as8SrcDir );
		//ok directory exists!
		if(!(NULL == (pDir = opendir(as8SrcDir))))
		{
			//pDir is open
			while( 0 != (pFileEnt = readdir(pDir)))
			{
				//LogMsg( LOG_INFO, "listFilesAndFolders: found file %s\n", pFileEnt->d_name );
				//got a file or directory
				if( isDotDotDirectory(  pFileEnt->d_name ) )
				{
					// skip . and ..
					//LogMsg( LOG_INFO, "listFilesAndFolders: skipping file %s\n", pFileEnt->d_name );
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
				//LogMsg( LOG_INFO, "listFilesAndFolders:  found file %s\n", as8SrcFile );
				//=== Last Modification Date ===//
				struct stat64 oStat;
				if ( 0 != stat64( as8SrcFile, &oStat ) )
				{
					///ERROR how do we handle
					LogMsg( LOG_ERROR, "listFilesAndFolders: ERROR %d when stat file %s", VxGetLastError(), as8SrcFile );
					continue;
				}

				VxFileInfo fileInfo( as8SrcFile );
                fileInfo.setFileLength( oStat.st_size );

				if( pFileEnt->d_type == DT_DIR )
				{
					// its a directory
                    fileInfo.setFileType( VXFILE_TYPE_DIRECTORY );
					fileInfo.assureTrailingDirectorySlash();
					LogMsg( LOG_INFO, "listFilesAndFolders: Is Directory %s", fileInfo.getFileName().c_str() );
				}
				else
				{
					// its a file
					LogMsg( LOG_INFO, "listFilesAndFolders: Is File %s", fileInfo.getFileName().c_str() );
                    fileInfo.setFileType( fileExtensionToFileTypeFlag( fileInfo.getFileName().c_str() ) );
				}

				if ( 0 != ( fileInfo.getFileType() & fileFilterMask ) )
				{
					fileList.push_back( fileInfo );
				}
				else
				{
					LogMsg( LOG_INFO, "listFilesAndFolders: Skip file type 0x%x not in filter mask 0x%x File %s", fileInfo.getFileType(), fileFilterMask, fileInfo.getFileName().c_str() );			
				}
			}

			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
			LogMsg( LOG_INFO, "listFilesInDirectory:  could not open directory %s", as8SrcDir );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "listFilesInDirectory:  directory %s does not exist", as8SrcDir );
	}

	return 0;
#endif //LINUX
}

//============================================================================
bool VxFileUtil::deleteFilesInFolder( std::string fileFolder, bool folderNameEndsWithOnlineId )
{
	bool result = !fileFolder.empty() && directoryExists( fileFolder.c_str() );
	if( !result )
	{
		LogMsg( LOG_ERROR, "deleteFilesInFolder directory does not exist" );
		return result;
	}

	if( folderNameEndsWithOnlineId )
	{
		std::string noTrailingSlash = fileFolder;
		removeTrailingDirectorySlash( noTrailingSlash );
		std::size_t foundPos = noTrailingSlash.find_last_of( "/" );
		if( std::string::npos == foundPos )
		{
			LogMsg( LOG_ERROR, "deleteFilesInFolder directory %s does not contain backslash", fileFolder.c_str() );
			return false;
		}

		VxGUID onlineId;
		onlineId.fromVxGUIDHexString( &( noTrailingSlash.c_str()[foundPos + 1]) );
		if( !onlineId.isVxGUIDValid() )
		{
			LogMsg( LOG_ERROR, "deleteFilesInFolder directory %s does not end with online id", fileFolder.c_str() );
			return false;
		}
	}

	std::vector<std::string> fileList;
	listFilesInDirectory( fileFolder.c_str(), fileList );
	for( auto& fileName : fileList )
	{
		result &= 0 == deleteFile( fileName.c_str() );
	}

	return result;
}

//============================================================================
bool VxFileUtil::incrementFileName( std::string& strFileName )
{
	std::string strRetFileNamePart;
	std::string strRetExtensionPart;
	seperateFileNameAndExtension(	strFileName,		// file name with extension				
									strRetFileNamePart,		// return file name part without .ext
									strRetExtensionPart );	// return .ext part
	if( strRetExtensionPart.size() && strRetExtensionPart.size() )
	{
		const char* fileNamePart = strRetFileNamePart.c_str();
		char lastChar = fileNamePart[ strRetFileNamePart.size() - 1 ];
		if( isdigit( lastChar ) && ( '9' > lastChar ))
		{
			// safe to just increment the char
			*( (char *)&fileNamePart[ strRetFileNamePart.size() - 1 ] ) = lastChar + 1;
			strFileName = strRetFileNamePart + strRetExtensionPart;
		}
		else
		{
			strRetFileNamePart += "_1";
			strFileName = strRetFileNamePart + strRetExtensionPart;
		}

		return true;
	}

	return false;
}

//============================================================================
uint8_t VxFileUtil::fileExtensionToFileTypeFlag( const char* pFileName )
{
	uint8_t fileType = VXFILE_TYPE_OTHER;
	if( pFileName )
	{
		std::string fileName = pFileName;
		if( fileName.size() )	
		{
			if ( '/' == pFileName[fileName.size() - 1] )
			{
				fileType = VXFILE_TYPE_DIRECTORY;
			}
			else
			{
				std::string fileExt = "";
				getFileExtension( fileName, fileExt );
				if( fileExt.size() )
				{
					fileType = VxFileExtensionToFileTypeFlag( fileExt.c_str() );
				}							
			}
		}
	}
	
	return fileType;
}

//============================================================================
uint64_t VxFileUtil::getDiskFreeSpace( const char* filePath  )
{
	uint64_t totalSpace;
	uint64_t freeSpace = 0;
	getDiskSpace( filePath, totalSpace, freeSpace );
	return freeSpace;
}

//============================================================================
bool VxFileUtil::getDiskSpace( const char* filePath, uint64_t& totalDiskSpace, uint64_t& diskSpaceAvail )
{
	totalDiskSpace = 0;
	diskSpaceAvail = 0;

#ifdef WIN32
	std::string dir = filePath;
	if( dir.length() > 2 )
	{
		dir = dir.substr( 0, 2 );

		ULARGE_INTEGER FreeBytesAvailable;
		ULARGE_INTEGER TotalNumberOfBytes;
		ULARGE_INTEGER TotalNumberOfFreeBytes;

        BOOL result = GetDiskFreeSpaceExA(	dir.c_str(),
											&FreeBytesAvailable,
											&TotalNumberOfBytes,
											&TotalNumberOfFreeBytes
											);
		if( result )
		{
			totalDiskSpace = TotalNumberOfBytes.QuadPart;
			diskSpaceAvail = FreeBytesAvailable.QuadPart;
			return true;
		}
		else
		{
			LogMsg( LOG_ERROR, "getDiskSpace %s error %d", filePath, GetLastError() );
		}
	}
	return false;
#else
	struct statfs sfs;
	if (statfs(filePath, &sfs) == 0)
	{
		totalDiskSpace = (uint64_t)sfs.f_blocks * sfs.f_bsize;
		diskSpaceAvail = (uint64_t)sfs.f_bavail * sfs.f_bsize;
		return true;
	}
/*#else
	struct statvfs sfs;
	if (statvfs(disk, &sfs) == 0)
	{
		totalDiskSpace = (uint64_t)sfs.f_blocks * sfs.f_bsize;
		diskSpaceAvail = (uint64_t)sfs.f_bavail * sfs.f_bsize;
		return true;
	}
#endif*/
#endif
	return false;
}

//============================================================================
bool VxFileUtil::u64ToHexAscii( uint64_t fileLen, std::string& retHexAscii  )
{
	return dataToHexAscii( (uint8_t *)&fileLen, 8, retHexAscii );
}

//============================================================================
bool VxFileUtil::hexAsciiToU64( const char* hexAscii, uint64_t& retFileLen  )
{
	return hexAsciiToData( hexAscii, (uint8_t *)&retFileLen, 8  ); 
}

//============================================================================
bool VxFileUtil::dataToHexAscii( uint8_t * dataBuf, int dataLen, std::string& retHexAscii  )
{
	if( ( 0 == dataBuf ) || ( 0 == dataLen ) )
	{
		retHexAscii = "";
		return false;
	}

	char * hexBuf = new char[ dataLen * 2 + 1 ];
	int strIdx = 0;
	for( int i = 0; i < dataLen; ++i )
	{
		hexBuf[ strIdx++ ] = binaryToHexChar( dataBuf[ i ] >> 4 );
		hexBuf[ strIdx++ ] = binaryToHexChar( dataBuf[ i ] );
	}

	hexBuf[ strIdx ] = 0;
	retHexAscii = hexBuf;
	delete[] hexBuf;
	return true;
}

//============================================================================
bool VxFileUtil::hexAsciiToData( const char* hexAscii, uint8_t * retDataBuf, int dataLen  )
{
	int strLen = strlen( hexAscii );
	uint8_t u8Byte;
	int byteIdx = 0;
	for( int i = 0; i < strLen; ++i )
	{
		u8Byte = (charToHexBinary(*hexAscii))<<4;
		hexAscii++;
		u8Byte |= charToHexBinary(*hexAscii);
		hexAscii++;
		retDataBuf[ byteIdx++ ] = u8Byte;
		if( byteIdx >= dataLen )
		{
			break;
		}
	}

	return true;
}

//============================================================================
uint8_t  VxFileUtil::charToHexBinary( char cVal )
{
	if( cVal >= '0' && cVal <= '9' )
	{
		return cVal & 0x0f;
	}
	else if( cVal >= 'A' && cVal <= 'F' )
	{
		return cVal - 55;
	}
	else if( cVal >= 'a' && cVal <= 'f' )
	{
		return cVal - 87;
	}
	else
	{
		LogMsg( LOG_ERROR, "VxGUID::charToHex invalid char 0x%2.2x\n", cVal );
		return 0;
	}
}

//============================================================================
char  VxFileUtil::binaryToHexChar( uint8_t u8Val )
{
	uint8_t byteVal = u8Val & 0x0f;
	if( byteVal < 10 )
	{
		return byteVal + '0';
	}

	return ( byteVal - 9 ) + 'A';
}
