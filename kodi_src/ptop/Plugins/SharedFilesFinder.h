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
#include <vector>
#include <CoreLib/VxDefs.h>
#include "SharedFileInfo.h"

class VxThread;

class SharedFileFinder
{
public:

	void						Abort( void )			{ m_bAbort = true; }

	//find files matching the extension (file type) list
	RCODE						FindFilesByExtension(	std::string					csPath,								//start path to search in
														std::vector<std::string> &	acsExtensionList,					//Extensions of files to find
														std::vector<SharedFileInfo> &	aoFileList,							//return FileInfo in array
														bool						bRecurse = false,					//recurse subdirectories if TRUE
														bool						bUseFilterListToExclude = false		//if TRUE dont return files matching filter else return files that do
														);
	//find files matching the extension (file type) list
	RCODE						FindFilesByName(	VxThread *						poThread,
													std::string						csPath,								//start path to search in
													std::vector<std::string> &		acsWildNameList,					//Wildcard Name match strings
													std::vector<SharedFileInfo> &	aoFileList,							//return FileInfo in array
													bool							bRecurse = false,					//recurse subdirectories if TRUE
													bool							bUseFilterListToExclude = false		//if TRUE dont return files matching filter else return files that do
													);
protected:
	bool						HasSameExtension(	std::string csCurrentNode,
													std::vector<std::string> &acsExtensionList );
	bool						HasMatchingName(	std::string csCurrentNode,
													std::vector<std::string> &acsWildNameList );

	bool						m_bAbort;
};

//! search for files
RCODE FindSharedFilesByExtension(	std::string					csPath,							//start path to search in
									std::vector<std::string>&	acsExtensionList,				//Extensions ( file extentions )
									std::vector<SharedFileInfo>&	aoFileList,						//return FileInfo in array
									bool						bRecurse = false,				//recurse subdirectories if TRUE
									bool						bUseFilterListToExclude = false	//if TRUE dont return files matching filter else return files that do
									);

RCODE FindSharedFilesByName(	VxThread *						poThread,
								std::string						csPath,							//start path to search in
								std::vector<std::string>&		acsWildNameList,				//filters
								std::vector<SharedFileInfo>&	aoFileList,						//return FileInfo in array
								bool							bRecurse = false,				//recurse subdirectories if TRUE
								bool							bUseFilterListToExclude = false	//if TRUE dont return files matching filter else return files that do
								);



