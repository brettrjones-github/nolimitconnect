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

#include "VxFileIsTypeFunctions.h"
#include "VxFileUtil.h"
#include "VxParse.h"

#include <string.h>

#define PHOTO_FILE_EXTENTIONS			"jpg,jpeg,bmp,tga,png,gif,pcx,tif,ico"				
#define AUDIO_FILE_EXTENTIONS			"mp3,wav,wma,ogg,opus"				
#define VIDEO_FILE_EXTENTIONS			"asf,mpg,mpeg,mp4,3gp,mov,avi,divx,mkv,wmv,rm"				
#define DOCUMENT_FILE_EXTENTIONS		"doc,txt,htm,html,pdf"	
#define EXECUTABLE_FILE_EXTENTIONS		"exe,com,bat,cmd"		
#define CDIMAGE_OR_ARC_FILE_EXTENTIONS	"7z,zip,rar,tar,gz,iso,cue,ccd,img,sub,bin,mds,nrg,pdi,mds,vob"
#define THUMBNAIL_FILE_EXTENTIONS	    "nlt"
#define COMBINED_FILE_EXTENTIONS		"opus,jpg,jpeg,bmp,tga,png,gif,pcx,mp3,wav,wma,ogg,asf,mpg,mpeg,mp4,3gp,mov,avi,divx,mkv,wmv,rm,exe,com,bat,cmd,7z,zip,rar,tar,doc,txt,htm,html,pdf,nlt"	

//============================================================================
bool VxIsPhotoFile( std::string & cs )
{
	std::string csExt; 
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsPhotoFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsPhotoFileExtention( const char * pExt )
{
	return stristr( PHOTO_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsAudioFile( std::string & cs )
{
	std::string csExt; 
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsAudioFileExtention( csExt.c_str() );
}
//============================================================================
bool VxIsAudioFileExtention( const char * pExt )
{
	return stristr( AUDIO_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsVideoFile( std::string &cs )
{
	std::string csExt;
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsVideoFileExtention( csExt.c_str() );
}
//============================================================================
bool VxIsVideoFileExtention( const char * pExt )
{
	return stristr( VIDEO_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsDocumentFile( std::string &cs )
{
	std::string csExt;
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsDocumentFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsDocumentFileExtention( const char * pExt )
{
	return stristr( DOCUMENT_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsArcOrCDImageFile( std::string &cs )
{
	std::string csExt;
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsArcOrCDImageFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsArcOrCDImageFileExtention( const char * pExt )
{
	return stristr( CDIMAGE_OR_ARC_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsExecutableFile( std::string &cs )
{
	std::string csExt;
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsExecutableFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsExecutableFileExtention( const char * pExt )
{
	return stristr( EXECUTABLE_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsThumbnailFile( std::string &cs )
{
    std::string csExt;
    VxFileUtil::getFileExtension( cs, csExt );
    return VxIsThumbnailFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsThumbnailFileExtention( const char * pExt )
{
    return stristr( THUMBNAIL_FILE_EXTENTIONS, pExt )?1:0;
}

//============================================================================
bool VxIsRecognizedFile( std::string &cs )
{
	std::string csExt;
	VxFileUtil::getFileExtension( cs, csExt );
	return VxIsRecognizedFileExtention( csExt.c_str() );
}

//============================================================================
bool VxIsRecognizedFileExtention( const char * pExt )
{
	return stristr( COMBINED_FILE_EXTENTIONS, pExt )?0:1;
}

//============================================================================
bool VxIsShortcutFileExtention( const char * pExt )
{
#ifdef TARGET_OS_WINDOWS
	return stristr( pExt, "lnk" ) ? true : false;
#else
	return false; // no support for linux yet
#endif //TARGET_OS_WINDOWS
}

//============================================================================
bool VxIsShortcutFile( std::string& fileName )
{
#ifdef TARGET_OS_WINDOWS
	std::string csExt;
	VxFileUtil::getFileExtension( fileName, csExt );
	if( csExt.size() )
	{
		return VxIsShortcutFileExtention( csExt.c_str() );
	}
#endif //TARGET_OS_WINDOWS	
	return false;
}

//============================================================================
uint8_t	VxFileExtensionToFileTypeFlag( const char *	pFileExt )
{
	uint8_t u8FileType = VXFILE_TYPE_OTHER;
	if( pFileExt )
	{
		const char * extension = strrchr( pFileExt, '.' );
		if( extension )
		{
			extension++;
		}
		else
		{
			extension = pFileExt;
		}

		if( VxIsPhotoFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_PHOTO;
		}
		else if( VxIsAudioFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_AUDIO;
		}
		else if( VxIsVideoFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_VIDEO;
		}
		else if( VxIsDocumentFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_DOC;
		}
		else if( VxIsArcOrCDImageFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_ARCHIVE_OR_CDIMAGE;
		}
		else if( VxIsExecutableFileExtention( extension ) )
		{
			u8FileType = VXFILE_TYPE_EXECUTABLE;
		}
	}

	return u8FileType;
}

//============================================================================
bool VxIsMediaFile( uint8_t u8FileTypeFlag )
{
	return ( u8FileTypeFlag & ( VXFILE_TYPE_PHOTO | VXFILE_TYPE_AUDIO | VXFILE_TYPE_VIDEO )) ? true : false;
}

//============================================================================
bool VxShouldOpenFile( uint8_t u8FileTypeFlag )
{
	return ( u8FileTypeFlag & ( VXFILE_TYPE_PHOTO | VXFILE_TYPE_AUDIO | VXFILE_TYPE_VIDEO | VXFILE_TYPE_DOC )) ? true : false;
}

