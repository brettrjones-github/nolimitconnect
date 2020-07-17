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

#include "VxDebug.h"
#include <string.h>

#ifdef TARGET_OS_WINDOWS
# include <WinSock2.h>
# include <windows.h>
#elif defined(TARGET_OS_ANDROID)
# include <android/log.h>
# include <unistd.h>
#else
# include <unistd.h>
#endif // TARGET_OS_WINDOWS

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <string.h>

//#define ENABLE_LOG_LIST 1 // define if want stored log list capabilities

bool g_StreamActive = false;

namespace
{
#if defined(DEBUG)
    uint32_t g_ModuleEnableLoggingFlags = ( uint32_t )(
        eLogMulticast
        | eLogConnect
        | eLogListen
        | eLogSkt
        | eLogPkt
        | eLogNetworkState
        | eLogNetworkMgr
        | eLogIsPortOpenTest
        // | eLogThread
        | eLogStorage
        | eLogAssets
        | eLogPlugins
        // | eLogStartup
        | eLogHosts
        | eLogTcpData
        | eLogUdpData
        //| eLogNetworkRelay
        //| eLogAcceptConn
        //| eLogPlayer
        //| eLogWindowPositions
        );
#else
    uint32_t g_ModuleEnableLoggingFlags = 0;
#endif // defined(DEBUG)

    unsigned long				g_u32LogFlags                   = LOG_PRIORITY_MASK;

    void *						g_pvUserData                    = 0;


    int							g_iLogNameLen                   = 0;

#if ENABLE_LOG_LIST
	std::vector<LogEntry>		g_LogEntryList;
	void						AddLogEntry( unsigned long flags, const char * text )
	{
		g_oFileLogMutex.lock();
		g_LogEntryList.push_back( LogEntry( flags, text ) );
		if( 4000 < g_LogEntryList.size() )
		{
			g_LogEntryList.erase( g_LogEntryList.begin(), g_LogEntryList.begin() + 1000 );
		}

		g_oFileLogMutex.unlock();
	}

#endif // ENABLE_LOG_LIST
}

//============================================================================
void                    default_log_handler( void * userData, uint32_t u32MsgType, char * pLogMsg );
void                    vx_error( unsigned long u32MsgType, const char* msg, ... );

LOG_FUNCTION		    g_pfuncLogHandler = default_log_handler;
//============================================================================

//============================================================================
void LogAppendLineFeed( char * buf, size_t sizeOfBuf )
{
    int strLen = strlen( buf );
    if( ( sizeOfBuf > 2 ) && ( 0 < strLen ) && ( strLen < ( sizeOfBuf - 2 ) ) )
    {
        if( buf[ strLen - 1 ] != '\n' )
        {
            buf[ strLen ] = '\n';
            buf[ strLen + 1 ] = 0;
        }
    }
}

//============================================================================
/// @brief return true if should log the given module
bool IsLogEnabled( ELogModule logModule )
{
    return ( g_ModuleEnableLoggingFlags & logModule ) ? true : false;
}

//============================================================================
void LogModule( ELogModule eLog, unsigned long u32MsgType, const char* msg, ... )
{
    if( 0 == ( g_u32LogFlags && u32MsgType ) )
    {
        return; // don't log
    }

    if( IsLogEnabled( eLog ) )
    {
        char as8Buf[ MAX_ERR_MSG_SIZE ];
        va_list argList;
        va_start( argList, msg );
        vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
        as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
        LogAppendLineFeed( as8Buf, sizeof( as8Buf ) );
        va_end( argList );

        VxHandleLogMsg( u32MsgType, as8Buf );
    }
}



