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

#include "VxFileUtil.h"

class VxFileFinder
{
public:
	bool m_bAbort;

	void Abort( void ){ m_bAbort = true; }

	//find files matching the extension (file type) list
	RCODE FindFilesByExtension(	std::string					csPath,								//start path to search in
								std::vector<std::string> &	acsExtensionList,					//Extensions of files to find
								std::vector<VxFileInfo> &	aoFileList,							//return FileInfo in array
								bool						bRecurse = false,					//recurse subdirectories if true
								bool						bUseFilterListToExclude = false		//if true dont return files matching filter else return files that do
								);
	//find files matching the extension (file type) list
	RCODE FindFilesByName(		std::string					csPath,								//start path to search in
								std::vector<std::string> &	acsWildNameList,					//Wildcard Name match strings
								std::vector<VxFileInfo> &	aoFileList,							//return FileInfo in array
								bool						bRecurse = false,					//recurse subdirectories if true
								bool						bUseFilterListToExclude = false		//if true dont return files matching filter else return files that do
								);
protected:
	bool HasSameExtension( std::string csCurrentNode,
							std::vector<std::string> &acsExtensionList );
	bool HasMatchingName( std::string csCurrentNode,
							std::vector<std::string> &acsWildNameList );
};

//! search for files
RCODE VxFindFilesByExtension(	std::string					csPath,							//start path to search in
								std::vector<std::string>&	acsExtensionList,				//Extensions ( file extentions )
								std::vector<VxFileInfo>&	aoFileList,						//return FileInfo in array
								bool						bRecurse = false,				//recurse subdirectories if true
								bool						bUseFilterListToExclude = false	//if true dont return files matching filter else return files that do
								);

RCODE VxFindFilesByName(	std::string					csPath,							//start path to search in
							std::vector<std::string>&	acsWildNameList,				//filters
							std::vector<VxFileInfo>&	aoFileList,						//return FileInfo in array
							bool						bRecurse = false,				//recurse subdirectories if true
							bool						bUseFilterListToExclude = false	//if true dont return files matching filter else return files that do
							);




