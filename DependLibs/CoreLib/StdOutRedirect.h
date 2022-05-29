#ifndef STD_OUT_REDIRECT_H
#define STD_OUT_REDIRECT_H
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
#include <CoreLib/config_corelib.h>

class StdOutRedirect
{
public:
	StdOutRedirect(int bufferSize);
	virtual ~StdOutRedirect();

	int startRedirect();
	int stopRedirect();
	int getStdOutBuffer(char *buffer, int size);
	int getStdErrBuffer(char *buffer, int size);

private:
	int m_fdStdOutPipe[2];
	int m_fdStdOut;
	int m_fdStdErrPipe[2];
	int m_fdStdErr;
};

#endif // STD_OUT_REDIRECT_H