////============================================================================
//uint32_t VxGetModuleLogFlags( void )
//{
//    return g_ModuleEnableLoggingFlags;
//}
//
////============================================================================
//void  VxSetLogPriorityMask( uint32_t flags )
//{
//    g_ModuleEnableLoggingFlags = flags;
//}
//
////============================================================================
//uint32_t VxGetLogPriorityMask( void )
//{
//    return g_u32LogFlags;
//}
//
////============================================================================
//void  VxSetModuleLogFlags( uint32_t flags )
//{
//    g_u32LogFlags = flags;
//}

//============================================================================
void log_to_file( const char * pFileName, const char * pMsg )
{
static bool firstLog = true;
    //if( false == VxIsAppShuttingDown() )
	{
		if(pMsg)
		{
			int iMsgLen = strlen( pMsg );
            //g_oFileLogMutex.lock();
			if( firstLog )
			{
                unlink( pFileName );
				firstLog = false;
			}
			
			FILE * poFile = fopen( pFileName, "ab+" );
			if( poFile )
			{ 
				fwrite( pMsg, iMsgLen, 1, poFile );
				fclose( poFile );
			}
			
            //g_oFileLogMutex.unlock();
		}
	}
}

//============================================================================
void VxSetLogFlags( unsigned long u32LogFlags )
{
	g_u32LogFlags = u32LogFlags;
}

//============================================================================
// This function sets the handler of log messages
void VxSetLogHandler( LOG_FUNCTION pfuncLogHandler, void * userData )
{
	g_pfuncLogHandler = pfuncLogHandler;
	g_pvUserData = userData;
}

//============================================================================
// This function is called by vx_assert() when the assertion fails.
void  vx_error_output( unsigned long u32MsgType, char* exp,char * file, int line)
{
    vx_error(u32MsgType, "** VX ASSERTION **\r\nexpression: %s\r\nfile: %s\r\nline: %d\r\n", exp, file, line);
}

//============================================================================
// This function is called by vx_assert2() when the assertion fails.
void vx_error_output2(unsigned long u32MsgType, char* exp,  char* msg, char* file, int line)
{
    vx_error(u32MsgType, "** VX ASSERTION ***\r\nprogrammer says: %s\r\nexpression: %s\r\nfile: %s\r\nline: %d\r\n", msg, exp, file, line);
}

//============================================================================
// This function is called when a serious situation is encountered which
// requires abortion of the program.
void vx_error( unsigned long u32MsgType, const char* msg, ...)
{
	char as8Buf[ MAX_ERR_MSG_SIZE * 2 ];
    va_list argList;
    va_start(argList, msg);
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
    LogAppendLineFeed( as8Buf, sizeof( as8Buf ) );
    va_end(argList);

	// append the module name
	//strcat( as8Buf, "Module Name: " );
	//char as8ModuleName[ VX_MAX_PATH ];
	//VxGetExecuteModuleName( as8ModuleName, sizeof( as8ModuleName ) );
	//strcat( as8Buf, as8ModuleName );
	//strcat( as8Buf, "\r\n\r\n" );

	//printf( as8Buf ); // show the error
	//std::string strAssertFile = g_strUserAppDataDir +  "vx_assert.txt";	
	//writeWholeFile( strAssertFile.c_str(), as8Buf, (uint32_t)strlen( as8Buf ) );
 //   fflush(stdout);

	LogMsg( u32MsgType,  as8Buf ); // send to log
	if( LOG_FATAL == u32MsgType )
	{
		//exit(99);
	}
};
//============================================================================
/// default log handler
void default_log_handler( void * userData, uint32_t u32MsgType, char * pLogMsg )
{
	u32MsgType = (u32MsgType & LOG_PRIORITY_MASK);
	//if( u32MsgType > g_u32LogLevel )
	//{
	//	return; // dont log
	//}
#ifdef TARGET_OS_ANDROID
#define MY_LOG_TAG "LogHandler:"
//#ifndef	_DEBUG
//	return; // dont do logging in release mode
//#endif //	_DEBUG
	switch( u32MsgType )
	{
	case LOG_INFO:
		__android_log_write(ANDROID_LOG_INFO, MY_LOG_TAG, pLogMsg);
		break;
		
	case LOG_FATAL:
	case LOG_ASSERT:
	case LOG_ERROR:
		__android_log_write(ANDROID_LOG_ERROR, MY_LOG_TAG, pLogMsg);
		break;
		
	case LOG_WARN:
		__android_log_write(ANDROID_LOG_WARN, MY_LOG_TAG, pLogMsg);
		break;
		
	case LOG_VERBOSE:
		__android_log_write(ANDROID_LOG_DEBUG, MY_LOG_TAG, pLogMsg);
		break;
		
	default:
		__android_log_write(ANDROID_LOG_DEBUG, MY_LOG_TAG, pLogMsg);
	}
#else
	printf( pLogMsg );
#endif// TARGET_OS_ANDROID
}

