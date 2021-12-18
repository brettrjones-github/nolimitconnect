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
// http://www.nolimitconnect.com
//============================================================================

#include <AppDependLibrariesConfig.h>
#include <CoreLib/VxDefs.h>
#include <CoreLib/VxFileUtil.h>

class ICurlCallback
{
public:
	virtual void				curlCommandResultData( const char * data ) = 0;
};

class CurlTool
{
public:
	CurlTool( ICurlCallback& curlCallback );
	virtual ~CurlTool();

	virtual int					executeCurl( const char * cmd );

protected:
	int							parseIntoArgsList( const char * cmd, std::vector<std::string>& argList );

	VxFileUtil					m_FileUtil;
	ICurlCallback&				m_CurlCallback;
};

