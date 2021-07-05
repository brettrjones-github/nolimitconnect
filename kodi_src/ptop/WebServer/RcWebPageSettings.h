#pragma once
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

#include <string>

#ifdef USE_WEB_FILESHARE
#define		MAX_MY_URLS					20		// maximum urls that can be listed
#define		MAX_MY_INTRO_LEN			4095	// maximum length of introduction

// definitions for website icons/images
#define		TC_WEB_UP_DIR_ICON			"images/up_a_directory16x16.png"
#define		TC_WEB_DN_DIR_ICON			"images/folder16x16.png"
#define		TC_WEB_FILE_ICON			"images/file_icon16x16.png"
#define		TC_WEB_BLANK_ICON			"images/blank16x16.png"

#define WEB_MY_TC_PICTURE_ALIAS		"my_tc_picture/"
//! get web site alias for shared files folder
_inline const char * GetWbMyFilesAlias( EWbMyFiles eMyFileType )
{
	switch( eMyFileType )
	{
	case eWbMyFilesImages:		return "my_tc_images/";
	case eWbMyFilesMusic:		return "my_tc_music/";
	case eWbMyFilesVideos:		return "my_tc_videos/";
	case eWbMyFilesOther:		return "my_tc_other/";
	default:					return "default_tc_err/";
	}
}
//! get default title for shared files folder
_inline const char * GetWbMyFilesDefaultTitle( EWbMyFiles eMyFileType )
{
	switch( eMyFileType )
	{
	case eWbMyFilesImages:		return "My Images";
	case eWbMyFilesMusic:		return "My Music";
	case eWbMyFilesVideos:		return "My Videos";
	case eWbMyFilesOther:		return "My Other Files";
	default:					return "Files Title ERROR";
	}
}


class WbMyFilesSettings
{
public:
	//=== constructor ===//
	WbMyFilesSettings();

	//! return true if file share is enabled
	bool		IsMyFilesEnabled( void )							{ return m_bEnableMyFiles; }
	//! set file share enable/disable
	void		SetMyFilesEnable( bool bEnable )					{  m_bEnableMyFiles = bEnable; }

	//! get shared files title
	char *		GetMyFilesTitle( void )								{ return m_as8MyFilesTitle; }
	//! set shared files directory
	void		SetMyFilesTitle( const char * pTitle )				{ SafeStrCopy( m_as8MyFilesTitle, pTitle, sizeof( m_as8MyFilesTitle ) ); }

	//! get shared files alias
	char *		GetMyFilesAlias( void )								{ return m_as8MyFilesAlias; }
	//! set shared files alias
	void		SetMyFilesAlias( const char * pAlias )				{ SafeStrCopy( m_as8MyFilesAlias, pAlias, sizeof( m_as8MyFilesAlias ) ); }

	//! get shared files directory
	char *		GetMyFilesDir( void )								{ return m_as8MyFilesDir; }
	//! set shared files directory
	void		SetMyFilesDir( const char * pDir )					{ SafeStrCopy( m_as8MyFilesDir, pDir, sizeof( m_as8MyFilesDir ) ); }

	//! return true if files are password protected
	bool		IsMyFilesPwdProtected( void )						{ return m_bEnableMyFilesPwd; }
	//! return true if files are password protected
	void		SetMyFilesPwdEnable( bool bEnable )					{ m_bEnableMyFilesPwd = bEnable; }

	//! get shared files pwd
	char *		GetMyFilesPwd( void )								{ return m_as8MyFilesPwd; }
	//! set shared files directory
	void		SetMyFilesPwd( const char * pPwd )					{ SafeStrCopy( m_as8MyFilesPwd, pPwd, sizeof( m_as8MyFilesPwd ) ); }

	//! verify the password is correct 
	bool		VerifyPwd( const char * pPwd )						{ return ( 0 == strcmp(pPwd, m_as8MyFilesPwd))?1:0; };


	//=== vars ===//
	bool				m_bEnableMyFiles;					// if true enable sharing of files
	char				m_as8MyFilesTitle[ VX_MAX_PATH ];		// title of shared files
	char				m_as8MyFilesDir[ VX_MAX_PATH ];		// directory of shared files
	char				m_as8MyFilesAlias[ VX_MAX_PATH ];		// website alias for directory of shared files
	bool				m_bEnableMyFilesPwd;				// if true enable password
	char				m_as8MyFilesPwd[ 64 ];				// shared files password
	char				m_as8MyFilesRes[ 1024 ];			// reserved for future use
};

