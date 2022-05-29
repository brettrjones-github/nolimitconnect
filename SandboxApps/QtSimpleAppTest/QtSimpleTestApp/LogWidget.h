//============================================================================
// Copyright (C) 2018 Brett R. Jones
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
#pragma once

#include <QPlainTextEdit>
#include <QFile>
#include <QMutex>

class LogWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    LogWidget( QWidget * parent );

    void                        toGuiLog( uint32_t u32LogFlags, char * logMsg );

signals:
    void                        signalLogMsg( const QString& logMsg );

protected slots:
    void                        slotLogMsg( const QString& text );

private:
    QFile                       m_LogFile;
    QMutex                      m_LogMutex;
};
