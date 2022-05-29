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

#define VXFILE_TYPE_UNKNOWN					0x00

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

enum EFileFilterType 
{
	eFileFilterAll					= 0,
	eFileFilterPhoto				= 1,
	eFileFilterAudio				= 2,
	eFileFilterVideo				= 3,
	eFileFilterDocuments			= 4,
	eFileFilterArchive				= 5,
	eFileFilterOther				= 6,

    eFileFilterEnd                  = 7,

    eFileFilterPhotoOnly            = 8,
    eFileFilterAudioOnly            = 9,
    eFileFilterVideoOnly            = 10,

	eMaxFileFilterType
};
