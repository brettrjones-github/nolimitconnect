//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "RcWebServer.h"

#ifdef USE_WEB_FILESHARE
//============================================================================
RcWebPageSettings::RcWebPageSettings()
{
	memset( (RcWebPageSettings *)this, 0, sizeof( RcWebPageSettings ) );
}

//============================================================================
WbMyFilesSettings::WbMyFilesSettings()
{
	memset( (WbMyFilesSettings *)this, 0, sizeof( WbMyFilesSettings ) );
}

//============================================================================
//! save settings to file
RCODE RcWebPageSettings::SaveWbSettings( void )
{
	//return writeWholeFile( g_strSettingsFile.c_str(), this, sizeof( RcWebPageSettings ) );
	return 0;
}
//============================================================================
//! load settings from file
RCODE RcWebPageSettings::LoadWbSettings( void )
{
	//return readWholeFile( g_strSettingsFile.c_str(), this, sizeof( RcWebPageSettings ) );
	return 0;
}
//============================================================================
//! set defaults
RCODE RcWebPageSettings::SetWbSettingsDefaults( void )
{
	memset( this, 0, sizeof( RcWebPageSettings ) );

	// copy resource icons to web site
	std::string strImageDir = GetWebSiteDir() + "images/";
	makeDirectory( strImageDir.c_str() );

	// up a directory icon
	std::string strWebIcon			= GetWebSiteDir() + TC_WEB_UP_DIR_ICON;
	std::string strResourceIcon		= GetResourceDir() + TC_WEB_UP_DIR_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// down a directory icon
	strWebIcon			= GetWebSiteDir() + TC_WEB_DN_DIR_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_DN_DIR_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// file icon
	strWebIcon			= GetWebSiteDir() + TC_WEB_FILE_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_FILE_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// blank icon
	strWebIcon			= GetWebSiteDir() + TC_WEB_BLANK_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_BLANK_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );



	// setup default settings


	SetServerEnable( true );
	SetWebPageDir( GetWebSiteDir().c_str() );

	SetRedirectUrl( "http://www.myspace.com" );

	SetMyIntroEnable( true );
	SetMyIntroText( "Hi!.. Let's Communicate!" );

	SetMyUrlsEnable( true );
	SetMyUrlTitle( "My Favorite Web Sites." );
	SetMyUrl( 0, "http://www.nolimitconnect.com" );

	SetMyPictureEnable( true );
	SetMyPictureTitle( "A Picture Of Me" );
	std::string strPathsDir = GetWebSiteDir() + WEB_MY_TC_PICTURE_ALIAS;
	makeDirectory( strPathsDir.c_str() );

	std::string strImageFile = GetResourceDir() + "images/me.jpg";
	vx_assert( fileExists( strImageFile.c_str() ) );
	SetMyPictureRealFile( strImageFile.c_str() );

	for( int i = 1; i < eMaxNormalWbMyFiles; i++ )
	{
		SetMyFilesAlias(  ( EWbMyFiles )i, GetWbMyFilesAlias( ( EWbMyFiles )i ) );
		SetMyFilesTitle(  ( EWbMyFiles )i, GetWbMyFilesDefaultTitle( ( EWbMyFiles )i ) );	
	}

	return 0;
}
//============================================================================
//! set my picture file
void RcWebPageSettings::SetMyPictureRealFile( const char * pFileName )
{
	/*
	vx_assert( fileExists( pFileName ) );
	std::string strFile;
	std::string strPath;
	seperatePathAndFile( pFileName, strPath, strFile );
	SafeStrCopy( m_as8MyPictureRealFile, pFileName, sizeof( m_as8MyPictureRealFile ) );
	strcpy( m_as8MyPictureWebFile, WEB_MY_TC_PICTURE_ALIAS );
	strcat( m_as8MyPictureWebFile, strFile.c_str() );
	*/
}

#endif // USE_WEB_FILESHARE