void LogMsgVarg(unsigned long u32MsgType, const char *fmt, va_list argList )
{

	if (0 == (g_u32LogFlags && u32MsgType))
	{
		return; // don't log
	}

	char as8Buf[MAX_ERR_MSG_SIZE];
    //va_start(argList, fmt );
	vsnprintf( as8Buf, sizeof(as8Buf), fmt, argList );
	as8Buf[sizeof(as8Buf) - 1] = 0;
    LogAppendLineFeed( as8Buf, sizeof( as8Buf ) );
    //va_end(argList);
	
	VxHandleLogMsg( u32MsgType, as8Buf );
}


//============================================================================
void LogMsg( unsigned long u32MsgType, const char* msg, ...)
{

	if (0 == (g_u32LogFlags && u32MsgType))
	{
		return; // don't log
	}

	char as8Buf[MAX_ERR_MSG_SIZE];
	va_list argList;
	va_start(argList, msg);
	vsnprintf(as8Buf, sizeof(as8Buf), msg, argList);
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    LogAppendLineFeed( as8Buf, sizeof( as8Buf ) );
	va_end(argList);

	VxHandleLogMsg( u32MsgType, as8Buf );
}

//============================================================================
void VxHandleLogMsg( unsigned long u32MsgType, char * logMsg )
{
	#ifdef TARGET_OS_WINDOWS
		OutputDebugStringA( logMsg );
	#endif // TARGET_OS_WINDOWS

	//if( false == VxIsAppShuttingDown() )
	//{
	//	AddLogEntry( u32MsgType, as8Buf );
	//}

#ifdef TARGET_OS_ANDROID

    if( g_pfuncLogHandler != default_log_handler )
    {
        // call the default also so will get logcat in android
        default_log_handler( g_pvUserData, u32MsgType, logMsg);
    }

	g_pfuncLogHandler( g_pvUserData, u32MsgType, logMsg);
#else
    //if( false == VxIsAppShuttingDown() )
	{
		g_pfuncLogHandler( g_pvUserData, u32MsgType, logMsg );
	}
#endif // TARGET_OS_WINDOWS
}

//============================================================================
static void DumpMsg( unsigned long u32MsgType, int instance, int sampleCnt, char * msg )
{
    if( msg && strlen( msg ) )
    {
        LogMsg( u32MsgType, "Dump %d Cnt %d %s\n", instance, sampleCnt, msg );
    }
}

