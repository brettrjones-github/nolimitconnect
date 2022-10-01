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

#include "AppletBase.h"

#include "ui_AppletApplicationInfo.h"

#include <QMutex>
#include <QFile>

#include <CoreLib/VxDebug.h>

class AppletApplicationInfo : public AppletBase, public ILogCallbackInterface
{
    Q_OBJECT
public:
    AppletApplicationInfo( AppCommon& app, QWidget* parent );
    virtual ~AppletApplicationInfo();

    void                        logMsg( const char* logMsg, ... );
    void                        infoMsg( const char * infoMsg, ... );

    void                        onLogEvent( uint32_t u32LogFlags, const char * logMsg ) override;
    void                        toGuiInfoMsg( char * logMsg );

signals:
    void                        signalLogMsg( const QString& logMsg );
    void                        signalInfoMsg( const QString& logMsg );

protected slots:
    void						gotoWebsite( void );
    void                        slotExtraInfoButtonClick( void );
    void                        slotLogMsg( const QString& text );
    void                        slotInfoMsg( const QString& text );

    void                        slotCopyToClipboardClicked( void );
    void                        slotBrowseStorage( void );

protected:
    void						setupApplet( void );
    void                        clear()                 { getLogEdit()->clear(); }
    QPlainTextEdit *            getInfoEdit( void )     { return ui.m_InfoPlainTextEdit; }
    QPlainTextEdit *            getLogEdit( void )      { return ui.m_LogPlainTextEdit; }
    void                        fillBasicInfo( void );
    void                        fillExtraInfo( void );

    QFile                       m_LogFile;
    QMutex                      m_LogMutex;
    LOG_FUNCTION                m_OldLogFunction{ nullptr };
    void *                      m_OldLogUserData{ nullptr};
    bool                        m_VerboseLog{ false };

    Ui::AppletApplicationInfoUi ui;
};


