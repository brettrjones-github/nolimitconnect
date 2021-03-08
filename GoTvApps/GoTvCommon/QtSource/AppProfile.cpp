//============================================================================
// Copyright (C) 2012 Brett R. Jones 
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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppProfile.h"

#include <CoreLib/VxProfile.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <QString>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif // TARGET_OS_WINDOWS

#include <stdio.h>

#define INI_FILE "MyP2PWeb.ini"

//============================================================================
AppProfile::AppProfile()
	: m_u16TcpPort(0)
	, m_u16UdpPort(0)
	, m_u32EnableDebug(0)
	, m_u32DisableUdp(0)
	, m_u32LogFlags(0xff)
	, m_u32LogToFile( 0 )
	, m_strLocalHostIp( "" )
{
}

//============================================================================
void AppProfile::loadProfile( void )
{
	// if has ini file then use settings in it 
	getExecuteDirectory( m_strExeDir );

	std::string strIniFileName = m_strExeDir + INI_FILE;

	uint32_t u32UseExeDir = 0;
	if( fileExists( strIniFileName.c_str() ) )
	{

		m_u16TcpPort = (uint16_t)getProfileLong( 
			strIniFileName.c_str(),
			"network",
			"TcpPort",
			0 );

		m_u16UdpPort = (uint16_t)getProfileLong( 
			strIniFileName.c_str(),
			"network",
			"UdpPort",
			0 );

		getProfileString( 
			strIniFileName.c_str(),
			"network",
			"LocalHostIp",
			"",
			m_strLocalHostIp );

		m_u32EnableDebug = (uint32_t)getProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"EnableDebug",
			0 );

		m_u32DisableUdp = (uint32_t)getProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"DisableUdp",
			0x0 );

		m_u32LogFlags = (uint32_t)getProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"LogFlags",
			0x7fffffff );

		m_u32LogToFile = (uint32_t)getProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"LogToFile",
			0 );

		getProfileString( 
			strIniFileName.c_str(),
			"id",
			"UserName",
			"",
			m_strUserName );

		getProfileString( 
			strIniFileName.c_str(),
			"id",
			"guid",
			"",
			m_strUserGuid );

		getProfileString( 
			strIniFileName.c_str(),
			"id",
			"network",
			"",
			m_strNetworkName );

		m_u32NetServiceOnly = getProfileLong( 
			strIniFileName.c_str(),
			"netservices",
			"NetServiceOnly",
			0x0 );

		u32UseExeDir = (uint32_t)getProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"UseExeDirectory",
			0x0 );
	}

	if( u32UseExeDir )
	{
        m_strRootUserDataDir = m_strExeDir + VxGetApplicationNameNoSpacesLowerCase();
		m_strRootUserDataDir += "/";
		makeDirectory( m_strRootUserDataDir.c_str() );
		m_strRootXferDir = m_strRootUserDataDir;

		m_strRootUserDataDir += "data/";
		makeDirectory( m_strRootUserDataDir.c_str() );

		m_strRootXferDir  += "xfer/";
		makeDirectory( m_strRootXferDir.c_str() );
	}
	else
	{
		//=== determine root path to store all application data and settings etc ===//
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
        QString dataPath =  QStandardPaths::writableLocation(QStandardPaths::AppDataLocation );
#endif //TARGET_OS_WINDOWS
		m_strRootUserDataDir = dataPath.toUtf8().constData();

#ifdef DEBUG
		// remove the D from the end so release and debug builds use the same storage directory
		if( !m_strRootUserDataDir.empty() && ( m_strRootUserDataDir.c_str()[m_strRootUserDataDir.length() - 1] == 'D' ) )
		{
			m_strRootUserDataDir = m_strRootUserDataDir.substr( 0, m_strRootUserDataDir.length() - 1 );
		}
#endif // DEBUG

		VxFileUtil::makeForwardSlashPath( m_strRootUserDataDir );
        VxFileUtil::assurePathEndWithSlash( m_strRootUserDataDir );

		// No need to put application in path because when call QCoreApplication::setApplicationName("MyP2PWeb")
        makeDirectory( m_strRootUserDataDir.c_str() );

		int exePathHash = 0;
#if defined(TARGET_OS_ANDROID)
        // in some cases android may give a new exe directory for each launch and we get a different hash each time
        // so for android only use the data directory
        if( m_strRootUserDataDir.length() )
        {
            const char * exePath = m_strRootUserDataDir.c_str();
            for( unsigned int i = 0; i < m_strRootUserDataDir.length(); i++ )
            {
                exePathHash += exePath[ i ];
            }
        }

        char as8ExePathHash[32];
        sprintf( as8ExePathHash, "nlc%x/", exePathHash );
        // android does not allow numbers in file path so substitute )
        for( size_t i = 0; i < strlen(as8ExePathHash); i++)
        {
            if( !isalpha(as8ExePathHash[i]))
            {
                as8ExePathHash[i] = 'z';
            }
        }

        m_NlcPathPrefix += as8ExePathHash;

#else
        // use exe directory to make a hash as part of path so can run multiple instances
		if( m_strExeDir.length() )
		{
			const char * exePath = m_strExeDir.c_str();
            for( unsigned int i = 0; i < m_strExeDir.length(); i++ )
			{
				exePathHash += exePath[ i ];
			}
		}

        char as8ExePathHash[32];
        sprintf( as8ExePathHash, "nlc%x/", exePathHash );
        m_NlcPathPrefix += as8ExePathHash;

#endif // defined(TARGET_OS_ANDROID)

        VxFileUtil::assurePathEndWithSlash( m_strRootUserDataDir );
        makeDirectory( m_strRootUserDataDir.c_str() );
        if(!VxFileUtil::directoryExists(m_strRootUserDataDir.c_str()))
        {
            LogMsg( LOG_ERROR, "AppProfile::loadProfile Could not create root data dir %s", m_strRootUserDataDir.c_str());
        }

        m_strRootUserDataDir += m_NlcPathPrefix;
        makeDirectory( m_strRootUserDataDir.c_str() );
        if(!VxFileUtil::directoryExists(m_strRootUserDataDir.c_str()))
        {
            LogMsg( LOG_ERROR, "AppProfile::loadProfile Could not create prefixed root data dir %s", m_strRootUserDataDir.c_str());
        }

        // it made it a sub directory of DataLocation
        VxSetRootDataStorageDirectory( m_strRootUserDataDir.c_str() );

		VxSetRootUserDataDirectory( m_strRootUserDataDir.c_str() );

		//=== determine root path for data xfer like incomplete/downloads/uploads etc ===//
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
	#ifdef TARGET_OS_WINDOWS
			QString docsPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
	#else
			// linux hides document under .local so use desktop if possible
			QString docsPath = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
			if( docsPath.isEmpty() )
			{
				docsPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
			}
	#endif // TARGET_OS_WINDOWS		
#else
    #if defined(TARGET_OS_WINDOWS) || defined(TARGET_OS_ANDROID)
        QString docsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	#else
		// linux hides document under .local so use desktop if possible
		QString docsPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
		if( docsPath.isEmpty() )
		{
			docsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
		}
	#endif // TARGET_OS_WINDOWS		
#endif //QT_VERSION < QT_VERSION_CHECK

		m_strRootXferDir = docsPath.toUtf8().constData();

		VxFileUtil::makeForwardSlashPath( m_strRootXferDir );
        VxFileUtil::assurePathEndWithSlash( m_strRootXferDir );

        m_strRootXferDir += VxGetApplicationNameNoSpacesLowerCase();
        VxFileUtil::assurePathEndWithSlash( m_strRootXferDir );

		makeDirectory( m_strRootXferDir.c_str() );
		m_strRootXferDir += m_NlcPathPrefix;
        VxFileUtil::assurePathEndWithSlash( m_strRootXferDir );
		makeDirectory( m_strRootXferDir.c_str() );
        if(!VxFileUtil::directoryExists(m_strRootXferDir.c_str()))
        {
            LogMsg( LOG_ERROR, "AppProfile::loadProfile Could not create xfer dir %s", m_strRootXferDir.c_str());
        }
	}

	LogMsg( LOG_INFO, "User Data Dir %s Xfer Dir %s app data dir %s doc dir %s\n", 
			m_strRootUserDataDir.c_str(),
			m_strRootXferDir.c_str(),
			getOsSpecificAppDataDir().c_str(),
			getOsSpecificHomeDir().c_str()
			);
	//LogMsg( LOG_INFO, "App Data Directory %s\n", m_strRootAppDataDir.c_str() );

	std::string strLogDataDir = m_strRootUserDataDir + "logs/";
	makeDirectory( strLogDataDir.c_str() );
	m_strDebugFileName = strLogDataDir + "DebugLog.txt";

	if( m_u32LogToFile )
	{
		VxSetLogToFile( m_strDebugFileName.c_str() );
	}
}

