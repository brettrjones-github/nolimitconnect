#include "AppLogic.h"
#include "NetWidget.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktConnectSimple.h>

#include <QStandardPaths>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

AppLogic& GetAppLogic( void )
{
    static AppLogic appLogic;
    return appLogic;
}


//============================================================================
AppLogic::AppLogic( )
    : m_AppSettings()
{
    registerMetaData();
#ifdef TARGET_OS_WINDOWS
    WSADATA wd;
    if( WSAStartup( MAKEWORD( 2, 2 ), &wd ) != 0 )
    {
        LogMsg( LOG_ERROR, " WSAStartup failed" );
    }
#endif
}

//============================================================================
void AppLogic::registerMetaData( void )
{
    qRegisterMetaType<uint32_t>( "uint32_t" );
}
