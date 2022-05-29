#pragma once
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

#ifdef __cplusplus


#include <vector>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <stddef.h>
#include <errno.h>

#define VXFILE_TYPE_MASK					0xff
#define VXFILE_TYPE_PHOTO					0x01
#define VXFILE_TYPE_AUDIO					0x02
#define VXFILE_TYPE_VIDEO					0x04
#define VXFILE_TYPE_DOC						0x08
#define VXFILE_TYPE_ARCHIVE_OR_CDIMAGE		0x10
#define VXFILE_TYPE_EXECUTABLE				0x20
#define VXFILE_TYPE_OTHER					0x40
#define VXFILE_TYPE_DIRECTORY				0x80

#define VXFILE_TYPE_ALLNOTEXE				0x5F
#define VXFILE_TYPE_ANY						0x7F

#define RCODE long
# define     VxGetLastError()	errno
# define	 VxMkDir(exp,exp2)	mkdir(exp,exp2)     //make directory command we can use in windows like linux
# define     VX_MAX_PATH			260


class VxFileUtil
{
public:
	static RCODE				getCurrentWorkingDirectory( std::string strRetDir );
	static RCODE				setCurrentWorkingDirectory( const char * pDir );

	static bool					isDotDotDirectory( const char * fileName );
#ifdef TARGET_OS_WINDOWS
    static bool					isDotDotDirectory( const wchar_t * fileName );
#endif // TARGET_OS_WINDOWS

	// append file name to path.. account for url etc
	static std::string			addFileToFolder( std::string& strFolder, std::string& strFile );

	//! returns file size or 0 if doesn't exist
	static uint64_t				fileExists( const char * pFileName );
	static uint64_t				getFileLen( const char * pFileName );
    static bool                 getFileTypeAndLength( const char * pFileName, uint64_t& retFileLen, uint8_t& retFileType );
	static bool					directoryExists( const char * pDir );

	//! Make all directories that don't exist in a given path
	static RCODE				makeDirectory( const char * pDirectoryPath );
	//! Make all directories that don't exist in a given path
	static RCODE				makeDirectory( std::string& strDirectoryPath );
	static void					assureTrailingDirectorySlash( std::string& strDirectoryPath );
	static void					removeTrailingDirectorySlash( std::string& strDirectoryPath );
	static std::string			makeUniqueFileName( const char * fileName );

	//! read a line from file into buffer and null terminate it
	static RCODE				readLine( FILE *pgFile, char *pBuf, int iBufLen );

	static FILE *				fileOpen( const char * pFileName, const char * pFileMode );
	static RCODE				fileSeek ( FILE * poFile, uint32_t u32Pos );

	//! File seek..NOTE: only seeks from beginning of file
	static RCODE				fileSeek ( FILE * poFile, uint64_t u64Pos );
	static RCODE				copyFile( const char * pOldPath, const char * pNewPath );
	static RCODE				deleteFile( const char * pFileName );
	static RCODE				renameFile( const char * pFileOldName, const char * pFileNewName );

	//! copy files to destination directory then delete the source files
	static RCODE				moveFiles( char * pDestDir, char * pSrcDir );
	//! move a file from one directory to another
	static RCODE				moveAFile( const char * srcFile, const char * destFile );

	//! separate Path and file name into separate strings
	static RCODE				seperatePathAndFile(	const char *	pFullPath,		// path and file name			
														std::string &	strRetPath,		// return path to file
														std::string &	strRetFile );	// return file name

	//! separate Path and file name into separate strings
	static RCODE				seperatePathAndFile(	std::string &	strFullPath,		// path and file name				
														std::string &	strRetPath,		// return path to file
														std::string &	strRetFile );	// return file name

	//! separate file name into file name and extension strings
	static void					seperateFileNameAndExtension(	std::string &	fileNameWithExt,		// file name with extension				
																std::string &	strRetFileNamePart,		// return file name part without .ext
																std::string &	strRetExtensionPart );	// return .ext part