//============================================================================
RCODE AppProfile::saveDebugSettings( void )
{
	RCODE rc = 0;
	getExecuteDirectory( m_strExeDir );
	std::string strIniFileName = m_strExeDir + INI_FILE;

	if( fileExists( strIniFileName.c_str() ) )
	{
		rc |= setProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"DisableUdp",
			m_u32DisableUdp );

		rc |= setProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"LogFlags",
			m_u32LogFlags );

		rc |= setProfileLong( 
			strIniFileName.c_str(),
			"debug",
			"LogToFile",
			m_u32LogToFile );
	}
	else
	{
		rc = -1;
	}
	return rc;
}

////============================================================================
//void AppProfile::setUserSpecificDataDir( std::string& strDir )
//{
//	m_strUserSpecificDataDir = strDir;
//}
//
////============================================================================
//const char * AppProfile::getUserSpecificDataDir()
//{
//	return m_strUserSpecificDataDir.c_str();
//}

//============================================================================
const char * AppProfile::getLocalHostIp()
{
	return m_strLocalHostIp.c_str();
}

//============================================================================
std::string& AppProfile::getOsSpecificAppDataDir( void )
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString dataLocation = paths[0];
#else
	QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif //QT_5_OR_GREATER
	m_strOsSpecificAppDataDir = dataLocation.toStdString();
	makeForwardSlashPath( m_strOsSpecificAppDataDir );
	m_strOsSpecificAppDataDir += "/";
    m_strOsSpecificHomeDir += m_NlcPathPrefix;
    makeDirectory( m_strOsSpecificHomeDir.c_str() );
    if(!VxFileUtil::directoryExists(m_strOsSpecificHomeDir.c_str()))
    {
        LogMsg( LOG_ERROR, "AppProfile::getOsSpecificDocumentsDir Could not create app data dir %s", m_strOsSpecificHomeDir.c_str());
    }

	return m_strOsSpecificAppDataDir;
}

