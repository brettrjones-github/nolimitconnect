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
// http://www.nolimitconnect.org
//============================================================================
#include "RcWebServer.h"

#ifdef USE_WEB_FILESHARE
//! thread function to build websit
static void * BuildWebSiteThreadFunc( void * pvContext )
{
	VxThread * poThread = (VxThread *)pvContext;
	poThread->setIsThreadRunning( true );
	RcWebPageBuilder * poBuilder = (RcWebPageBuilder *)poThread->getThreadUserParam();

	poBuilder->m_Mutex.lock( __FILE__, __LINE__ );
	RCODE rc = poBuilder->DoBuildWebSite();
	poBuilder->m_Mutex.unlock( __FILE__, __LINE__ );

	if( 0 == rc )
	{
		poBuilder->m_bBuildWebSiteSuccess = true;
		poBuilder->webSiteReadyCallback( 0 );
	}
	else
	{
		poBuilder->m_rcLastBuildErr = rc;
		poBuilder->m_bBuildWebSiteFail = true;
	}

	//! Thread calls this just before exit
	poThread->threadAboutToExit();
    return nullprt;
}

//============================================================================
//=== constructor ===//
RcWebPageBuilder::RcWebPageBuilder()
	: m_bBuildWebSiteSuccess(0)
	, m_bBuildWebSiteFail(0)
	, m_rcLastBuildErr(0)
	, m_iBuildProgress(0)
{
	// empty
}
//============================================================================
//! make html accepatable name.. replace spaces with %20
char * RcWebPageBuilder::MakeHtmlAcceptableName( const char * pName )
{
	size_t iLen = strlen( pName ) + 1;
	vx_assert( iLen < VX_MAX_PATH );
static char as8Buf[ VX_MAX_PATH * 2 ];
	int iBufIdx = 0;
	for( size_t i = 0; i < iLen; i++ )
	{
		if( ' ' == pName[i] )
		{
			as8Buf[ iBufIdx++ ] = '%';
			as8Buf[ iBufIdx++ ] = '2';
			as8Buf[ iBufIdx++ ] = '0';
		}
		else
		{
			as8Buf[ iBufIdx++ ] = pName[i];
		}
	}
	return as8Buf;
}
//============================================================================
//! format file length into 4 character string
void  RcWebPageBuilder::FormatFileSize( uint64_t u64FileSize, char * pBuf )
{
	double dSize = (double)u64FileSize;
	char as8Buf[32];
	char as8Suffix[32];
	if( u64FileSize < 1000 )
	{
		sprintf( as8Buf, "%d", (uint32_t)u64FileSize );
		strcpy( as8Suffix, "  B" );
	}
	else if( u64FileSize < 1000000 )
	{
		sprintf( as8Buf, "%3.1f", dSize/1000 );
		strcpy( as8Suffix, " KB" );
	}
	else if( u64FileSize < 1000000000 )
	{
		sprintf( as8Buf, "%3.1f", dSize/1000000 );
		strcpy( as8Suffix, " MB" );
	}
	else if( u64FileSize < 1000000000000 )
	{
		sprintf( as8Buf, "%3.1f", dSize/1000000000 );
		strcpy( as8Suffix, " GB" );
	}
	else if( u64FileSize < 1000000000000000 )
	{
		sprintf( as8Buf, "%3.1f", dSize/1000000000000 );
		strcpy( as8Suffix, " TB" );
	}
	else if( u64FileSize < 1000000000000000000 )
	{
		sprintf( as8Buf, "%3.1f", dSize/1000000000000000 );
		strcpy( as8Suffix, " PB" );
	}
	else
	{
		strcpy( as8Buf, "???" );
		strcpy( as8Suffix, " ??" );
	}
	pBuf[0] = 0;
	size_t iLen = strlen( as8Buf );
	if( iLen > 4 )
	{
		// too long.. remove the decimal point
		char * pTemp = strchr( as8Buf, '.' );
		if( pTemp )
		{
			pTemp[0] = pTemp[2];
			pTemp[1] = 0;
		}
		iLen = strlen( as8Buf );
	}
	if( iLen < 4 )
	{
		// fill in leading spaces
		for( size_t i = 0; i < ( 4 - iLen ); i++ )
		{
			pBuf[i] = ' ';
			pBuf[i+1] = 0;
		}
	}
	strcat( pBuf, as8Buf );
	strcat( pBuf, as8Suffix );
}

