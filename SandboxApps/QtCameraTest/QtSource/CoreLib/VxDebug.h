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

//#include "VxDefs.h"
#include <stdarg.h>
#include <stdint.h>

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


#define LOG_SKT		(LOG_INFO | 0x0200)
#define LOG_JNI		(LOG_INFO | 0x0400)
#define LOG_TEST	(LOG_INFO | 0x0800)
#define LOG_PKT	    (LOG_INFO | 0x1000)
#define LOG_THREAD	(LOG_INFO | 0x2000)

// defines so less work converting Linux code
#define LOG_WARNING		LOG_WARN
#define LOG_ERR			LOG_ERROR
#define LOG_CRIT		LOG_FATAL

#define MAX_ERR_MSG_SIZE 4096
typedef void (*LOG_FUNCTION )( void *, uint32_t, char * );

#ifdef __cplusplus
#include <string>
#include <vector>
enum ELogModule
{
    eLogNone = 0,

    eLogMulticast = 0x0001,
    eLogConnect = 0x0002,
    eLogListen = 0x0004,
    eLogSkt = 0x0008,

    eLogPkt = 0x0010,
    eLogNetAccessStatus = 0x0020,  // internet and p2p access status
    eLogNetworkState = 0x0040,
    eLogNetworkMgr = 0x0080,

    eLogIsPortOpenTest = 0x0100,
    eLogThread = 0x0200,
    eLogStorage = 0x0400, // mostly user and app directories
    eLogAssets = 0x0800,

    eLogPlugins = 0x1000,
    eLogWindowPositions = 0x2000,
    eLogStartup = 0x4000,
    eLogHosts = 0x8000,

    eLogPlayer = 0x00010000,
    eLogTcpData = 0x00020000,
    eLogUdpData = 0x00040000,
    eLogAcceptConn = 0x00080000,

    eLogNetworkRelay = 0x00100000,

    eMaxLogModule
};

void LogModule( ELogModule eLog, unsigned long u32MsgType, const char* msg, ... );

/// @brief return true if should log the given module
bool IsLogEnabled( ELogModule logModule );
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
#endif // __cplusplus

#define LOG_PRIORITY_MASK	    0x000001ff
#define LOG_MODULE_MASK		    0xfffffe00

#define LOG_FLAG_THREADS        0x00000400
#define LOG_FLAG_ASSETS         0x00000800
#define LOG_FLAG_P2P_ENGINE     0x00001000
#define LOG_FLAG_FFMPEG         0x00002000
#define LOG_FLAG_XPMC           0x00004000
#define LOG_FLAG_PORT_TEST      0x00008000
#define LOG_FLAG_CONNECT        0x00010000

#define LOG_FLAG_VIDEO_PLAY     0x00200000
#define LOG_FLAG_AUDIO_PLAY     0x00400000
#define LOG_FLAG_PLAYER_SYNC    0x00800000
#define LOG_FLAG_PLAYER         0x00F00000

#ifdef __cplusplus
extern "C" {
#endif

// set logging priority mask or 0 to disable logging example  VxSetLogPriorityMask( LOG_VERBOSE ) or VxSetLogPriorityMask( LOG_PRIORITY_MASK )
void                            VxSetLogPriorityMask( uint32_t flags );
uint32_t                        VxGetLogPriorityMask( void );

// get module enable logging flags defined above
void                            VxSetModuleLogFlags( uint32_t flags );
uint32_t                        VxGetModuleLogFlags( void );

void							VxSetLogHandler( LOG_FUNCTION pfuncLogHandler, void * userData );
void							VxSetLogToFile( const char * pFileName );
void							VxSetLogFlags( unsigned long u32LogFlags );
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

    extern bool g_StreamActive;

#ifdef __cplusplus
}
#endif


