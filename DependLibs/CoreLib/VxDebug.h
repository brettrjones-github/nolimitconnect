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
// http://www.nolimitconnect.com
//============================================================================

#include "VxDefs.h"
#include <stdarg.h>

#define LOG_NONE    (0x0000)
#define LOG_VERBOSE (0x0001)
#define LOG_FATAL	(0x0002)
#define LOG_SEVERE	(0x0004)
#define LOG_ASSERT	(0x0008)
#define LOG_ERROR	(0x0010)
#define LOG_WARN	(0x0020)
#define LOG_DEBUG	(0x0040)
#define LOG_INFO	(0x0080)
#define LOG_STATUS	(0x0100)
#define LOG_PRIORITY_MASK	    0x000001ff

// defines so less work converting Linux code
#define LOG_WARNING		LOG_WARN
#define LOG_ERR			LOG_ERROR
#define LOG_CRIT		LOG_FATAL

#define MAX_ERR_MSG_SIZE 4096
typedef void ( *LOG_FUNCTION )( void *, uint32_t, char * );

// protected by #ifdef __cplusplus so can be included in .c code
#ifdef __cplusplus
#include <string>
#include <vector>

enum ELogModule
{
    eLogNone = 0,

    eLogMulticast       = 0x0001, // 1
    eLogConnect         = 0x0002, // 2
    eLogListen          = 0x0004, // 3
    eLogSkt             = 0x0008, // 4

    eLogPkt             = 0x0010, // 5
    eLogNetAccessStatus = 0x0020, // 6 internet and p2p access status
    eLogNetworkState    = 0x0040, // 7
    eLogNetworkMgr      = 0x0080, // 8

    eLogIsPortOpenTest  = 0x0100, // 9
    eLogThread          = 0x0200, // 10
    eLogStorage         = 0x0400, // 11 mostly user and app directories
    eLogAssets          = 0x0800, // 12

    eLogPlugins         = 0x1000, // 13
    eLogWindowPositions = 0x2000, // 14
    eLogStartup         = 0x4000, // 15
    eLogHosts           = 0x8000, // 16

    eLogMediaStream     = 0x00010000, // 17
    eLogTcpData         = 0x00020000, // 18
    eLogUdpData         = 0x00040000, // 19
    eLogAcceptConn      = 0x00080000, // 20

    eLogNetworkRelay    = 0x00100000, // 21
    eLogNetService      = 0x00200000, // 22
    eLogRunTest         = 0x00400000, // 23
    eLogHostConnect     = 0x00800000, // 24
    eLogHostSearch      = 0x01000000, // 25
    eLogVideo           = 0x02000000, // 26

    eMaxLogModule
};

void LogModule( ELogModule eLog, unsigned long u32MsgType, const char* msg, ... );

/// @brief return true if should log the given module
bool IsLogEnabled( ELogModule logModule );

class ILogCallbackInterface
{
public:
    virtual void                onLogEvent( uint32_t u32LogFlags, const char * logMsg ) = 0;
};


class LogEntry
{
public:
	LogEntry()
		: m_LogFlags( 0 )
		, m_LogText( "" )
	{}
	LogEntry(unsigned long flags, const char * text )
		: m_LogFlags( flags )
		, m_LogText( text )
	{}
	~LogEntry(){}
	LogEntry( const LogEntry& rhs )
	{
		*this = rhs;
	}

	LogEntry & operator =( const LogEntry &rhs )
	{
		if( this != &rhs )
		{
			m_LogFlags			= rhs.m_LogFlags;
			m_LogText			= rhs.m_LogText;
		}

		return *this;
	}

	//=== vars ===//
	unsigned long			m_LogFlags;
	std::string				m_LogText;
};


// uncomment to enable log listing for retrieval
//#define ENABLE_LOG_LIST 1

void							VxGetLogMessages( unsigned long u32MsgTypes, std::vector<LogEntry>& retMsgs );
// add a log handler
void							VxAddLogHandler( ILogCallbackInterface * logHandler );
// remove a log handler
void							VxRemoveLogHandler( ILogCallbackInterface * logHandler );

#endif // __cplusplus


#ifdef __cplusplus
extern "C" {
#endif

// set logging priority mask or 0 to disable logging example  VxSetLogPriorityMask( LOG_VERBOSE ) or VxSetLogPriorityMask( LOG_PRIORITY_MASK )
void                            VxSetLogPriorityMask( uint32_t flags );
uint32_t                        VxGetLogPriorityMask( void );

// get module enable logging flags defined above
void                            VxSetModuleLogFlags( uint32_t flags );
uint32_t                        VxGetModuleLogFlags( void );

// enable log to file
void							VxSetLogToFile( const char * pFileName );
void							VxSetLogFlags( unsigned long u32LogFlags );

// enable/disable default log handler
void                            VxEnableDefaultLogHandler( bool enableDefaultHandler );

void							LogMsg( unsigned long u32MsgType, const char* msg, ...);
void							LogMsgVarg(unsigned long u32MsgType, const char *fmt, va_list vargs);
void							VxHandleLogMsg(unsigned long u32MsgType, char * msg);
void							HexDump( unsigned long u32MsgType, unsigned char* data, int dataLen, int instance, char * msg );
void							DumpInt8( unsigned long u32MsgType, int8_t* data, int dataLen, int instance, char * msg );
void							DumpInt16( unsigned long u32MsgType, int16_t* data, int dataLen, int instance, char * msg );
void							DumpFloat( unsigned long u32MsgType, float* data, int dataLen, int instance, char * msg );

// Convenience macros for logging
#define LogVerboseMsg(msg, ...) LogMsg(LOG_VERBOSE, (msg), __VA_ARGS__)
#define LogInfoMsg(msg, ...) LogMsg(LOG_VERBOSE, (msg), __VA_ARGS__)
#define LogDebugMsg(msg, ...) LogMsg(LOG_DEBUG, (msg), __VA_ARGS__)
#define LogWarnMsg(msg, ...) LogMsg(LOG_WARN, (msg), __VA_ARGS__)
#define LogErrorMsg(msg, ...) LogMsg(LOG_ERROR, (msg), __VA_ARGS__)
#define LogFatalMsg(msg, ...) LogMsg(LOG_FATAL, (msg), __VA_ARGS__)
#define LogStatusMsg(msg, ...) LogMsg(LOG_STATUS, (msg), __VA_ARGS__)


#if defined( _DEBUG ) || defined( DEBUG )
	/// This function is called by vx_assert() when the assertion fails.
	void  vx_error_output( unsigned long u32LogLevel, char* exp, char * file, int line);

	// debug mode throws message if expression is false
	#define vx_assert(exp) { if (!(exp)) vx_error_output(LOG_ASSERT,(char*)#exp,(char*)__FILE__,__LINE__); }
	#define vx_verify(exp) { if (!(exp)) vx_error_output(LOG_ASSERT,(char*)#exp,(char*)__FILE__,__LINE__); }
	#define vx_fatal(exp) { if (!(exp)) vx_error_output(LOG_FATAL,(char*)#exp,(char*)__FILE__,__LINE__); }
#else
	// release mode evaluates vx_assert ect to null
	#define vx_assert(exp)
	#define vx_verify(exp) (exp)
	#define vx_fatal(exp) (exp)
#endif // RELEASE_MODE


#ifdef __cplusplus
}
#endif


