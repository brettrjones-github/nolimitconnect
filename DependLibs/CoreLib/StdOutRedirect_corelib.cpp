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


#include "config_corelib.h"
#ifdef WIN32

#include "StdOutRedirect.h"
#include "VxGlobals.h"

#include <WinSock2.h>
#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <cstdio>

#include "VxDebug.h"

#define READ_FD 0
#define WRITE_FD 1

#define CHECK(a) if ((a)!= 0) return -1;


StdOutRedirect::StdOutRedirect(int bufferSize)
{
	if (_pipe(m_fdStdOutPipe, bufferSize, O_TEXT)!=0)
	{
		LogMsg( LOG_ERROR, "ERROR: Could not create stdout pipe\n");
	}
	m_fdStdOut = _dup(_fileno(stdout));

	if (_pipe(m_fdStdErrPipe, bufferSize, O_TEXT)!=0)
	{
		LogMsg( LOG_ERROR, "ERROR: Could not create stderr pipe\n");
	}
	m_fdStdErr = _dup(_fileno(stderr));
}

StdOutRedirect::~StdOutRedirect()
{
	if( m_fdStdOut && ( -1 != m_fdStdOut ) )
	{
		_close(m_fdStdOut);
	}
	_close(m_fdStdOutPipe[WRITE_FD]);
	_close(m_fdStdOutPipe[READ_FD]);

	if( m_fdStdErr && ( -1 != m_fdStdErr ) )
	{
		_close(m_fdStdErr);
	}
	_close(m_fdStdErrPipe[WRITE_FD]);
	_close(m_fdStdErrPipe[READ_FD]);
}

int StdOutRedirect::startRedirect()
{
	if( -1 == m_fdStdOut )
	{
		// for some reason stdout may not yet exist in windows
		// this hack fixes that
		std::string strTmp = VxGetAppTempDirectory();
		strTmp += "stdout.txt";

		if( 0 == freopen (strTmp.c_str(),"w+",stdout) )
		{
			LogMsg( LOG_ERROR, "Cannot open stdout\n" );
		}
		m_fdStdOut = _dup(_fileno(stdout));
		if( -1 == m_fdStdOut )
		{
			LogMsg( LOG_ERROR, "ERROR: Could not duplicate stdout fileno\n");
			m_fdStdOut = 0;
			return -1;
		}
	}

	if( -1 == m_fdStdErr )
	{
		// for some reason stdout may not yet exist in windows
		// this hack fixes that
		std::string strTmp = VxGetAppTempDirectory();
		strTmp += "stderr.txt";

		if( 0 == freopen (strTmp.c_str(),"w+",stderr) )
		{
			LogMsg( LOG_ERROR, "Cannot open stdout\n" );
		}
		m_fdStdErr = _dup(_fileno(stderr));
		if( -1 == m_fdStdErr )
		{
			LogMsg( LOG_ERROR, "ERROR: Could not duplicate stderr fileno\n");
			return -1;
		}
	}

	fflush( stdout );
	CHECK(_dup2(m_fdStdOutPipe[WRITE_FD], _fileno(stdout)));
	std::cout.sync_with_stdio();
	setvbuf( stdout, NULL, _IONBF, 0 ); // absolutely needed

	fflush( stderr );
	CHECK(_dup2(m_fdStdErrPipe[WRITE_FD], _fileno(stderr)));
	std::cerr.sync_with_stdio();
	setvbuf( stderr, NULL, _IONBF, 0 ); // absolutely needed
	return 0;
}

int StdOutRedirect::stopRedirect()
{
	CHECK(_dup2(m_fdStdOut, _fileno(stdout)));
	std::cout.sync_with_stdio();

	CHECK(_dup2(m_fdStdErr, _fileno(stderr)));
	std::cerr.sync_with_stdio();

	return 0;
}

int StdOutRedirect::getStdOutBuffer(char *buffer, int size)
{
	int nOutRead = _read(m_fdStdOutPipe[READ_FD], buffer, size);
	buffer[nOutRead] = '\0';
	return nOutRead;
}

int StdOutRedirect::getStdErrBuffer(char *buffer, int size)
{
	int nOutRead = _read(m_fdStdErrPipe[READ_FD], buffer, size);
	buffer[nOutRead] = '\0';
	return nOutRead;
}

#endif // WIN32
