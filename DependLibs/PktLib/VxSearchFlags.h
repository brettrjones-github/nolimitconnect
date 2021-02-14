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
// http://www.nolimitconnect.com
//============================================================================

#include <inttypes.h>

#pragma pack(push) 
#pragma pack(1)

class PktBlobEntry;
//! 2 bytes in size
class VxSearchFlags
{
public:
	VxSearchFlags() = default;
    VxSearchFlags( const VxSearchFlags& rhs );
    VxSearchFlags&              operator =( const VxSearchFlags& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	void						setSearchFlags( uint8_t flags )			{ m_u8SearchFlags = flags; }
	uint8_t						getSearchFlags( void )				    { return m_u8SearchFlags; }
	void						setSharedFileTypes( uint8_t flags )		{ m_u8FileTypeFlags = flags; }
	uint8_t						getSharedFileTypes( void )			    { return m_u8FileTypeFlags; }

	bool 						hasSearchFlags( void );
	bool						hasSharedFiles( void );

	void						setHasPhotoFiles( bool bHasFiles );
	bool						hasPhotoFiles( void );

	void						setHasAudioFiles( bool bHasFiles );
	bool						hasAudioFiles( void );

	void						setHasVideoFiles( bool bHasFiles );
	bool						hasVideoFiles( void );

	void						setHasDocFiles( bool bHasFiles );
	bool						hasDocFiles( void );

	void						setHasArcOrCdImageFiles( bool bHasFiles );
	bool						hasArcOrCdFiles( void );

	void						setHasOtherFiles( bool bHasFiles );
	bool						hasOtherFiles( void );

	void						setHasProfilePicture( bool bHasPicture );
	bool						hasProfilePicture( void );

	void						setHasSharedWebCam( bool bHasWebCam );
	bool						hasSharedWebCam( void );

	void						setHasModifiedStoryboard( bool bHasStoryBoard );
	bool						hasModifiedStoryboard( void );

protected:
	//=== vars ===//
    uint8_t						m_u8SearchFlags{ 0 };
	uint8_t						m_u8FileTypeFlags{ 0 };
};

#pragma pack(pop)