class RcWebPageSettings
{
public:
	//=== vars ===//
	bool				m_bEnableServer;					// if true completely disable server
	char				m_as8WebPageDir[ VX_MAX_PATH ];		// root directory of web page
	//=== redirect ===//
	bool				m_bRedirect;						// if true redirect browser to another site
	char				m_as8RedirectUrl[ VX_MAX_PATH ];		// redirect to this url

	//=== introduction ===//
	bool				m_bEnableMyIntro;					// if true show introduction
	char				m_as8MyIntroText[MAX_MY_INTRO_LEN + 1];	// user introduction text

	//=== links ===//
	bool				m_bEnableMyUrls;					// if true show favorite urls
	char				m_as8MyUrlsTitle[ VX_MAX_PATH ];		// title for favorite urls
	char				m_as8MyUrls[MAX_MY_URLS][VX_MAX_PATH];	// user favorite urls

	//=== my picture ===//
	bool				m_bEnableMyPicture;					// if true show favorite picture
	char				m_as8MyPictureTitle[ VX_MAX_PATH ];	// title of picture
	char				m_as8MyPictureRealFile[ VX_MAX_PATH ];		// real picture file
	char				m_as8MyPictureWebFile[ VX_MAX_PATH ];		// real picture file

	//=== my files ===//
	WbMyFilesSettings	m_aoMyFiles[ eMaxAllWbMyFiles ];


	std::string			m_strResourseDir;
	std::string			m_strWebSiteDir;
	//=== reserved for future use ===//
	char				m_as8WbRes[ 4096 ];

	//=== constructor ===//
	RcWebPageSettings();

	//! save settings to file
	RCODE		SaveWbSettings( void );
	//! load settings from file
	RCODE		LoadWbSettings( void );
	//! set defaults
	RCODE		SetWbSettingsDefaults( void );

	//! return true if server is enabled
	bool		GetServerEnable( void )								{ return m_bEnableServer; }
	//! set server enabled/disable
	void		SetServerEnable( bool bEnable )						{  m_bEnableServer = bEnable; }


	//! get root directory of web page 
	char *		GetWebPageDir( void )								{ return m_as8WebPageDir; }
	//! set root directory of web page 
	void		SetWebPageDir( const char * pDir )					{ SafeStrCopy( m_as8WebPageDir, pDir, sizeof( m_as8WebPageDir ) ); }
	//! get directory of web page resources 
	std::string&		GetResourceDir( void )						{ return m_strResourseDir; }
	//! get directory of web page resources 
	std::string&		GetWebSiteDir( void )						{ return m_strWebSiteDir; }


	//=== redirect ===//
	//! return true if redirect is enabled
	bool		IsRedirectEnabled( void )							{ return m_bRedirect; }
	//! set redirect enable/disable
	void		SetRedirectEnable( bool bEnable )					{  m_bRedirect = bEnable; }
	//! get redirect url
	char *		GetRedirectUrl( void )								{ return m_as8RedirectUrl; }
	//! set redirect url
	void		SetRedirectUrl( const char * pUrl )					{ SafeStrCopy( m_as8RedirectUrl, pUrl, sizeof( m_as8RedirectUrl ) ); }

	//=== introduction ===//
	//! return true if my introduction is enabled
	bool		IsMyIntroEnabled( void )							{ return m_bEnableMyIntro; }
	//! set my introduction enable/disable
	void		SetMyIntroEnable( bool bEnable )					{  m_bEnableMyIntro = bEnable; }
	//! get contents of introduction
	char *		GetMyIntroText( void )								{ return m_as8MyIntroText; }
	//! set contents of introduction
	void		SetMyIntroText( const char * pIntro )				{ SafeStrCopy( m_as8MyIntroText, pIntro, sizeof( m_as8MyIntroText ) ); }

	//=== my picture ===//
	//! return true if my picture is enabled
	bool		IsMyPictureEnabled( void )							{ return m_bEnableMyPicture; }
	//! set my picture enable/disable
	void		SetMyPictureEnable( bool bEnable )					{  m_bEnableMyPicture = bEnable; }
	//! get title my picture
	char *		GetMyPictureTitle( void )							{ return m_as8MyPictureTitle; }
	//! set my picture title
	void		SetMyPictureTitle( const char * pTitle )			{ SafeStrCopy( m_as8MyPictureTitle, pTitle, sizeof( m_as8MyPictureTitle ) ); }
	//! get my picture file
	char *		GetMyPictureRealFile( void )						{ return m_as8MyPictureRealFile; }
	//! set my picture file
	void		SetMyPictureRealFile( const char * pFileName );