//============================================================================
std::string& AppProfile::getOsSpecificHomeDir( void )
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString homeLocation = paths[0];
#else
    QString homeLocation = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
#endif //QT_5_OR_GREATER
	m_strOsSpecificHomeDir = homeLocation.toStdString();
	makeForwardSlashPath( m_strOsSpecificHomeDir );
    VxFileUtil::assurePathEndWithSlash( m_strOsSpecificHomeDir );
    m_strOsSpecificHomeDir += m_NlcPathPrefix;
    makeDirectory( m_strOsSpecificHomeDir.c_str() );
    if(!VxFileUtil::directoryExists(m_strOsSpecificHomeDir.c_str()))
    {
        LogMsg( LOG_ERROR, "AppProfile::getOsSpecificDocumentsDir Could not create home dir %s", m_strOsSpecificHomeDir.c_str());
    }

	return m_strOsSpecificHomeDir;
}

//============================================================================
std::string& AppProfile::getOsSpecificDocumentsDir( void )
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
	QString docsLocation = paths[0];
#else
	QString docsLocation = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#endif //QT_5_OR_GREATER
	m_strOsSpecificDocumentsDir = docsLocation.toStdString();
	makeForwardSlashPath( m_strOsSpecificDocumentsDir );
    VxFileUtil::assurePathEndWithSlash( m_strOsSpecificDocumentsDir );
    m_strOsSpecificDocumentsDir += m_NlcPathPrefix;
    makeDirectory( m_strOsSpecificDocumentsDir.c_str() );
    if(!VxFileUtil::directoryExists(m_strOsSpecificDocumentsDir.c_str()))
    {
        LogMsg( LOG_ERROR, "AppProfile::getOsSpecificDocumentsDir Could not create documents dir %s", m_strOsSpecificDocumentsDir.c_str());
    }

	return m_strOsSpecificDocumentsDir;
}
