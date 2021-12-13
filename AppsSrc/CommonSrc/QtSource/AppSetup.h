#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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

#include "AppDefs.h"
#include <QObject>

class AppSetupWorker : public QObject 
{
    Q_OBJECT
public:
    AppSetupWorker() = default;
    ~AppSetupWorker() = default;

public slots:
    void process();

signals:
    void finished();
    void error( QString err );
    void signalSetupResult( int rc, qint64 bytesCopied );

private:
    // add your variables here
};


class AppSetup : public QObject
{
    Q_OBJECT
public:
    AppSetup() = default;
    ~AppSetup() = default;

    const char * SETUP_COMPLETE_FILE = "setup_completed.txt";

    bool areUserAssetsInitilized();
    void initializeUserAssets();

signals:
    void signalSetupResult( QObject * appSetupInst, int rc, qint64 bytesCopied );

protected slots:
    void slotError( QString err );
    void slotSetupResult( int rc, qint64 bytesCopied );

protected:
    std::string getAssetsInitilizedFileName();
};