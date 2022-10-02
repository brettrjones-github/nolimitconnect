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

#include "VxFileInfo.h"

//============================================================================
bool VxIsPhotoFile( std::string& cs );
bool VxIsPhotoFileExtention( const char* pExt );
bool VxIsAudioFile( std::string& cs );
bool VxIsAudioFileExtention( const char* pExt );
bool VxIsVideoFile( std::string& cs );
bool VxIsVideoFileExtention( const char* pExt );
bool VxIsDocumentFile( std::string& cs );
bool VxIsDocumentFileExtention( const char* pExt );
bool VxIsArcOrCDImageFile( std::string& cs );
bool VxIsArcOrCDImageFileExtention( const char* pExt );
bool VxIsExecutableFile( std::string& cs );
bool VxIsExecutableFileExtention( const char* pExt );
bool VxIsThumbnailFile( std::string& cs );
bool VxIsThumbnailFileExtention( const char* pExt );

bool VxIsRecognizedFile( std::string& cs );
bool VxIsRecognizedFileExtention( const char* pExt );

bool VxIsShortcutFileExtention( const char* pExt );
bool VxIsShortcutFile( std::string& cs );

uint8_t	VxFileExtensionToFileTypeFlag( const char*	pFileExt );

bool VxIsMediaFile( uint8_t u8FileTypeFlag ); // multimedia
bool VxShouldOpenFile( uint8_t u8FileTypeFlag ); // includes docs
