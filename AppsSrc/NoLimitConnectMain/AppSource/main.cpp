


#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppCommonConfig.h"
#ifdef BUILD_NLC_APP

#include <AppsSrc/CommonSrc/QtSource/AppCommon.h>
#include <AppsSrc/CommonSrc/QtSource/HomeWindow.h>
#include <AppsSrc/CommonSrc/QtSource/GuiParams.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QStandardPaths>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
# include <QGLFormat>
# if defined (Q_OS_ANDROID)
#  include <QtAndroid>
# endif
#else
# if defined (Q_OS_ANDROID)
#include <QCoreApplication>
#include <QtCore/QLoggingCategory>
#include <QtCore/QJniEnvironment>
#include <QtCore/private/qandroidextras_p.h>
# endif
#endif

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>
#include "AccountMgr.h"
#include <NetLib/VxPeerMgr.h>
#include <GuiInterface/INlc.h>

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

#if defined (Q_OS_ANDROID) && QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtAndroid>
const QVector<QString> permissions({"android.permission.READ_EXTERNAL_STORAGE",
                                    "android.permission.WRITE_EXTERNAL_STORAGE",
                                    "android.permission.INTERNET",
                                    "android.permission.ACCESS_WIFI_STATE",
                                    "android.permission.CHANGE_WIFI_STATE",
                                    "android.permission.ACCESS_NETWORK_STATE",
                                    "android.permission.CHANGE_NETWORK_STATE",
                                    "android.permission.RECORD_AUDIO",
                                    "android.permission.CAMERA",
                                    "android.permission.VIBRATE",
                                    "android.permission.READ_PHONE_STATE",
                                    "android.permission.KILL_BACKGROUND_PROCESSES"});

#endif

int main(int argc, char **argv)
{
    VxSetGuiThreadId();
    LogMsg( LOG_DEBUG, "Creating QApplication" );

    QApplication::setAttribute( Qt::AA_ShareOpenGLContexts );
    QApplication::setAttribute( Qt::AA_DontCheckOpenGLContextThreadAffinity );
#if !defined(TARGET_OS_ANDROID) // on android AA_EnableHighDpiScaling causes main page to be quarter size
    //if( myApp->screens().at( 0 )->geometry().width() > 1090 )
    //{
    //    QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling, true );
    //}
    //else 
    //{
    //    QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling, false );
    //}
#endif // !defined(TARGET_OS_ANDROID)

    // for some reason QApplication must be newed or does not initialize
    QApplication* myApp = new QApplication( argc, argv );

#if defined (Q_OS_ANDROID) && QT_VERSION < QT_VERSION_CHECK(6,0,0)
    //Request requiered permissions at runtime.. does not seem to work with Qt 6.2.0
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

    LogMsg( LOG_DEBUG, "permission done" );
#endif
#if defined (Q_OS_ANDROID)

    const QString externStoragePemission(QLatin1String ("android.permission.WRITE_EXTERNAL_STORAGE"));
    auto storagePermissionResult = QtAndroidPrivate::checkPermission(externStoragePemission).result();
    if( storagePermissionResult != QtAndroidPrivate::Authorized )
    {
        if( QtAndroidPrivate::Authorized !=  QtAndroidPrivate::requestPermission(externStoragePemission).result() )
        {
            LogMsg(LOG_INFO, "Cannot Proceed without external storage permission");
            return 0;
        }
    }
#endif // defined (Q_OS_ANDROID)

    // initialize display scaling etc
    // the best method I have found to scale the gui is to use the default font height as the scaling factor
    QFontMetrics fontMetrics( myApp->font() );
    GuiParams::initGuiParams(fontMetrics.height());

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

    INlc& gotv = INlc::getINlc();
    gotv.doPreStartup();

    GetPtoPEngine(); // engine first.. there is some interdependencies
    AppCommon& appCommon = CreateAppInstance( gotv, myApp );

    appCommon.loadWithoutThread();


#if ENABLE_KODI
    // send command line parameters to Kodi
	static CAppParamParser appParamParser;
    appParamParser.Parse( argv, argc );
    INlc::getINlc().initRun( appParamParser );
#endif // ENABLE_KODI

	int result = 0;

	//homePage.startup();
	//homePage.show();
	//if( false == homePage.userCanceled() )
	{
		result = myApp->exec();
	}

    //nolimit.doPreShutdown();
	//delete myApp;
	return result;
}

#endif // BUILD_NLC_APP
