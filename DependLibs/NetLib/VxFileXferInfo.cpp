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

#include "VxFileXferInfo.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
VxFileXferInfo::VxFileXferInfo( VxGUID& lclSessionId )
: m_LclSessionId( lclSessionId )
{
}

//============================================================================
VxFileXferInfo::~VxFileXferInfo()
{
	if( m_hFile )
	{
		fclose(m_hFile);
        m_hFile = nullptr;
	}
}

//============================================================================
bool VxFileXferInfo::calcProgress( void ) 
{ 
	int prevProgress = m_PercentProgress;
	if( 0 != m_u64FileLen ) 
	{
		m_PercentProgress = (int)((m_u64FileOffs * 100) / m_u64FileLen);
	}

	return ( m_PercentProgress != prevProgress ); 
}

//============================================================================
std::string VxFileXferInfo::getDownloadIncompleteFileName( void )
{
	return m_strLocalFileName;
}

//============================================================================
std::string VxFileXferInfo::getDownloadCompleteFileName( void )
{
    std::string completedFileName = eAssetTypeThumbnail == getAssetType() ? VxGetAppThumbnailDirectory() : VxGetDownloadsDirectory();
	std::string strPath;
	std::string strAssetNameOnly;
	RCODE rc = VxFileUtil::seperatePathAndFile(	m_strLocalFileName,			
												strPath,			
												strAssetNameOnly );	
	if( 0 == rc )
	{
		completedFileName += strAssetNameOnly;
	}

	return completedFileName;
}
