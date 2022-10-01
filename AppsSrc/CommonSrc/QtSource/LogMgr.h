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
#pragma once

#include "AppCommon.h"
#include <QMutex>
#include <QString>
#include <QVector>

class LogQueEntry
{
public:
    LogQueEntry() = default;
    LogQueEntry( uint32_t u32LogFlags, const QString& text )
        : m_LogLevelFlags( u32LogFlags )
        , m_LogMsg( text )
    {
    }

    void                        setLogFlags( uint32_t u32LogFlags ) { m_LogLevelFlags = u32LogFlags; }
    uint32_t                    getLogFlags( void )                 { return m_LogLevelFlags; }
    void                        setLogText( const QString& text )   { m_LogMsg = text; }
    QString&                    getLogText( void )                  { return m_LogMsg; }
    
    uint32_t                    m_LogLevelFlags{0};
    QString                     m_LogMsg;
};

class LogMgr : public QWidget, public ILogCallbackInterface
{
    Q_OBJECT
public:
    LogMgr( QWidget* parent = nullptr );
    virtual ~LogMgr() override;

    void                        startupLogMgr( void );
    void                        shutdownLogMgr( void );

    void                        lockLogQue( void )                  { m_LogMutex.lock(); }
    void                        unlockLogQue( void )                { m_LogMutex.unlock(); }

    void                        setLogLevels( uint32_t logLevelFlags );
    uint32_t                    getLogLevels( void );

    void                        setLogModules( uint32_t logModuleFlags );
    uint32_t                    getLogModules( void );

    void                        setLogQueMaxLines( int maxLineCnt ) { m_LogQueMaxLines = maxLineCnt; }
    int                         getLogQueMaxLines( void )           { return m_LogQueMaxLines; }

    QVector<LogQueEntry>&       getLogQue( void )                   { return m_LogQue; }
    void                        clearLogQue( void )                 { m_LogQue.clear(); }

    void                        addLogMsgToQue( uint32_t u32LogFlags, const QString& text );

    void                        onLogEvent( uint32_t u32LogFlags, const char * logMsg ) override;

signals:
    void                        signalLogMsg( uint32_t u32LogFlags, const QString& logMsg );

protected slots:
    void                        slotLogMsg( uint32_t u32LogFlags, const QString& logMsg );

protected:
    AppCommon&                  m_MyApp;
    QMutex                      m_LogMutex;
    bool                        m_VerboseLog = false;
    QVector<LogQueEntry>        m_LogQue;
    int                         m_LogQueMaxLines{ 1024 };
};

// get log manager singleton
LogMgr& GetLogMgrInstance();