	//! remove the path and return just the file name
	static void					getJustFileName(	const char *	pFullPath,	// file name may be full or just file name
													std::string&	strRetJustFileName );		// return file name
    //! remove the file name and return just the path
    static std::string			getJustPath( std::string fullPath );	// file name and path

    //! get the . extension of file name
	static void					getFileExtension(	std::string&	strFileName,	// file name with extension
													std::string&	strRetExt );		// return extension ( ie "myfile.etm" would return etm"
	//! flip back slashes into forward slashes
	static void					makeForwardSlashPath( std::string & csFilePath );
	//! flip back slashes into forward slashes
	static void					makeForwardSlashPath( char * pFilePath );
	//! flip forward slashes into backward slashes
	static void					makeBackwardSlashPath( std::string & csFilePath );
	//! flip forward slashes into backward slashes
	static void					makeBackwardSlashPath( char * pFilePath );

	//! return true if last char is '/' else '\\'
	static bool					doesPathEndWithSlash( const char * pFileName );

    //! return true if is a root path like C:\dir or /dir
    static bool					isFullPath( const char * pFileName );

    //match file names using dos style wild chars
	static bool					fileNameWildMatch(const char  * pMatchName, const char * pWildName);
	//! read whole file of known length into existing buffer
	//! NOTE assumes buffer has enough room for the whole file
	static RCODE				readWholeFile(	const char *	pFileName,					// file to read
												void *			pvBuf,						// buffer to read into
                                                uint32_t		u32LenToRead,				// length to read ( assumes is same as file length
                                                uint32_t	*	pu32RetAmountRead = NULL );	// return length actually read if not null
	//! allocate memory and read whole file into memory
	//! NOTE: USER MUST DELETE THE RETURED POINTER OR MEMORY LEAK WILL OCCURE
	static RCODE				readWholeFile(	const char *	pFileName,			// file to read	
												void **			ppvRetBuf,			// return allocated buffer it was read into
                                                uint32_t *		pu32RetLenOfFile );	// return length of file
	//! write all of data to a file
	static RCODE				writeWholeFile(	const char *	pFileName,			// file to write to
												void *			pvBuf,				// data to write
                                                uint32_t		u32LenOfData );		// data length

	static RCODE				listFilesInDirectory(	const char *				pSrcDir,
														std::vector<std::string>&	fileList );

	static uint8_t				fileExtensionToFileTypeFlag( const char * fileName );
	static bool					incrementFileName( std::string& strFileName );

	static bool					getDiskSpace( const char * filePath, uint64_t& totalDiskSpace, uint64_t& diskSpaceAvail );
	static uint64_t				getDiskFreeSpace( const char * filePath  ); 

	static bool					u64ToHexAscii( uint64_t fileLen, std::string& retHexAscii  ); 
	static bool					hexAsciiToU64( const char * hexAscii, uint64_t& retFileLen  ); 

	static bool					dataToHexAscii( uint8_t * dataBuf, int dataLen, std::string& retHexAscii  ); 
	static bool					hexAsciiToData( const char * hexAscii, uint8_t * retDataBuf, int dataLen  ); 

	static uint8_t				charToHexBinary( char cVal );
	static char					binaryToHexChar( uint8_t u8Val );

	static std::string			makeKodiPath( const char * path );
};

size_t FindLastPathSeperator( std::string& path );
bool   AddExtraLongPathPrefix( std::string& path );
bool   RemoveExtraLongPathPrefix( std::string& path );
#ifdef TARGET_OS_WINDOWS
std::string WindowsRelativeToAbsolutePath( std::string& path );
#endif // TARGET_OS_WINDOWS

#endif // __cplusplus

#ifdef TARGET_OS_WINDOWS


#ifdef __cplusplus
extern "C" {
#endif
 bool WindowsRelativeToAbsolutePath( char * pathBuf,  int bufLen );
#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif // TARGET_OS_WINDOWS
