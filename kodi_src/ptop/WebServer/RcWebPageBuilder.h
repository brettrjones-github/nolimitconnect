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


class RcWebPageBuilder : public RcWebServer
{
public:
	//=== constructor ===//
	RcWebPageBuilder();

	//=== methods ===//
	//! shutdown web builder
	RCODE	ShutdownWebPageBuilder( void )
	{
		m_BuildThread.killThread();
		return 0;
	}
	//! rebuild web site
	RCODE	BuildWebSite( void );

	//! rebuild web site called from thread
	RCODE	DoBuildWebSite( void );

	//! get build website progress
	virtual int GetBuildProgress( void ){ return m_iBuildProgress; }

	//! called when website is ready
	//! non zero error if error occured
	virtual void webSiteReadyCallback( int iError ){ }

	//! build html indexes of shared files
	RCODE	BuildFileIndex(	std::string strMyFilesTitle,			// title of files
							std::string strMyFilesAlias,			// alias directory ( releative to website root )
							std::string strMyFilesWebDir,			// website directory
							std::string strMyFilesRealDir,			// shared files directory
							std::string strMyFilesParentIdxFile );	// parent index file

	//! make html accepatable name.. replace spaces with %20
	char *	MakeHtmlAcceptableName( const char * pName );

	//! format file length into 6 character string
	void	FormatFileSize( uint64_t u64FileSize, char * pBuf );


	//=== vars ===//
	bool				m_bBuildWebSiteSuccess;		// if true website build completed successfully
	bool				m_bBuildWebSiteFail;		// if true website build failed
	RCODE				m_rcLastBuildErr;			// last build error
	int					m_iBuildProgress;			// percent progress of build website

	uint64_t					m_u64TotalSharedBytes;		// number of bytes shared in shared files


	VxThread			m_BuildThread;		// thread to do website building
	VxMutex				m_Mutex;			// mutex for website access

	
};