//============================================================================
//! rebuild web site
RCODE RcWebPageBuilder::BuildWebSite( void )
{
	m_iBuildProgress = 1;
	m_bBuildWebSiteSuccess = false;
	m_bBuildWebSiteFail = false;
	m_rcLastBuildErr = 0;

	/*
	if( ( false == fileExists( g_strSettingsFile.c_str() ) ) ||
		LoadWbSettings() )
	{
		SetWbSettingsDefaults();
		SaveWbSettings();
	}
	*/
	return m_BuildThread.startThread( (VX_THREAD_FUNCTION_T)BuildWebSiteThreadFunc, this, "WebBuilderThread" );
}
#define HTML_WEBPAGE_HDR "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\
<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=iso-8859-1\">\
<FONT COLOR=\"#0000FF\">\
<title>A Tronacom Web Site</title>\
<A href=\"http://www.tronacom.com\">A Tronacom Web Site.</A>\
</FONT>\
</head><body bgcolor=\"#E3EFFF\">"

#define HTML_WEBPAGE_FOOTER "</body>\r\n</html>"

#define HTML_REDIRECT_CONTENT "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\
\"<html><head><meta HTTP-EQUIV=\"REFRESH\" content=\"0; url=%s\"></head><body>"

//============================================================================
//! rebuild web site called from thread
RCODE RcWebPageBuilder::DoBuildWebSite( void )
{
	m_iBuildProgress = 2;
	RCODE rc = 0;
	char * pTemp;
	/*
	if( ( false == fileExists( g_strSettingsFile.c_str() ) ) ||
		LoadWbSettings() )
	{
		SetWbSettingsDefaults();
		SaveWbSettings();
	}
	*/
	std::string strWebRoot = GetWebPageDir();
	makeDirectory( strWebRoot.c_str() );

	std::string strImages = strWebRoot + "images/";
	makeDirectory( strImages.c_str() );

	// up a directory icon
	std::string strWebIcon			= strWebRoot + TC_WEB_UP_DIR_ICON;
	std::string strResourceIcon		= GetResourceDir() + TC_WEB_UP_DIR_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// down a directory icon
	strWebIcon			= strWebRoot + TC_WEB_DN_DIR_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_DN_DIR_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// file icon
	strWebIcon			= strWebRoot + TC_WEB_FILE_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_FILE_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );

	// blank icon
	strWebIcon			= strWebRoot + TC_WEB_BLANK_ICON;
	strResourceIcon		= GetResourceDir() + TC_WEB_BLANK_ICON;
	vx_assert( fileExists( strResourceIcon.c_str() ) );
	copyFile( strResourceIcon.c_str(), strWebIcon.c_str() );




	std::string strIdxFile = strWebRoot +  "tc_main_index.htm";
	FILE * poFile = fopen( strIdxFile.c_str(), "wb+" );
	if( NULL == poFile )
	{
		// could not open index file
		rc = VxGetLastError();		
		LogMsg( LOG_INFO, "RcWebPageBuilder::DoBuildWebSite: error %d opening file %s\n", rc, strIdxFile.c_str() );
		goto error_exit;
	}
	if( IsRedirectEnabled() )
	{
		// redirect user to another site
		fprintf( poFile, HTML_REDIRECT_CONTENT, GetRedirectUrl());
		goto success_exit;
	}
	// write out the header
	size_t iLen = strlen( HTML_WEBPAGE_HDR );
	if( iLen != fwrite( HTML_WEBPAGE_HDR, 1, iLen, poFile ) )
	{
		rc = VxGetLastError();		
		LogMsg( LOG_INFO, "RcWebPageBuilder::DoBuildWebSite: error %d writing file %s\n", rc, strIdxFile.c_str() );
		goto error_exit;
	}
	m_iBuildProgress = 3;
	if( IsMyIntroEnabled() )
	{
		fprintf( poFile, "<p style=\"text-align: center;\">%s</p>\n", GetMyIntroText() );
	}
	m_iBuildProgress = 4;
	if( IsMyPictureEnabled() )
	{
		fprintf( poFile, "<p style=\"text-align: center;\">%s</p>\n", GetMyPictureTitle() );
		fprintf( poFile, "<p ALIGN=CENTER><IMG SRC = \"%s\"></p>\n", GetMyPictureWebFile() );
	}
	m_iBuildProgress = 5;
	if( IsMyUrlsEnabled() )
	{
		fprintf( poFile, "<p>%s<br>", GetMyUrlTitle() );
		fprintf(poFile, "<FONT COLOR=\"#0000FF\">" );
		for( int i = 0; i < MAX_MY_URLS; i++ )
		{
			pTemp = GetMyUrl( i );
			if( pTemp &&
				pTemp[0] )
			{
				
				fprintf( poFile, "<a href=\"%s\">%s</a><br>", pTemp, pTemp );
			}
		}
		fprintf( poFile, "</FONT>" );
		fprintf( poFile, "</p>\n" );
	}
	//=== shared files ===//
	for( int i = 1; i < eWbMyFilesError; i++ )
	{
		if( false == IsMyFilesEnabled( (EWbMyFiles)i ) )
		{
			continue;
		}
		std::string strMyFilesTitle		= GetMyFilesTitle( (EWbMyFiles)i );

		std::string strMyFilesAlias		= "/";
		strMyFilesAlias					= strMyFilesAlias + GetMyFilesAlias( (EWbMyFiles)i );

		std::string strMyFilesRealDir	= GetMyFilesDir( (EWbMyFiles)i );
		// make a link to the first index in the website
		std::string strMyFilesWebIdx = strMyFilesAlias + "tc_index.htm";
		strMyFilesWebIdx = MakeHtmlAcceptableName( strMyFilesWebIdx.c_str() );

		fprintf( poFile, "<a href=\"%s\">%s</a><br>", strMyFilesWebIdx.c_str(), strMyFilesTitle.c_str() );
	
		strMyFilesTitle = strMyFilesTitle + "/";

		std::string strMyFilesWebDir = GetWebSiteDir() + GetMyFilesAlias( (EWbMyFiles)i );


		BuildFileIndex( strMyFilesTitle,	// title of files
						strMyFilesAlias,	// alias directory ( releative to website root )
						strMyFilesWebDir,	// website directory
						strMyFilesRealDir,	// shared files directory
						"/tc_main_index.htm" );	// parent index file

	}

