
#include <app_precompiled_hdr.h>

#include "GoTvAppConfig.h"
#ifdef BUILD_GOTV_APP

#include <GoTvApps/GoTvCommon/QtSource/AppCommon.h>
#include <GoTvApps/GoTvCommon/QtSource/HomeWindow.h>
#include <GoTvApps/GoTvCommon/QtSource/GuiParams.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QStandardPaths>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>
#include <GoTvApps/GoTvCommon/QtSource/AccountMgr.h>
#include <NetLib/VxPeerMgr.h>
#include <GuiInterface/IGoTv.h>

namespace{
    void setupRootStorageDirectory()
    {
        std::string strRootUserDataDir;

        //=== determine root path to store all application data and settings etc ===//

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QString dataPath = QDesktopServices::storageLocation( QDesktopServices::DataLocation );
#else
        QString dataPath = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
#endif //TARGET_OS_WINDOWS
        strRootUserDataDir = dataPath.toUtf8().constData();

#ifdef DEBUG
        // remove the D from the end so release and debug builds use the same storage directory
        if( !strRootUserDataDir.empty() && ( strRootUserDataDir.c_str()[ strRootUserDataDir.length() - 1 ] == 'D' ) )
        {
            strRootUserDataDir = strRootUserDataDir.substr( 0, strRootUserDataDir.length() - 1 );
        }
#endif // DEBUG

        VxFileUtil::makeForwardSlashPath( strRootUserDataDir );
        strRootUserDataDir += "/";
        // No need to put application in path because when call QCoreApplication::setApplicationName("MyP2PWeb")
        // it made it a sub directory of DataLocation
        VxSetRootDataStorageDirectory( strRootUserDataDir.c_str() );
    }
}

#if defined (Q_OS_ANDROID)
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
# include <QtAndroid>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
const QVector<QString> permissions({"android.permission.READ_EXTERNAL_STORAGE",
                                    "android.permission.WRITE_EXTERNAL_STORAGE",
                                    "android.permission.INTERNET",
                                    "android.permission.ACCESS_WIFI_STATE",
                                    "android.permission.CHANGE_WIFI_STATE",
                                    "android.permission.ACCESS_NETWORK_STATE",
                                    "android.permission.RECORD_AUDIO",
                                    "android.permission.CAMERA",
                                    "android.permission.VIBRATE",
                                    "android.permission.READ_PHONE_STATE",
                                    "android.permission.KILL_BACKGROUND_PROCESSES"});
#endif

int main(int argc, char **argv)
{
    // for some reason QApplication must be newed or does not initialize
    QApplication* myApp = new QApplication( argc, argv );

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
# if defined(Q_OS_ANDROID)
    //Request requiered permissions at runtime
    for(const QString &permission : permissions)
    {
        LogMsg( LOG_DEBUG, "requesting permission %s", permission.toUtf8().constData() );
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied)
        {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
            {
                LogMsg( LOG_DEBUG, "DENIED permission %s", permission.toUtf8().constData() );
                delete myApp;
                return 0;
            }

            LogMsg( LOG_DEBUG, "ACCEPTED permission %s", permission.toUtf8().constData() );
        }
    }
# endif
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

    LogMsg( LOG_DEBUG, "permission done" );
    QApplication::setAttribute( Qt::AA_ShareOpenGLContexts );
    QApplication::setAttribute( Qt::AA_DontCheckOpenGLContextThreadAffinity );



#if !defined(TARGET_OS_ANDROID) // on android it causes main page to be quarter size
    //if( myApp->screens().at( 0 )->geometry().width() > 1090 )
    //{
    //    QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling, true );
    //}
    //else 
    //{
    //    QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling, false );
    //}
#endif // !defined(TARGET_OS_ANDROID)

    // initialize display scaling etc
    GuiParams::initGuiParams(10);

    // chicken and egg kind of thing.. we need the storage path here
    QGuiApplication::setApplicationDisplayName( VxGetApplicationTitle() );
    QCoreApplication::setOrganizationDomain( VxGetCompanyDomain() );
    // NOTE OrganizationName and ApplicationName become part of data storage location path
    QCoreApplication::setOrganizationName( "" ); // leave blank or will become part of data storage path
    QCoreApplication::setApplicationName( VxGetApplicationNameNoSpaces() );
    QCoreApplication::setApplicationVersion( VxGetAppVersionString() );

    // is this needed ? Q_INIT_RESOURCE( gotvcommon );

    // TODO allow user to change where the data is stored
    setupRootStorageDirectory();

    IGoTv& gotv = IGoTv::getIGoTv();
    gotv.doPreStartup();

    AppCommon& appCommon = CreateAppInstance( gotv, myApp );

	// create a thread to do application init and loading .. causes issues with QThreadDataStorage on linux so just load directly
#if 0
	//QThread * loaderThread = new QThread();
	//appCommon.moveToThread( loaderThread );
	//QObject::connect( loaderThread, SIGNAL( started() ), &appCommon, SLOT( slotStartLoadingFromThread() ) );
	//loaderThread->start();
#else
    appCommon.loadWithoutThread();
#endif // 0

    // send command line parameters to Kodi
	static CAppParamParser appParamParser;
    appParamParser.Parse( argv, argc );
    GetIGoTv().initRun( appParamParser );

	int result = 0;

	//homePage.startup();
	//homePage.show();
	//if( false == homePage.userCanceled() )
	{
		result = myApp->exec();
	}

    //gotv.doPreShutdown();
	//delete myApp;
	return result;
}

#endif // BUILD_GOTV_APP
