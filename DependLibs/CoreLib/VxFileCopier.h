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

class VxFileCopier
{
public:
	bool m_bAbort;

	void Abort( void ){ m_bAbort = true; }

	//find files matching the extension (file type) list
    RCODE                       copyDirectory(  std::string					curPath,                            // current file or path to copy
                                                std::string&				srcPath,						    // src path to copy from
                                                std::string&				destPath,       					// dest path to copy to
							                    std::vector<VxFileInfo> &	aoFileList,							// return FileInfo in array
							                    bool						bRecurse = false					// recurse subdirectories if true
							                    );
protected:
    RCODE                       substitutePath( std::string&		curPath,
                                                  std::string&		srcPath, 
                                                  std::string&		destPath, 
                                                  std::string&		resultPath );

    RCODE                       copyFileToDestDir( std::string& srcFileName, std::string& srcPath, std::string& destPath );

    RCODE                       createDestDirectoryIfNeeded( std::string&		curPath,
                                                             std::string&		srcPath,
                                                             std::string&		destPath );
};



