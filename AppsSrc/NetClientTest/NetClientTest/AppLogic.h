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
// http://www.nolimitconnect.org
//============================================================================
#pragma once

#include "NlcPingResponseServer.h"
#include "AppSettings.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QSettings>

class NetClientWidget;
class NetClientThread;

class AppLogic : public QObject
{
    Q_OBJECT
public:
    explicit AppLogic();

    AppSettings&                getAppSettings() { return m_AppSettings; }
    QString                     getAppNameNoSpaces() { return m_AppSettings.getAppNameNoSpaces(); }
    QString                     getAppDomain() { return m_AppSettings.getAppDomain(); }

    void						setLogicThread( NetClientThread * renderThread ) { m_RenderThread = renderThread; }

// public slots:

protected:
    void                        registerMetaData( void );

    QString						m_AppNameNoSpaces;
    QString						m_AppDomain;
    AppSettings                 m_AppSettings;

	NetClientThread *			m_RenderThread = nullptr;

};

AppLogic& GetAppLogic( void );

