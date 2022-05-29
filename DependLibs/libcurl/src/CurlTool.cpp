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

#include "CurlTool.h"
#include "CurlToolExe.h"

#include <CoreLib/VxParse.h>

CurlTool::CurlTool( ICurlCallback& curlCallback )
: m_CurlCallback( curlCallback )
{
}

CurlTool::~CurlTool()
{
}

int CurlTool::executeCurl( const char * cmd )
{
	std::vector<std::string> argList;
	// first param is the executable
	std::string exeName;
	m_FileUtil.getExecuteFullPathAndName( exeName );
	argList.push_back( exeName ); 

	parseIntoArgsList( cmd, argList );

	int argc = (int)argList.size();
	char **argv = new char *[argc + 1];

	int argIdx = 0;

	std::vector<std::string>::iterator iter;
	for( iter = argList.begin(); iter != argList.end(); ++iter )
	{
		argv[argIdx] = (char *)((*iter).c_str());
		argIdx++;
	}
	argv[argc] = NULL;

	return CurlToolExe( argc, argv );
}

int CurlTool::parseIntoArgsList( const char * cmd, std::vector<std::string>& argList )
{
	std::string strCmd = cmd;

	StdStringSplit( strCmd, ' ', argList );
	return (int)argList.size();
}
