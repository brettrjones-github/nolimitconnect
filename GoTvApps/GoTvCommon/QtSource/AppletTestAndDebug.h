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
#include "ui_AppletTestAndDebug.h"
#include <GuiInterface/IToGui.h>

#include <CoreLib/VxDebug.h> // needed for ILogCallbackInterface

#include <QMutex>
#include <QFile>

class VxUrl;

class AppletTestAndDebug : public AppletBase, public ILogCallbackInterface
{
    Q_OBJECT
public:
    AppletTestAndDebug( AppCommon& app, QWidget * parent );
    virtual ~AppletTestAndDebug();

    void                        logMsg( const char* logMsg, ... );
    void                        infoMsg( const char * infoMsg, ... );

    void                        onLogEvent( uint32_t u32LogFlags, const char * logMsg ) override;
    void                        toGuiInfoMsg( char * logMsg );

signals:
    void                        signalLogMsg( const QString& logMsg );
    void                        signalInfoMsg( const QString& logMsg );

protected slots:
    void						gotoWebsite( void );
    void                        slotShowLogButtonClick( void );
    void                        slotShowAppInfoButtonClick( void );
    void                        slotLogMsg( const QString& text );
    void                        slotInfoMsg( const QString& text );
    void                        slotRunTestStatus( QString testName, ERunTestStatus testStatus, QString testMsg );

    void                        slotCopyMyUrlToClipboardClicked( void );
    void                        slotCopyTestUrlToClipboardClicked( void );
    void                        slotCopyResultToClipboardClicked( void );

    void                        slotBrowseFilesButtonClicked( void );
    void                        slotPingTestButtonClicked( void );
    void                        slotIsMyPortOpenButtonClicked( void );
    void                        slotQueryHostIdButtonClicked( void );
    void                        slotGenerateGuidButtonClicked( void );

    void                        slotNewUrlSelected( const QString &newUrl );

protected:
    void						setupApplet( void );
    void						updateDlgFromSettings( void );
    void						updateSettingsFromDlg( void );
    QPlainTextEdit *            getInfoEdit( void )     { return ui.m_InfoPlainTextEdit; }
    void                        fillBasicInfo( void );
    void                        fillExtraInfo( void );
    void                        startUrlTest( ENetCmdType netCmdType );

    QFile                       m_LogFile;
    QMutex                      m_LogMutex;
    LOG_FUNCTION                m_OldLogFunction{ nullptr };
    void *                      m_OldLogUserData{ nullptr};
    bool                        m_VerboseLog{ false };
    VxGUID                      m_SessionId;

    Ui::AppletTestAndDebugUi ui;
};