//============================================================================
void HexDump( unsigned long u32MsgType, unsigned char* data, int dataLen, int instance, char * msg )
{
    DumpMsg( u32MsgType, instance, dataLen, msg );
	char buf[ 132 ];

	while( dataLen )
	{
		int dataThisDump = dataLen;
		if( dataThisDump > 16 )
		{
			dataThisDump = 16;
		}

		int curIdx = 0;

		for( int i = 0; i < dataThisDump; ++i )
		{
			sprintf( &buf[curIdx], "%2.2x ", data[i] );
			curIdx += 3;
		}

		if( dataThisDump < 16 )
		{
			int spaces = 16 - dataLen;
			for( int i = 0; i < spaces; ++i )
			{
				buf[curIdx] = ' ';
				curIdx++;
			}
		}

		for( int i = 0; i < dataThisDump; ++i )
		{
			if( (buf[curIdx] < 20) || (buf[curIdx] > 127) || !isalnum( buf[curIdx] ) )
			{
				buf[curIdx] = '.';
			}
			else
			{
				sprintf( &buf[curIdx], "%c", data[i] );
			}

			curIdx++;
		}

		strcpy( &buf[curIdx], "\n" );
		LogMsg( u32MsgType, buf );
		data += dataThisDump;
		dataLen -= dataThisDump;
	}
}

void DumpInt8( unsigned long u32MsgType, int8_t* data, int dataLen, int instance, char * msg )
{
    int sampleCnt = dataLen / sizeof( int8_t );
    DumpMsg( u32MsgType, instance, sampleCnt, msg );

    char outBuf[ 4096 ];
    outBuf[ 0 ] = 0;
    int sampleIdx = 0;
    //LogMsg( u32MsgType, "DumpInt8 cnt %d ", sampleCnt );
    while( sampleCnt >= 16 )
    {
        for( int i = 0; i < 16; i++ )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%d ", data[ sampleIdx ] );
            sampleIdx++;
        }

        LogMsg( u32MsgType, outBuf );
        outBuf[ 0 ] = 0;
        sampleCnt -= 16;
    }

    if( sampleCnt )
    {
        outBuf[ 0 ] = 0;
        while( sampleCnt )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%d ", data[ sampleIdx ] );
            sampleIdx++;
            sampleCnt--;
        }

        LogMsg( u32MsgType, outBuf );
    }
}

void DumpInt16( unsigned long u32MsgType, int16_t* data, int dataLen, int instance, char * msg )
{
    int sampleCnt = dataLen / sizeof( int16_t );
    DumpMsg( u32MsgType, instance, sampleCnt, msg );

    char outBuf[ 4096 ];
    outBuf[ 0 ] = 0;
    int sampleIdx = 0;
    //LogMsg( u32MsgType, "DumpInt16 cnt %d ", sampleCnt );
    while( sampleCnt >= 16 )
    {
        for( int i = 0; i < 16; i++ )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%d ", data[ sampleIdx ] );
            sampleIdx++;
        }

        LogMsg( u32MsgType, outBuf );
        outBuf[ 0 ] = 0;
        sampleCnt -= 16;
    }

    if( sampleCnt )
    {
        outBuf[ 0 ] = 0;
        while( sampleCnt  )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%d ", data[ sampleIdx ] );
            sampleIdx++;
            sampleCnt--;
        }

        LogMsg( u32MsgType, outBuf );
    }
}

void DumpFloat( unsigned long u32MsgType, float* data, int dataLen, int instance, char * msg )
{
    int sampleCnt = dataLen / sizeof( float );
    DumpMsg( u32MsgType, instance, sampleCnt, msg );

    char outBuf[ 4096 ];
    outBuf[ 0 ] = 0;
    int sampleIdx = 0;
    //LogMsg( u32MsgType, "DumpFloat cnt %d ", sampleCnt );
    while( sampleCnt >= 16 )
    {
        for( int i = 0; i < 16; i++ )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%3.3f ", data[ sampleIdx ] );
            sampleIdx++;
        }

        LogMsg( u32MsgType, outBuf );
        outBuf[ 0 ] = 0;
        sampleCnt -= 16;
    }

    if( sampleCnt )
    {
        outBuf[ 0 ] = 0;
        while( sampleCnt )
        {
            int curIdx = strlen( outBuf );
            sprintf( &outBuf[ curIdx ], "%3.3f ", data[ sampleIdx ] );
            sampleIdx++;
            sampleCnt--;
        }

        LogMsg( u32MsgType, outBuf );
    }
}