success_exit:
	fprintf( poFile, HTML_WEBPAGE_FOOTER );
	fclose( poFile );
	m_iBuildProgress = 100;
	webSiteReadyCallback( 0 );
	return 0;
error_exit:
	if( poFile )
	{
		fclose( poFile );
	}
	LogMsg( LOG_INFO, "RcWebPageBuilder::DoBuildWebSite: error %d\n", rc );
	LogMsg(  0, "RcWebPageBuilder::DoBuildWebSite: error %d\n", rc );
	return -1;
}

#define WEB_IDX_HDR "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\r\n\
<html><head><title>Index of %s</title></head><body bgcolor=\"#E3EFFF\">\r\n<H1>Index of %s</H1>\r\n\
<PRE><image src=\"/images/blank16x16.png\"> <A>  Size  </A> <A> Name  </A><HR>\r\n\
<image src=\"/images/up_a_directory16x16.png\"> <A>  ---    </A> <A HREF=\"%s\">Parent Directory</A>\r\n"

#define WEB_IDX_FOOTER "</PRE><HR></body></html>\r\n"


//============================================================================
//! build html indexes of shared files
RCODE RcWebPageBuilder::BuildFileIndex(		std::string strMyFilesTitle,			// title of files
											std::string strMyFilesAlias,			// alias directory ( releative to website root )
											std::string strMyFilesWebDir,			// website directory
											std::string strMyFilesRealDir,			// shared files directory
											std::string strMyFilesParentIdxFile )	// parent index file
{
	if( m_BuildThread.isAborted() )
	{
		return 0;
	}
	RCODE rc;
	makeDirectory( strMyFilesWebDir.c_str() );
	std::string strCurIdxFile = strMyFilesWebDir + "tc_index.htm";
	FILE * poFile = fopen( strCurIdxFile.c_str(), "wb+" );
	if( NULL == poFile )
	{
		// could not create index file
		rc = VxGetLastError();
		LogMsg( TC_WEB_ERR_MSG, "RcWebPageBuilder::BuildFileIndex:Error %d creating file %s\n", rc, strCurIdxFile.c_str() );
		return rc;
	}
	strMyFilesParentIdxFile = MakeHtmlAcceptableName( strMyFilesParentIdxFile.c_str() );
	// write out header with title and link to parent index
	fprintf( poFile, WEB_IDX_HDR, strMyFilesTitle.c_str(), strMyFilesTitle.c_str(), strMyFilesParentIdxFile.c_str() );
	// make new parent index file
	strMyFilesParentIdxFile = strMyFilesAlias + "tc_index.htm";

	// recusively search the directory for files and sub directories
	std::string csSearchPath = strMyFilesRealDir;
#ifdef TARGET_OS_WINDOWS
    HANDLE			hFind			= INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW	sFindData;
	std::string		strCurFileNode;
    if( 0 != csSearchPath.size ( ) )
    {
		strCurFileNode = csSearchPath + "*.*";
        hFind = FindFirstFileW( strCurFileNode.c_str(),
                                &sFindData );
        if( INVALID_HANDLE_VALUE == hFind )
        {
			// could not do find file
			rc = VxGetLastError();
			LogMsg( TC_WEB_ERR_MSG, "RcWebPageBuilder::BuildFileIndex:Error %d searching dir %s\n", rc, csSearchPath.c_str() );
			goto finalize_exit;
		}
        do
        {
			// look at file node attributes
            strCurFileNode = csSearchPath + sFindData.cFileName;
            if ( ( 0 != (sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) )
            {
				// its a directory
			    if( ( 0 != strcmp ( ".", sFindData.cFileName ) )	&&
			        ( 0 != strcmp ( "..", sFindData.cFileName ) )	) 
			    {
					std::string strDirName = sFindData.cFileName;

					// make html acceptable href link to directory index
					std::string strHrefDirName = strMyFilesAlias + sFindData.cFileName;
					strHrefDirName = strHrefDirName + "/tc_index.htm";
					strHrefDirName = MakeHtmlAcceptableName( strHrefDirName.c_str() );

					fprintf( poFile, "<IMG SRC=\"/images/folder16x16.png\">   ---     <A HREF=\"%s\">%s</A>\r\n", 
									strHrefDirName.c_str(),
									strDirName.c_str() );

					// recurse down a directory
					std::string strRecurseTitle = strMyFilesTitle + sFindData.cFileName;	// title of files
					strRecurseTitle = strRecurseTitle + "/";

					std::string strRecurseAlias = strMyFilesAlias + sFindData.cFileName;	// title of files
					strRecurseAlias = strRecurseAlias + "/";

					std::string strRecurseWebDir = strMyFilesWebDir + sFindData.cFileName;	// title of files
					strRecurseWebDir = strRecurseWebDir + "/";

					std::string strRecurseRealDir = strMyFilesRealDir + sFindData.cFileName;	// title of files
					strRecurseRealDir = strRecurseRealDir + "/";

					// do recursive call
					BuildFileIndex(		strRecurseTitle, 		// title of files
										strRecurseAlias,		// alias directory ( releative to website root )
										strRecurseWebDir,		// website directory
										strRecurseRealDir,		// shared files directory
										strMyFilesParentIdxFile );		// parent index file
			    }
            }
		    else
		    {
				// its a file
				// ignore hidden and system and temporary files
				if( ( 0 == ((FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_TEMPORARY) & sFindData.dwFileAttributes ) ) )
				{
					// get file size
					uint64_t u64FileLen = (uint64_t)sFindData.nFileSizeHigh << 32;
					u64FileLen += sFindData.nFileSizeLow;
					if( 0 != u64FileLen )
					{
						std::string strFileName = sFindData.cFileName;

						// make html acceptable href link to directory index
						std::string strHrefFileName = strMyFilesAlias + strFileName;
						strHrefFileName = MakeHtmlAcceptableName( strHrefFileName.c_str() );

						// format file size
						char as8FileLenBuf[ 16 ];
						FormatFileSize( u64FileLen, as8FileLenBuf );

						// make an line in the index file
						fprintf( poFile, "<image src=\"/images/file_icon16x16.png\"> <A> %s </A> <A HREF=\"%s\">%s</A>\r\n",
									as8FileLenBuf,
									strHrefFileName.c_str(),
									strFileName.c_str() );
					}
				}
		    }
        }
        while( ( false == m_BuildThread.isAborted() ) &&
                ( FindNextFileW( hFind, &sFindData ) ) );
        FindClose ( hFind );
    }
#else 
	// linux version of find files
	// find the files in the directory
	DIR *pDir;
	struct dirent *pFileEnt;
	if( directoryExists( as8SrcDir ) )
	{
		LogMsg( LOG_INFO, "VxListSubDirectories: directory %s exists.. opening dir\n", as8SrcDir );
		//ok directory exists!
		if(!(NULL == (pDir = opendir(as8SrcDir))))
		{
			//pDir is open
			while( 0 != (pFileEnt = readdir(pDir)))
			{
				LogMsg( LOG_INFO, "VxListSubDirectories: found file %s\n", pFileEnt->d_name );
				//got a file or directory
				if( '.' == pFileEnt->d_name[ 0 ] )//||
			//			(DT_LNK == pFileEnt->d_type )  ||
			//			(DT_UNKNOWN == pFileEnt->d_type ) )
				{
					// skip . and ..
					LogMsg( LOG_INFO, "VxListSubDirectories: skipping file %s\n", pFileEnt->d_name );
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
				struct stat m;
				if( 0 != stat( as8SrcFile, &m) )
				{
					///ERROR how do we handle
					LogMsg( LOG_INFO, "VxListSubDirectories: Unable to stat file %s\n", as8SrcFile );
					continue;
				}
				if( pFileEnt->d_type == DT_DIR )
				{
					// its a directory
					LogMsg( LOG_INFO, "VxListSubDirectories: file %s is directory\n", as8SrcFile );
					char * pName = new char[ strlen( pFileEnt->d_name ) + 1 ];
					vx_assert( pName );
					strcpy( pName, pFileEnt->d_name );
					poNode = new VxNode( pName );
					vx_assert( poNode );
					poRetList->AddHead( poNode );
				}
				else
				{
					// its a file
					continue;
				}
			}
			// end of listing
			// done with listing
			closedir(pDir);
			return 0;
		}
		else
		{
			LogMsg( LOG_INFO, "VxListSubDirectories: could not open directory %s \n", as8SrcDir );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "VxListSubDirectories: directory %s does not exist \n", as8SrcDir );
	}
	return 0;
#endif // TARGET_OS_WINDOWS

finalize_exit:
	fprintf( poFile, WEB_IDX_FOOTER );
	fclose( poFile );
	return 0;
}
#endif // USE_WEB_FILESHARE
