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

#include <QSettings>

class AppSettings : public QSettings
{
public:
    AppSettings();
    ~AppSettings() = default;

    QString&                    getAppNameNoSpaces()    { return m_AppName; }
    QString&                    getAppDomain()          { return m_AppDomain; }

 
protected:
    QString                     m_AppName;
    QString                     m_AppDomain;
    QString                     m_SettingsFile;
};