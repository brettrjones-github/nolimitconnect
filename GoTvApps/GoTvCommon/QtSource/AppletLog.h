#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletBase.h"

#include "ui_AppletLog.h"

#include <QMutex>
#include <QFile>

#include <CoreLib/VxDebug.h>

class AppletLog : public AppletBase, public ILogCallbackInterface
{
    Q_OBJECT
public:
    AppletLog( AppCommon& app, QWidget * parent );
    virtual ~AppletLog();

    void                        logMsg( const char* logMsg, ... );

    void                        onLogEvent( uint32_t u32LogFlags, const char * logMsg ) override;

signals:
    void                        signalLogMsg( const QString& logMsg );

protected slots:
    void						gotoWebsite( void );
    void                        slotLogSettingButtonClick( void );
    void                        slotLogMsg( const QString& text );

    void                        slotCopyToClipboardClicked( void );

protected:
    void						setupApplet( void );
    void                        clear()                 { getLogEdit()->clear(); }
    QPlainTextEdit *            getLogEdit( void )      { return ui.m_LogPlainTextEdit; }
    void                        fillBasicInfo( void );

    QFile                       m_LogFile;
    QMutex                      m_LogMutex;
    LOG_FUNCTION                m_OldLogFunction{ nullptr };
    void *                      m_OldLogUserData{ nullptr};
    bool                        m_VerboseLog{ false };

    Ui::AppletLogUi ui;
};


