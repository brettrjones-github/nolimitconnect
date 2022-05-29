#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "IGoTvDefs.h"

#define ILOG_LEVEL_NONE         (0) // none ( disable )
#define ILOG_LEVEL_INFO	        (1) // info only
#define ILOG_LEVEL_FATAL	    (2) // fatal + info
#define ILOG_LEVEL_ASSERT	    (3) // assert + fatal + info
#define ILOG_LEVEL_SEVERE	    (4) // severe + assert + fatal + info
#define ILOG_LEVEL_ERROR	    (5) // error + severe + assert + fatal + info
#define ILOG_LEVEL_WARN	        (6) // warn + error + severe + assert + fatal + info
#define ILOG_LEVEL_DEBUG	    (7) // debug + warn + error + severe + assert + fatal + info
#define ILOG_LEVEL_VERBOSE      (8) // all

#define ILOG_LEVEL_RELEASE ILOG_LEVEL_ERROR // change this to change what default log level is when building release

class IGoTv;

class ILog
{
public:
    ILog( );
    virtual  ~ILog() = default;

    // call these first to set all log levels
    void                        setAllLogLevel( int level );
    void                        clearAllLogLevel();

    // adjust log levels of different modules after the "all" level is set
    // this level is overall end log output.. levels of others higher than this will still be filtered out
    void                        setCoreLogLevel( int level );

    void                        setKodiLogLevel( int level );
    void                        clearKodiLogLevel();

    void                        setFfmpegLogLevel( int level );
    int                         getFfmpegLogLevel() { return m_ffmpegLogLevel; }
    void                        clearFfmpegLogLevel();

private:
    int                         m_CoreLogLevel;
    int                         m_ffmpegLogLevel;
    int                         m_kodiLogLevel;
};