	//! get my picture file as link in website
	char *		GetMyPictureWebFile( void )							{ return m_as8MyPictureWebFile; }

	//=== my urls ===//
	//! return true if my urls is enabled
	bool		IsMyUrlsEnabled( void )								{ return m_bEnableMyUrls; }
	//! set my urls enable/disable
	void		SetMyUrlsEnable( bool bEnable )						{  m_bEnableMyUrls = bEnable; }
	//! get title my urls
	char *		GetMyUrlTitle( void )								{ return m_as8MyUrlsTitle; }
	//! set my urls url
	void		SetMyUrlTitle( const char * pTitle )				{ SafeStrCopy( m_as8MyUrlsTitle, pTitle, sizeof( m_as8MyUrlsTitle ) ); }
	//! get my urls url
	char *		GetMyUrl( int iIdx )								{ vx_assert( iIdx < MAX_MY_URLS ); return m_as8MyUrls[iIdx]; }
	//! set my urls url
	void		SetMyUrl( int iIdx, const char * pUrl )					{ vx_assert( iIdx < MAX_MY_URLS ); SafeStrCopy( m_as8MyUrls[iIdx], pUrl, VX_MAX_PATH ); }

	//=== shared files ===//
	//! return true if shared files of given type is enabled
	bool		IsMyFilesEnabled( EWbMyFiles eWbFilesType )				{ return m_aoMyFiles[ eWbFilesType ].IsMyFilesEnabled(); }
	//! enable/disable sharing of files of given type
	bool		SetMyFilesEnable( EWbMyFiles eWbFilesType, bool bEnable )	{ m_aoMyFiles[ eWbFilesType ].SetMyFilesEnable( bEnable ); }
	//! get title shared files
	char *		GetMyFilesTitle( EWbMyFiles eWbFilesType )				{ return m_aoMyFiles[ eWbFilesType ].GetMyFilesTitle(); }
	//! set my files title
	void		SetMyFilesTitle( EWbMyFiles eWbFilesType, const char * pTitle ){ m_aoMyFiles[ eWbFilesType ].SetMyFilesTitle(pTitle); };
	//! get alias for my files directory
	char *		GetMyFilesAlias( EWbMyFiles eWbFilesType )				{ return m_aoMyFiles[ eWbFilesType ].GetMyFilesAlias(); };
	//! set alias for my files directory
	void		SetMyFilesAlias( EWbMyFiles eWbFilesType, const char * pAlias ){ m_aoMyFiles[ eWbFilesType ].SetMyFilesAlias(pAlias); };
	//! get my files directory
	char *		GetMyFilesDir( EWbMyFiles eWbFilesType )				{ return m_aoMyFiles[ eWbFilesType ].GetMyFilesDir(); };
	//! set my files directory
	void		SetMyFilesDir(  EWbMyFiles eWbFilesType, const char * pFileName ){ m_aoMyFiles[ eWbFilesType ].SetMyFilesDir(pFileName); };

	//! return true if files are password protected
	bool		IsMyFilesPwdProtected( EWbMyFiles eWbFilesType )		{ return m_aoMyFiles[ eWbFilesType ].IsMyFilesPwdProtected(); }
	//! return true if files are password protected
	bool		SetMyFilesPwdEnable( EWbMyFiles eWbFilesType, bool bEnable ){ m_aoMyFiles[ eWbFilesType ].SetMyFilesPwdEnable( bEnable ); }
	//! get my files password
	char *		GetMyFilesPwd( EWbMyFiles eWbFilesType )				{ return m_aoMyFiles[ eWbFilesType ].GetMyFilesPwd(); };
	//! set my files password
	void		SetMyFilesPwd(  EWbMyFiles eWbFilesType, const char * pPwd ){ m_aoMyFiles[ eWbFilesType ].SetMyFilesPwd(pPwd); };

	//! verify the password is correct for the given file share type
	bool		VerifyPwd( EWbMyFiles eWbFilesType, const char * pPwd )		{ return m_aoMyFiles[ eWbFilesType ].VerifyPwd(pPwd); };


	//! get group of shared files setting
	WbMyFilesSettings *	GetMyFilesSetting( EWbMyFiles eWbFilesType )	{ return &m_aoMyFiles[ eWbFilesType ]; };



};
#else
class RcWebPageSettings
{
public:
	//=== vars ===//
	std::string			m_strWebSiteDir;

};
#endif //USE_WEB_FILESHARE
