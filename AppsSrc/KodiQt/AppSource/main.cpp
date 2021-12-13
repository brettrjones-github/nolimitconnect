/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "GoTvAppConfig.h"
#ifdef BUILD_GOTV_APP

#include "KodiQtSettings.h"
#include <CommonSrc/QtSource/AppCommon.h>
#include <CommonSrc/QtSource/HomeWindow.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif

#include "AnalogClock.h"
#include <CoreLib/VxGlobals.h>
#include "VxDataHelper.h"
#include <NetLib/VxPeerMgr.h>
#include <GuiInterface/IGoTv.h>


int main(int argc, char **argv)
{
	// for some reason QApplication must be newed or does not initialize 
	QApplication* myApp = new QApplication( argc, argv );
    IGoTv& gotv = IGoTv::getIGoTv();
    gotv.doPreStartup();

    static GoTvPlayerSettings playerSettings;
    AppCommon& appCommon = CreateAppInstance( gotv, myApp, playerSettings );




	//QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
	//QGuiApplication app( argc, argv );
	//QGuiApplication::setApplicationDisplayName( QObject::tr("NoLimitConnect Player") );
	//QCoreApplication::setOrganizationDomain( VxGetCompanyDomain() );
	// NOTE OrganizationName and ApplicationName become part of data storage location path
	//QCoreApplication::setOrganizationName( "" );
	//QCoreApplication::setApplicationName( VxGetApplicationNameNoSpaces() );
	//QCoreApplication::setApplicationVersion( VxGetAppVersionString() );

	//QCoreApplication::setOrganizationName( "QtProject" );
	//QCoreApplication::setApplicationVersion( QT_VERSION_STR );
	//MainWindow window;
//	if( !window.loadFile( parser.positionalArguments().value( 0, QLatin1String( ":/Resources/bubbles.svg" ) ) ) )
//		return -1;
	//window.show();
#ifdef TARGET_OS_WINDOWS
	//Initialize COM
	//  Initialize com fails with error about cannot change type.. I think Qt does this somewhere
	//HRESULT hr = CoInitialize(0);
	//HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
	//if( FAILED( hr ) )
	//{
	//	LogMsg( LOG_DEBUG, "CoInitializeEx failed" );
	//}

	// Initialise Winsock
	//WSADATA wd;
	//WSAStartup( MAKEWORD( 2, 2 ), &wd );
//#ifndef _DEBUG
//	// we don't want to see the "no disc in drive" windows message box
//	SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX );
//#endif
#endif // TARGET_OS_WINDOWS
	// create a thread to do application init and loading
	QThread * loaderThread = new QThread();
	appCommon.moveToThread( loaderThread );
	QObject::connect( loaderThread, SIGNAL( started() ), &appCommon, SLOT( slotStartLoadingFromThread() ) );
	loaderThread->start();

	int result = 0;

	//homePage.startup();
	//homePage.show();
	//AnalogClockWindow clock;
	//clock.show();
	//if( false == homePage.userCanceled() )
	{
		result = myApp->exec();
	}

	delete myApp;
	return result;

/*    Q_INIT_RESOURCE(gotvcommon);

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("SVG Viewer");
    QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt SVG Viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow window;
    if (!window.loadFile(parser.positionalArguments().value(0, QLatin1String(":/Resources/bubbles.svg"))))
        return -1;
    window.show();
    return app.exec();*/
}

#endif // BUILD_GOTV_APP