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

#include "VxSearchFlags.h"
#include "VxSearchDefs.h"
#include "PktBlobEntry.h"

#include <CoreLib/VxFileInfo.h>

//============================================================================
VxSearchFlags::VxSearchFlags( const VxSearchFlags& rhs )
    : m_u8SearchFlags( rhs.m_u8SearchFlags )
    , m_u8FileTypeFlags( rhs.m_u8FileTypeFlags )
{
}

//============================================================================
VxSearchFlags& VxSearchFlags::operator =( const VxSearchFlags& rhs )
{
    if( this != &rhs )
    {
        m_u8SearchFlags = rhs.m_u8SearchFlags;
        m_u8FileTypeFlags = rhs.m_u8FileTypeFlags;
    }

    return *this;
}

//============================================================================
bool VxSearchFlags::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_u8SearchFlags );
    result &= blob.setValue( m_u8FileTypeFlags );
    return result;
}

//============================================================================
bool VxSearchFlags::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_u8SearchFlags );
    result &= blob.getValue( m_u8FileTypeFlags );
    return result;
}

//============================================================================
//! return true if any search bits are set
bool VxSearchFlags::hasSearchFlags( void )					
{ 
	return getSearchFlags()  ? true : false; 
}

//============================================================================
//! return true if has any shared files
bool	VxSearchFlags::hasSharedFiles( void )					
{ 
	return ( VXFILE_TYPE_ALLNOTEXE & m_u8FileTypeFlags ) ? true : false; 
}

//============================================================================
bool	VxSearchFlags::hasPhotoFiles( void )					
{ 
	return (VXFILE_TYPE_PHOTO & getSharedFileTypes()) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasPhotoFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSharedFileTypes( getSharedFileTypes() | VXFILE_TYPE_PHOTO );
	else
		setSharedFileTypes( getSharedFileTypes() & ~VXFILE_TYPE_PHOTO );
}

//============================================================================
bool	VxSearchFlags::hasAudioFiles( void )					
{ 
	return (VXFILE_TYPE_AUDIO & getSharedFileTypes()) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasAudioFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSharedFileTypes( getSharedFileTypes() | VXFILE_TYPE_AUDIO );
	else
		setSharedFileTypes( getSharedFileTypes() & ~VXFILE_TYPE_AUDIO );
}

//============================================================================
bool	VxSearchFlags::hasVideoFiles( void )					
{ 
	return (VXFILE_TYPE_VIDEO & getSharedFileTypes()) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasVideoFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSharedFileTypes( getSharedFileTypes() | VXFILE_TYPE_VIDEO );
	else
		setSharedFileTypes( getSharedFileTypes() & ~VXFILE_TYPE_VIDEO );
}

//============================================================================
bool	VxSearchFlags::hasDocFiles( void )					
{ 
	return (VXFILE_TYPE_DOC & getSharedFileTypes()) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasDocFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSharedFileTypes( getSharedFileTypes() | VXFILE_TYPE_DOC );
	else
		setSharedFileTypes( getSharedFileTypes() & ~VXFILE_TYPE_DOC );
}

//============================================================================
bool	VxSearchFlags::hasArcOrCdFiles( void )					
{ 
	return (VXFILE_TYPE_ARCHIVE_OR_CDIMAGE & getSharedFileTypes()) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasArcOrCdImageFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSharedFileTypes( getSharedFileTypes() | VXFILE_TYPE_ARCHIVE_OR_CDIMAGE );
	else
		setSharedFileTypes( getSharedFileTypes() & ~VXFILE_TYPE_ARCHIVE_OR_CDIMAGE );
}

//============================================================================
//! return true if has Other files
bool	VxSearchFlags::hasOtherFiles( void )					
{ 
	return (VXFILE_TYPE_OTHER & getSharedFileTypes()) ? true : false; 
}

//============================================================================
//! set has audio files
void	VxSearchFlags::setHasOtherFiles( bool bHasFiles )				
{
	if( bHasFiles )
		setSearchFlags( getSharedFileTypes() | VXFILE_TYPE_OTHER );
	else
		setSearchFlags( getSharedFileTypes() & ~VXFILE_TYPE_OTHER );
}

//============================================================================
//! return true if user has set profile picture
bool	VxSearchFlags::hasProfilePicture( void )					
{ 
	return (RC_FLAG_HAS_PROFILE_PIC & getSearchFlags()) ? true : false; 
}

//============================================================================
//! set has profile picture
void	VxSearchFlags::setHasProfilePicture( bool bHasPicture )				
{
	if( bHasPicture )
		setSearchFlags( getSearchFlags() | RC_FLAG_HAS_PROFILE_PIC );
	else
		setSearchFlags( getSearchFlags() & ~RC_FLAG_HAS_PROFILE_PIC );
}

//============================================================================
bool	VxSearchFlags::hasSharedWebCam( void )					
{ 
	return (RC_FLAG_HAS_SHARED_WEBCAM & getSearchFlags() ) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasSharedWebCam( bool bHasWebCam )				
{
	if( bHasWebCam )
		setSearchFlags( getSearchFlags() | RC_FLAG_HAS_SHARED_WEBCAM );
	else
		setSearchFlags( getSearchFlags() & ~RC_FLAG_HAS_SHARED_WEBCAM );
}

//============================================================================
bool	VxSearchFlags::hasModifiedStoryboard( void )					
{ 
	return ( RC_FLAG_HAS_STORY_BOARD & getSearchFlags() ) ? true : false; 
}

//============================================================================
void	VxSearchFlags::setHasModifiedStoryboard( bool bHasStoryBoard )				
{
	if( bHasStoryBoard )
		setSearchFlags( getSearchFlags() | RC_FLAG_HAS_STORY_BOARD );
	else
		setSearchFlags( getSearchFlags() & ~RC_FLAG_HAS_STORY_BOARD );
}


