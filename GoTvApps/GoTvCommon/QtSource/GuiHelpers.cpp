//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
#include "GuiHelpers.h"
#include "GuiParams.h"

#include "ActivityBase.h"
#include "AppletBase.h"
#include "AppCommon.h"
#include "AppTranslate.h"
#include "MyIconsDefs.h"
#include "VxFrame.h"
#include "PluginSettingsWidget.h"

#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/ObjectCommon.h>

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <VxVideoLib/VxVideoLib.h>
#include <VxVideoLib/VxVidCap.h>


#include <QDesktopServices>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QLineEdit>
#include <QUrl>


//============================================================================
bool GuiHelpers::isCameraSourceAvailable()
{
    bool cameraAvail = false;
#ifdef TARGET_OS_WINDOWS
    IVxVidCap * vidCap = VxGetVidCapInterface();
    if( vidCap )
    {
        if( 0 != vidCap->startupVidCap() )
        {
            cameraAvail = true;
        }

        vidCap->shutdownVidCap();
    }
#endif // TARGET_OS_WINDOWS

    return cameraAvail;
}

//============================================================================
QString GuiHelpers::getJustFileName( QString& fileNameAndPath )
{
	QString justFileName("");
	if( !fileNameAndPath.isEmpty() )
	{
		int lastSlashIdx = fileNameAndPath.lastIndexOf('/');
		if( lastSlashIdx >= 0 )
		{
			justFileName = fileNameAndPath.right( fileNameAndPath.length() - ( lastSlashIdx + 1 ) );
		}

		if( justFileName.isEmpty() )
		{
			justFileName = fileNameAndPath;
		}
	}

	return justFileName;
}

//============================================================================
void GuiHelpers::splitPathAndFileName( QString& fileNameAndPath, QString& retFileName, QString& retPath )
{
	retFileName = "";
	retPath = "";
	if( !fileNameAndPath.isEmpty() )
	{
		int lastSlashIdx = fileNameAndPath.lastIndexOf('/');
		if( lastSlashIdx >= 0 )
		{
			retFileName = fileNameAndPath.right( fileNameAndPath.length() - ( lastSlashIdx + 1 ) );
		}

		if( retFileName.isEmpty() )
		{
			retPath = fileNameAndPath;
			retFileName = fileNameAndPath;
		}
		else
		{
			retPath = fileNameAndPath.left( lastSlashIdx );
		}
	}
}

//============================================================================
QString GuiHelpers::getAvailableStorageSpaceText()
{
    return GuiParams::describeFileLength( VxFileUtil::getDiskFreeSpace( VxGetAppDirectory( eAppDirRootDataStorage ).c_str() ) );
}

//============================================================================
QString GuiHelpers::browseForDirectory( QString startDir, QWidget * parent )
{

	QString dir = QFileDialog::getExistingDirectory(
		parent, 
		QObject::tr("Open Directory"),
		startDir, 
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if( false == dir.isEmpty() )
	{
		dir.replace( "\\", "/" );
		if( '/' != dir.at(dir.length() - 1) )
		{
			dir.append( "/" );
		}
	}

	return dir;
}

//============================================================================
bool GuiHelpers::copyResourceToOnDiskFile( QString resourcePath, QString fileNameAndPath )
{
	bool resourceCopied = false;
	//QFileInfo resPathInfo( resourceFile );
	QFile resfile( resourcePath );
	QFile onDiskFile( fileNameAndPath );
	if( resfile.open( QIODevice::ReadOnly ) )
	{
		if( onDiskFile.open( QIODevice::ReadWrite ) )
		{
			if( 0 == onDiskFile.write( resfile.readAll() ) )
			{
				qWarning() << "Could not write app resource file " << fileNameAndPath;
			}
			else
			{
				resourceCopied = true;
			}

			onDiskFile.close();
		}
		else
		{
			qWarning() << "Could not open app resource file for writing " << fileNameAndPath;
		}

		resfile.close();
	}
	else
	{
		qWarning() << "Could not open resource file " << resourcePath;
	}

	return resourceCopied;
}

//============================================================================
bool GuiHelpers::playFile( AppCommon& myApp, QString fullFileName, int pos0to100000 )
{
    if( fullFileName.isEmpty() )
    {
        myApp.toGuiUserMessage( "GuiHelpers::playFile Empty File Name" );
        return false;
    }

    uint8_t fileType;
    uint64_t fileLen;
    if( !VxFileUtil::getFileTypeAndLength( fullFileName.toUtf8().constData(), fileLen, fileType ) )
    {
        myApp.toGuiUserMessage( "File no longer available %s", fullFileName.toUtf8().constData() );
        return false;
    }

    if( VXFILE_TYPE_DIRECTORY & fileType )
    {
#ifdef TARGET_OS_WINDOWS
        ShellExecuteA( 0, 0, fullFileName.toUtf8().constData(), 0, 0, SW_SHOW );
#else
        QDesktopServices::openUrl( QUrl::fromLocalFile( fullFileName ) );
#endif // TARGET_OS_WINDOWS
    }
    else
    {
        // is file
        if( VxShouldOpenFile( fileType ) )
        {
            if( fileType & ( VXFILE_TYPE_AUDIO | VXFILE_TYPE_VIDEO ) )
            {
                // can be played
                return myApp.getFromGuiInterface().fromGuiPlayLocalMedia( fullFileName.toUtf8().constData(), fileLen, fileType, pos0to100000 );
            }

#ifdef TARGET_OS_WINDOWS
            ShellExecuteA( 0, 0, fullFileName.toUtf8().constData(), 0, 0, SW_SHOW );
#else
            QDesktopServices::openUrl( QUrl::fromLocalFile( fullFileName ) );
#endif // TARGET_OS_WINDOWS
        }
        else
        {
            myApp.toGuiUserMessage( "File Type Not Supported" );
            return false;
        }
    }

    return true;
}

//============================================================================
int GuiHelpers::calculateTextHeight( QFontMetrics& fontMetrics, QString textStr )
{
	int textHeight = 0;
	if( textStr.isEmpty() )
	{
		textHeight = fontMetrics.height();
	}
	else
	{
		int lineCount = textStr.count( QLatin1Char('\n') ) + 1;
		textHeight = lineCount * fontMetrics.height();
	}

	return textHeight;
}

//============================================================================
bool GuiHelpers::isAppletAService( EApplet applet )
{
    return ( ( eAppletServiceAboutMe == applet )
             || ( eAppletServiceAvatarImage == applet )
             || ( eAppletServiceConnectionTest == applet )
             || ( eAppletServiceHostGroup == applet )
             || ( eAppletServiceHostNetwork == applet )
             || ( eAppletServiceRandomConnect == applet )
             || ( eAppletServiceRandomConnectRelay == applet )
             || ( eAppletServiceShareFiles == applet )
             || ( eAppletServiceShareWebCam == applet )
             || ( eAppletServiceStoryboard == applet )
             || ( eAppletServiceRelay == applet )
             );
}

//============================================================================
bool GuiHelpers::isAppletAClient( EApplet applet )
{
    return ( ( eAppletClientAboutMe == applet )
             || ( eAppletClientAvatarImage == applet )
             || ( eAppletClientConnectionTest == applet )
             || ( eAppletClientHostGroup == applet )
             || ( eAppletClientHostGroupListing == applet )
             || ( eAppletClientRandomConnect == applet )
             || ( eAppletClientRandomConnectRelay == applet )
             || ( eAppletClientShareFiles == applet )
             || ( eAppletClientShareWebCam == applet )
             || ( eAppletClientStoryboard == applet )
             || ( eAppletClientRelay == applet )
             );
}

//============================================================================
EPluginType GuiHelpers::getAppletAssociatedPlugin( EApplet applet )
{
    EPluginType pluginType = ePluginTypeInvalid;
    switch( applet )
    {
    case eAppletClientAboutMe:              return ePluginTypeAboutMePage;
    case eAppletClientAvatarImage:          return ePluginTypeAvatarImage;
    case eAppletClientConnectionTest:       return ePluginTypeConnectTestClient;
    case eAppletClientHostGroup:            return ePluginTypeGroupHost;
    case eAppletClientHostGroupListing:     return ePluginTypeNetworkSearchList;
    case eAppletClientHostNetwork:          return ePluginTypeNetworkHost;
    case eAppletClientRandomConnect:        return ePluginTypeRandomConnectClient;
    case eAppletClientRandomConnectRelay:   return ePluginTypeRandomConnectHost;
    case eAppletClientShareFiles:           return ePluginTypeFileServer;
    case eAppletClientShareWebCam:          return ePluginTypeCamServer;
    case eAppletClientStoryboard:           return ePluginTypeStoryboard;
    case eAppletClientRelay:                return ePluginTypeRelay;

    case eAppletServiceAboutMe:              return ePluginTypeAboutMePage;
    case eAppletServiceAvatarImage:          return ePluginTypeAvatarImage;
    case eAppletServiceConnectionTest:       return ePluginTypeConnectTestHost;
    case eAppletServiceHostGroup:            return ePluginTypeGroupHost;
    case eAppletServiceHostNetwork:          return ePluginTypeNetworkHost;
    case eAppletServiceRandomConnect:        return ePluginTypeRandomConnectClient;
    case eAppletServiceRandomConnectRelay:   return ePluginTypeRandomConnectHost;
    case eAppletServiceShareFiles:           return ePluginTypeFileServer;
    case eAppletServiceShareWebCam:          return ePluginTypeCamServer;
    case eAppletServiceStoryboard:           return ePluginTypeStoryboard;
    case eAppletServiceRelay:                return ePluginTypeRelay;

    case eAppletSettingsAboutMe:            return ePluginTypeAboutMePage;
    case eAppletSettingsAvatarImage:        return ePluginTypeAvatarImage;
    case eAppletSettingsWebCamServer:       return ePluginTypeCamServer;
    case eAppletSettingsConnectTest:        return ePluginTypeConnectTestHost;
    case eAppletSettingsShareFiles:         return ePluginTypeFileServer;
    case eAppletSettingsFileXfer:           return ePluginTypeFileXfer;
    case eAppletSettingsHostChatRoom:       return ePluginTypeChatRoomHost;
    case eAppletSettingsHostGroup:          return ePluginTypeGroupHost;
    // case eAppletSettingsHostGroupListing:   return ePluginTypeGroupBlobing;
    case eAppletSettingsHostNetwork:        return ePluginTypeNetworkHost;
    case eAppletSettingsHostRandomConnect:  return ePluginTypeRandomConnectHost;
    case eAppletSettingsMessenger:          return ePluginTypeMessenger;
    case eAppletSettingsRandomConnect:      return ePluginTypeRandomConnectClient;
    case eAppletSettingsRandomConnectRelay: return ePluginTypeRandomConnectHost;
    // case eAppletSettingsRelay:              return ePluginTypeRelay;
    case eAppletSettingsStoryboard:         return ePluginTypeStoryboard;
    case eAppletSettingsTruthOrDare:        return ePluginTypeTruthOrDare;
    case eAppletSettingsVideoPhone:         return ePluginTypeVideoPhone;
    case eAppletSettingsVoicePhone:         return ePluginTypeVoicePhone;


    default:
        break;
    }

    return pluginType;
}

//============================================================================
EApplet GuiHelpers::pluginTypeToEditApplet( EPluginType pluginType )
{
    EApplet appletType = eAppletUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eAppletEditAboutMe;
    case ePluginTypeAvatarImage:            return eAppletEditAvatarImage;
    case ePluginTypeStoryboard:             return eAppletEditStoryboard;
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeConnectTestHost:        return eAppletSettingsConnectTest;
    case ePluginTypeFileServer:             return eAppletUnknown;
    case ePluginTypeFileXfer:               return eAppletUnknown;
    case ePluginTypeGroupHost:              return eAppletUnknown;
    case ePluginTypeNetworkSearchList:      return eAppletUnknown;
    case ePluginTypeNetworkHost:            return eAppletUnknown;
    case ePluginTypeRandomConnectClient:    return eAppletUnknown;
    case ePluginTypeRandomConnectHost:      return eAppletUnknown;
    case ePluginTypeRelay:                  return eAppletUnknown;
    default:
        break;
    }

    return appletType;
}

//============================================================================
EApplet GuiHelpers::pluginTypeToSettingsApplet( EPluginType pluginType )
{
    EApplet appletType = eAppletUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eAppletSettingsAboutMe;
    case ePluginTypeAvatarImage:            return eAppletSettingsAvatarImage;
    case ePluginTypeCamServer:              return eAppletSettingsWebCamServer;
    case ePluginTypeConnectTestHost:        return eAppletSettingsConnectTest;
    case ePluginTypeFileServer:             return eAppletSettingsShareFiles;
    case ePluginTypeFileXfer:               return eAppletSettingsFileXfer;
    case ePluginTypeChatRoomHost:           return eAppletSettingsHostChatRoom;
    case ePluginTypeGroupHost:              return eAppletSettingsHostGroup;
        // case ePluginTypeGroupBlobing:       return eAppletSettingsHostGroupListing;
    case ePluginTypeNetworkHost:            return eAppletSettingsHostNetwork;
    case ePluginTypeRandomConnectHost:      return eAppletSettingsHostRandomConnect;
    case ePluginTypeMessenger:              return eAppletSettingsMessenger;
    case ePluginTypeRandomConnectClient:    return eAppletSettingsRandomConnect;
    // case ePluginTypeRandomConnectHost:      return eAppletSettingsRandomConnectRelay;
        // case ePluginTypeRelay:                  return eAppletSettingsRelay;
    case ePluginTypeStoryboard:             return eAppletSettingsStoryboard;
    case ePluginTypeTruthOrDare:            return eAppletSettingsTruthOrDare;
    case ePluginTypeVideoPhone:             return eAppletSettingsVideoPhone;
    case ePluginTypeVoicePhone:             return eAppletSettingsVoicePhone;

    default:
        break;
    }

    return appletType;
}

//============================================================================
EApplet GuiHelpers::pluginTypeToViewApplet( EPluginType pluginType )
{
    EApplet appletType = eAppletUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eAppletEditAboutMe;
    case ePluginTypeAvatarImage:            return eAppletEditAvatarImage;
    case ePluginTypeStoryboard:             return eAppletEditStoryboard;
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeConnectTestHost:        return eAppletSettingsConnectTest;
    case ePluginTypeFileServer:             return eAppletUnknown;
    case ePluginTypeFileXfer:               return eAppletUnknown;
    case ePluginTypeGroupHost:              return eAppletUnknown;
    case ePluginTypeNetworkSearchList:      return eAppletUnknown;
    case ePluginTypeNetworkHost:            return eAppletUnknown;
    case ePluginTypeRandomConnectClient:    return eAppletUnknown;
    case ePluginTypeRandomConnectHost:      return eAppletUnknown;
    case ePluginTypeRelay:                  return eAppletUnknown;
    default:
        break;
    }

    return appletType;
}

//============================================================================
EApplet GuiHelpers::pluginTypeToUserApplet( EPluginType pluginType )
{
    EApplet appletType = eAppletUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eAppletEditAboutMe;
    case ePluginTypeAvatarImage:            return eAppletEditAvatarImage;
    //case ePluginTypeCamServer:              return eAppletSettingsWebCamServer;
    // case ePluginTypeConnectTestHost:     return eAppletSettingsConnectTest;
    //case ePluginTypeFileServer:             return eAppletShareFiles;
    // case ePluginTypeFileXfer:               return eAppletSettingsFileXfer;
    case ePluginTypeChatRoomHost:           return eAppletSettingsHostChatRoom;
    case ePluginTypeGroupHost:              return eAppletSettingsHostGroup;
    // case ePluginTypeGroupBlobing:       return eAppletSettingsHostGroupListing;
    case ePluginTypeNetworkHost:            return eAppletSettingsHostNetwork;
    case ePluginTypeRandomConnectHost:      return eAppletSettingsHostRandomConnect;
    //case ePluginTypeMessenger:              return eAppletSettingsMessenger;
    case ePluginTypeRandomConnectClient:          return eAppletServiceRandomConnect;
    //case ePluginTypeRandomConnectHost:     return eAppletServiceRandomConnectRelay;
    case ePluginTypeRelay:                  return eAppletServiceRelay;
    case ePluginTypeStoryboard:             return eAppletEditStoryboard;
    //case ePluginTypeTruthOrDare:            return eAppletSettingsTruthOrDare;
    //case ePluginTypeVideoPhone:             return eAppletSettingsVideoPhone;
    //case ePluginTypeVoicePhone:             return eAppletSettingsVoicePhone;

    default:
        break;
    }

    return appletType;
}

//============================================================================
EMyIcons GuiHelpers::pluginTypeToSettingsIcon( EPluginType pluginType )
{
    EMyIcons iconType = eMyIconUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eMyIconSettingsAboutMe;
    case ePluginTypeAvatarImage:            return eMyIconSettingsAvatarImage;
    case ePluginTypeCamServer:              return eMyIconSettingsShareWebCam;
    case ePluginTypeConnectTestHost:        return eMyIconSettingsConnectionTest;
    case ePluginTypeFileServer:             return eMyIconSettingsShareFiles;
    case ePluginTypeFileXfer:               return eMyIconSettingsFileXfer;
    case ePluginTypeGroupHost:              return eMyIconSettingsHostGroup;
    case ePluginTypeNetworkSearchList:      return eMyIconSettingsHostGroupListing;
    case ePluginTypeNetworkHost:            return eMyIconSettingsHostNetwork;
    case ePluginTypeMessenger:              return eMyIconSettingsMessenger;
    case ePluginTypeRandomConnectClient:    return eMyIconSettingsRandomConnect;
    case ePluginTypeRandomConnectHost:      return eMyIconSettingsRandomConnectRelay;
    case ePluginTypeRelay:                  return eMyIconSettingsRelay;
    case ePluginTypeStoryboard:             return eMyIconSettingsShareStoryboard;
    case ePluginTypeTruthOrDare:            return eMyIconSettingsTruthOrDare;
    case ePluginTypeVideoPhone:             return eMyIconSettingsVideoPhone;
    case ePluginTypeVoicePhone:             return eMyIconSettingsVoicePhone;

    default:
        break;
    }

    return iconType;
}

//============================================================================
bool GuiHelpers::isPluginSingleSession( EPluginType ePluginType )
{
    bool isSingleSessionPlugin = false;
	switch(ePluginType)
	{
	case ePluginTypeVoicePhone:
	case ePluginTypeVideoPhone:
	case ePluginTypeTruthOrDare:
	case ePluginTypeMessenger:
	case ePluginTypeAdmin:
		isSingleSessionPlugin = true;
		break;
	case ePluginTypeWebServer:
	case ePluginTypeRelay:
	case ePluginTypeCamServer:
	case ePluginTypeStoryboard: 
	case ePluginTypeFileServer:
	case ePluginTypeFileXfer:
    case ePluginTypeChatRoomClient:
	default:
		break;
	}

	return isSingleSessionPlugin;
}

//============================================================================
//! which plugins to show in permission list
bool GuiHelpers::isPluginAPrimaryService( EPluginType ePluginType )
{
    bool isPrimaryPlugin = false;
    switch( ePluginType )
    {
    case ePluginTypeVoicePhone:
    case ePluginTypeVideoPhone:
    case ePluginTypeTruthOrDare:
    case ePluginTypeMessenger:
    case ePluginTypeAdmin:
    case ePluginTypeWebServer:
    case ePluginTypeCamServer:
    case ePluginTypeStoryboard:
    case ePluginTypeFileServer:
    case ePluginTypeFileXfer:
    case ePluginTypeChatRoomClient:
    case ePluginTypeChatRoomHost:
    case ePluginTypeNetworkHost:
    case ePluginTypeGroupHost:
    // connection test is special in that we want to be able to set it up seperately
    // but is required if user is network host or group host
    case ePluginTypeConnectTestHost:
        isPrimaryPlugin = true;
        break;
    case ePluginTypeRelay:
    case ePluginTypeNetworkSearchList:
    case ePluginTypeConnectTestClient:
    default:
        break;
    }

    return isPrimaryPlugin;
}

//============================================================================
bool GuiHelpers::getSecondaryPlugins( EPluginType ePluginType, QVector<EPluginType> secondaryPlugins )
{
    secondaryPlugins.clear();
    switch( ePluginType )
    {
    case ePluginTypeNetworkHost:
        secondaryPlugins.push_back( ePluginTypeNetworkSearchList );
        secondaryPlugins.push_back( ePluginTypeConnectTestHost );
        break;

    case ePluginTypeChatRoomHost:
        secondaryPlugins.push_back( ePluginTypeRelay );
        secondaryPlugins.push_back( ePluginTypeConnectTestHost );
        break;

    case ePluginTypeGroupHost:
        secondaryPlugins.push_back( ePluginTypeRelay );
        secondaryPlugins.push_back( ePluginTypeConnectTestHost );
        break;

    case ePluginTypeRandomConnectHost:
        secondaryPlugins.push_back( ePluginTypeRelay );
        secondaryPlugins.push_back( ePluginTypeConnectTestHost );
        break;

    default:
        break;
    }

    return !secondaryPlugins.isEmpty();
}

//============================================================================
std::string GuiParams::describePlugin( EPluginType ePluginType, bool rmtInitiated )
{
    std::string strPluginDesc = "";

	switch(ePluginType)
	{
    case ePluginTypeAdmin:
		strPluginDesc = QObject::tr( "Administration Service" ).toUtf8().constData();
		break;

    case ePluginTypeAboutMePage:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared About Me Page" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "About Me Page Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeAvatarImage:
        strPluginDesc = QObject::tr( "Avatar Image Service" ).toUtf8().constData();
        break;

    case ePluginTypeCamServer:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Web Cam" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Web Cam Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeChatRoomClient:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Chat Room User" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Chat Room Host" ).toUtf8().constData();
        }
        break;

    case ePluginTypeChatRoomHost:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Chat Room User" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Hosted Chat Room" ).toUtf8().constData();
        }
        break;

    case ePluginTypeFileServer:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Files" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Shared Files Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeFileXfer:
        strPluginDesc = QObject::tr( "Person To Person File Transfer" ).toUtf8().constData();
        break;

    case ePluginTypeConnectTestHost:
        strPluginDesc = QObject::tr( "Connection Test Service" ).toUtf8().constData();
        break;

    case ePluginTypeConnectTestClient:
        strPluginDesc = QObject::tr( "Connection Test Client" ).toUtf8().constData();
        break;

    case ePluginTypeGroupHost:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Hosted Group Service" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Group Host Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeNetworkSearchList:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Hosted Group/ChatRoom List Search Service" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Group/ChatRoom Host List Search Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeNetworkHost:
        strPluginDesc = QObject::tr( "Host A NoLimitConnect Network" ).toUtf8().constData();
        break;

    case ePluginTypeMessenger:
        strPluginDesc = QObject::tr( "Messanger Service" ).toUtf8().constData();
        break;


    case ePluginTypeRelay:
        strPluginDesc = QObject::tr( "Relay Service" ).toUtf8().constData();
        break;

    case ePluginTypeRandomConnectClient:
        strPluginDesc = QObject::tr( "Connect To Random Person Client" ).toUtf8().constData();
        break;

    case ePluginTypeRandomConnectHost:
        strPluginDesc = QObject::tr( "Connect To Random Person Service" ).toUtf8().constData();
        break;

    case ePluginTypeStoryboard:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Story Page (Blog)" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Shared Story Page Service (Blog)" ).toUtf8().constData();
        }
        break;

    case ePluginTypeTruthOrDare:
        strPluginDesc = QObject::tr( "Truth Or Dare Video Chat Game" ).toUtf8().constData();
        break;

    case ePluginTypeVideoPhone:
        strPluginDesc = QObject::tr( "Phone Call With Video Chat" ).toUtf8().constData();
        break;

    case ePluginTypeVoicePhone:
        strPluginDesc = QObject::tr( "Phone Call With Voice Only" ).toUtf8().constData();
        break;

    case ePluginTypeCameraService:
        strPluginDesc = QObject::tr( "Camera Feed Service" ).toUtf8().constData();
        break;

    case ePluginTypeMJPEGReader:
        strPluginDesc = QObject::tr( "MJPEG Movie Reader" ).toUtf8().constData();
        break;

    case ePluginTypeMJPEGWriter:
        strPluginDesc = QObject::tr( "MJPEG Movie Recorder" ).toUtf8().constData();
        break;

    case ePluginTypePersonalRecorder:
        strPluginDesc = QObject::tr( "Personal Notes And Media Recorder" ).toUtf8().constData();
        break;

    case ePluginTypeNetServices:
        strPluginDesc = QObject::tr( "Network Services" ).toUtf8().constData();
        break;

    case ePluginTypeSearch:
        strPluginDesc = QObject::tr( "Search Services" ).toUtf8().constData();
        break;

    case ePluginTypeSndReader:
        strPluginDesc = QObject::tr( "Recorded Audio Reader" ).toUtf8().constData();
        break;

    case ePluginTypeSndWriter:
        strPluginDesc = QObject::tr( "Audio Recorder" ).toUtf8().constData();
        break;

    case ePluginTypeWebServer:
        strPluginDesc = QObject::tr( "Server for About Me And Story Pages" ).toUtf8().constData();
        break;

    default:
        strPluginDesc = QObject::tr( "UNKNOWN PLUGIN" ).toUtf8().constData();
	}

	return strPluginDesc;
}

//============================================================================
QFrame * GuiHelpers::getParentPageFrame( QWidget * curWidget )
{
    QFrame * pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName;
    QString messengerPageObjName;

    launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( ( objName == launchPageObjName ) || ( objName == messengerPageObjName ) )
        {
            pageFrame = dynamic_cast<QFrame *>( curParent );
            if( pageFrame )
            {
                break;
            }
        }

        if( !curParent->parent() )
        {
            LogMsg( LOG_WARNING, "Object %s has no parent", objName.toUtf8().constData() );
        }

        curParent = dynamic_cast<QObject *>( curParent->parent() );
    }

    return pageFrame;
}

//============================================================================
QFrame * GuiHelpers::getMessengerPageFrame( QWidget * curWidget )
{
    QFrame * pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName;
    QString messengerPageObjName;

    launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( (objName == launchPageObjName) || (objName == messengerPageObjName) )
        {
            if( objName == messengerPageObjName )
            {
                pageFrame = dynamic_cast<QFrame *>(curParent);
                if (pageFrame)
                {
                    break;
                }
            }
            else
            {
                bool foundMessengerFrame = false;
                QWidget * baseFrame = dynamic_cast<QWidget *>(curParent->parent());
                if( baseFrame )
                {
                    QObjectList childList = baseFrame->children();
                    for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                    {
                        QFrame *childFrame = dynamic_cast<QFrame *>(*iter);
                        if( childFrame && childFrame->objectName() == messengerPageObjName )
                        {
                            pageFrame = childFrame;
                            foundMessengerFrame = true;
                            break;
                        }
                    }
                }

                if( foundMessengerFrame )
                {
                    break;
                }
            }
        }

        if( !curParent->parent() )
        {
            LogMsg( LOG_WARNING, "Object %s has no parent", objName.toUtf8().constData() );
        }

        curParent = dynamic_cast<QObject *>(curParent->parent());
    }

    return pageFrame;
}

//============================================================================
QFrame* GuiHelpers::getLaunchPageFrame( QWidget * curWidget )
{
    QFrame * pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName;
    QString messengerPageObjName;

    launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while (curParent)
    {
        QString objName = curParent->objectName();
        if ((objName == launchPageObjName) || (objName == messengerPageObjName))
        {
            if( objName == launchPageObjName )
            {
                pageFrame = dynamic_cast<QFrame *>(curParent);
                if (pageFrame)
                {
                    break;
                }
            }
            else
            {
                bool foundLaunchFrame = false;
                QWidget * baseFrame = dynamic_cast<QWidget *>(curParent->parent());
                if( baseFrame )
                {
                    QObjectList childList = baseFrame->children();
                    for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                    {
                        QFrame* childFrame = dynamic_cast<QFrame *>(*iter);
                        if( childFrame && childFrame->objectName() == launchPageObjName )
                        {
                            pageFrame = childFrame;
                            foundLaunchFrame = true;
                            break;
                        }
                    }
                }

                if( foundLaunchFrame )
                {
                    break;
                }
            }
        }

        if (!curParent->parent())
        {
            LogMsg( LOG_WARNING, "Object %s has no parent", objName.toUtf8().constData() );
        }

        curParent = dynamic_cast<QObject *>(curParent->parent());
    }

    return pageFrame;
}

//============================================================================
QFrame* GuiHelpers::getOppositePageFrame( QWidget * curWidget )
{
    QFrame * pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName;
    QString messengerPageObjName;

    launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while (curParent)
    {
        QString objName = curParent->objectName();
        if ((objName == launchPageObjName) || (objName == messengerPageObjName))
        {
            QString otherPageObjeName = (objName == launchPageObjName) ? messengerPageObjName : launchPageObjName;

            bool foundOtherFrame = false;
            QWidget * baseFrame = dynamic_cast<QWidget *>(curParent->parent());
            if( baseFrame )
            {
                QObjectList childList = baseFrame->children();
                for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                {
                    QFrame* childFrame = dynamic_cast<QFrame *>(*iter);
                    if( childFrame && childFrame->objectName() == otherPageObjeName )
                    {
                        pageFrame = childFrame;
                        foundOtherFrame = true;
                        break;
                    }
                }
            }

            if( foundOtherFrame )
            {
                break;
            }
        }

        if (!curParent->parent())
        {
            LogMsg( LOG_WARNING, "Object %s has no parent", objName.toUtf8().constData() );
        }

        curParent = dynamic_cast<QObject *>(curParent->parent());
    }

    return pageFrame;
}


//============================================================================
AppletBase * GuiHelpers::findParentApplet( QWidget * parent )
{
    AppletBase * appletBase = nullptr;
    if( parent )
    {
        appletBase = dynamic_cast< AppletBase * >( parent );
        while( parent && !appletBase )
        {
            parent = dynamic_cast< QWidget * >( parent->parent() );
            if( parent )
            {
                appletBase = dynamic_cast< AppletBase * >( parent );
            }
        }
    }

    return appletBase;
}

//============================================================================
bool GuiHelpers::validateUserName( QWidget * curWidget, QString strUserName )
{
    if( strUserName.contains( "NoLimitConnect" )
        || strUserName.contains( "nolimitconnect" )
        || strUserName.contains( "No Limit Connect" )
        || strUserName.contains( "no limit connect" )
        || strUserName.contains( "NoLimitConnectWeb" )
        || strUserName.contains( "GoTvPtoP" )
        || strUserName.contains( "GoTv PtoP Web" )
        || strUserName.contains( "gotv ptop web" ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have NoLimitConnect in name." ) );
        return false;
    }

    if( strUserName.contains( "'" ) || strUserName.contains( "\"" ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have special character quote." ) );
        return false;
    }

    if( strUserName.contains( "," ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have comma." ) );
        return false;
    }

    if( strUserName.contains( "(" ) || strUserName.contains( ")" ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have special character parentheses." ) );
        return false;
    }

    if( strUserName.contains( "/" ) || strUserName.contains( "\\" ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have special character slashes." ) );
        return false;
    }

    if( strUserName.length() > 23 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name is too long (maximum 23 chars)." ) );
        return false;
    }

    if( strUserName.length() < 4 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name is too short (minimum 4 chars)." ) );
        return false;
    }

    return true;
}

//============================================================================
bool GuiHelpers::validateMoodMessage( QWidget * curWidget, QString strMoodMsg )
{
    //if( strMoodMsg.contains( "'" ) )
    //{
    //    QMessageBox::warning( curWidget, QObject::tr( "Application" ), QObject::tr( "Mood Message cannot have special character quote." ) );
    //    return false;
    //}

    int iLen = strMoodMsg.length();
    if( iLen > 27 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Application" ), QObject::tr( "Mood Message is too long (maximum 27 chars)" ) );
        return false;
    }

    return true;
}

//============================================================================
bool GuiHelpers::validateAge( QWidget * curWidget, int age )
{
    if( age < 0 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Age Verify" ), QObject::tr( "Invalid Age" ) );
        return false;
    }

    if( age > 120 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Age Verify" ), QObject::tr( "Age Connot be greater than 120 years old" ) );
        return false;
    }

    return true;
}

//============================================================================
void GuiHelpers::fillGender( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxGenderType; i++ )
        {
            comboBox->addItem( GuiParams::describeGender( (EGenderType)i ) );
        }
    }
}

//============================================================================
EGenderType GuiHelpers::getGender( QComboBox * comboBox )
{
    return (EGenderType)comboBox->currentIndex();
}

//============================================================================
bool GuiHelpers::setGender( QComboBox * comboBox, EGenderType gender)
{
    if( comboBox )
    {
        comboBox->setCurrentIndex(genderToIndex(gender));
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
uint8_t GuiHelpers::genderToIndex( EGenderType gender )
{
    if( ( gender >= 0 ) && ( gender < eMaxGenderType ) )
    {
        return ( uint8_t )gender;
    }

    return 0;
}

//============================================================================
void GuiHelpers::fillAge( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxAgeType; i++ )
        {
            comboBox->addItem(  GuiParams::describeAge( (EAgeType)i ) );
        }
    }
}

//============================================================================
EAgeType GuiHelpers::getAge( QComboBox * comboBox )
{
    if( comboBox )
    {
        return (EAgeType)comboBox->currentIndex();
    }
    else
    {
        return eAgeTypeUnspecified;
    }
}

//============================================================================
bool GuiHelpers::setAge( QComboBox * comboBox, EAgeType ageType)
{
    if( comboBox )
    {
        comboBox->setCurrentIndex(ageToIndex(ageType));
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
QString GuiParams::describeAge( EAgeType gender )
{
    switch( gender )
    {
    case eAgeTypeUnspecified:
        return QObject::tr( "Any" );
    case eAgeTypeUnder21:
        return QObject::tr( "Under 21" );
    case eAgeType21OrOlder:
        return QObject::tr( "21 Or Older" );
    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
QString GuiParams::describeAge( int age )
{
    QString ageStr( age );
    if( ( age >= 80 ) || ( age < 0 ) )
    {
        ageStr = "Old";
    }
    else if( age == 0 )
    {
        ageStr = "Any";
    }
    else if( age < 21 )
    {
        ageStr = "Young";
    }
    else if( age >= 21 && age < 80 )
    {
        ageStr = "Middle Age";
    }

    return ageStr;
}

//============================================================================
uint8_t GuiHelpers::ageToIndex( EAgeType age )
{
    if( ( age >= 0 ) && ( age < eMaxAgeType ) )
    {
        return ( uint8_t )age;
    }

    return 0;
}

//============================================================================
void GuiHelpers::fillContentRating( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxContentRating; i++ )
        {
            comboBox->addItem(  GuiParams::describeContentRating( (EContentRating)i ) );
        }
    }
}

//============================================================================
EContentRating GuiHelpers::getContentRating( QComboBox * comboBox )
{
    if( comboBox )
    {
        return (EContentRating)comboBox->currentIndex();
    }
    else
    {
        return eContentRatingUnspecified;
    }
}

//============================================================================
bool GuiHelpers::setContentRating( QComboBox * comboBox, EContentRating contentRating)
{
    if( comboBox )
    {
        comboBox->setCurrentIndex(contentRatingToIndex(contentRating));
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
uint8_t GuiHelpers::contentRatingToIndex( EContentRating rating )
{
    if( ( rating >= 0 ) && ( rating < eMaxContentRating ) )
    {
        return ( uint8_t )rating;
    }

    return 0;
}

//============================================================================
void GuiHelpers::fillContentCatagory( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxContentCatagory; i++ )
        {
            comboBox->addItem(  GuiParams::describeContentCatagory( ( EContentCatagory )i ) );
        }
    }
}

//============================================================================
uint8_t GuiHelpers::contentCatagoryToIndex( EContentCatagory rating )
{
    if( ( rating >= 0 ) && ( rating < eMaxContentCatagory ) )
    {
        return ( uint8_t )rating;
    }

    return 0;
}

//============================================================================
void GuiHelpers::fillLanguage( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxLanguageType; i++ )
        {
            comboBox->addItem(  GuiParams::describeLanguage( (ELanguageType)i ) );
        }
    }
}

//============================================================================
ELanguageType GuiHelpers::getLanguage( QComboBox * comboBox )
{
    if( comboBox )
    {
        return (ELanguageType)comboBox->currentIndex();
    }
    else
    {
        return eLangUnspecified;
    }
}

//============================================================================
bool GuiHelpers::setLanguage( QComboBox * comboBox, ELanguageType language)
{
    if( comboBox )
    {
        comboBox->setCurrentIndex(languageToIndex(language));
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
uint16_t GuiHelpers::languageToIndex( ELanguageType language )
{
    if( ( language >= 0 ) && ( language < eMaxLanguageType ) )
    {
        return ( uint16_t )language;
    }

    return 0;
}

//============================================================================
void GuiHelpers::fillPermissionComboBox( QComboBox * permissionComboBox )
{
    if( permissionComboBox )
    {
        permissionComboBox->clear();
        permissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateAdmin ) );
        permissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateFriend ) );
        permissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateGuest ) );
        permissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateAnonymous ) );
        permissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateIgnore ) );
    }
}

//============================================================================
EFriendState GuiHelpers::comboIdxToFriendState( int comboIdx )
{
    switch( comboIdx )
    {
    case 0:
        return eFriendStateAdmin;
    case 1:
        return eFriendStateFriend;
    case 2:
        return eFriendStateGuest;
    case 3:
        return eFriendStateAnonymous;
    default:
        return eFriendStateIgnore;
    }   
}

//============================================================================
int GuiHelpers::friendStateToComboIdx( EFriendState friendState )
{
    switch( friendState )
    {
    case eFriendStateAdmin:
        return 0;
    case eFriendStateFriend:
        return 1;
    case eFriendStateGuest:
        return 2;
    case eFriendStateAnonymous:
        return 3;
    default:
        return 4;
    }
}

//============================================================================
void GuiHelpers::setValuesFromIdentity( QWidget * curWidget, VxNetIdent * ident, QComboBox *  age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
{
    if( curWidget && ident && age && genderCombo && languageCombo && contentCombo )
    {
        genderCombo->setCurrentIndex( ident->getAgeType() );
        genderCombo->setCurrentIndex( ident->getGender() );
        languageCombo->setCurrentIndex( ident->getPrimaryLanguage() );
        contentCombo->setCurrentIndex( ident->getPreferredContent() );
    }
}

//============================================================================
void GuiHelpers::setIdentityFromValues( QWidget * curWidget, VxNetIdent * ident, QComboBox * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
{
    if( curWidget && ident && age && genderCombo && languageCombo && contentCombo )
    {
        int ageValue = age->currentIndex();
        if( ( 0 > ageValue ) || ( eMaxAgeType <= ageValue ) )
        {
            ageValue = 0;
        }

        ident->setAgeType( (EAgeType)ageValue );

        int genderValue = genderCombo->currentIndex();
        if( ( 0 > genderValue ) || ( eMaxGenderType <= genderValue ) )
        {
            genderValue = 0;
        }

        ident->setGender( genderValue );

        int languageValue = languageCombo->currentIndex();
        if( ( 0 > languageValue ) || ( eMaxLanguageType <= genderValue ) )
        {
            languageValue = 0;
        }

        ident->setPrimaryLanguage( languageValue );

        int contentValue = contentCombo->currentIndex();
        if( ( 0 > contentValue ) || ( eMaxContentRating <= genderValue ) )
        {
            contentValue = 0;
        }

        ident->setPreferredContent( contentValue );
    }
}

//============================================================================
ActivityBase * GuiHelpers::findParentActivity( QWidget * widget )
{
    ActivityBase * actBase = nullptr;
    QObject * objWidget = widget;
    while( objWidget )
    {
        ActivityBase * actTemp = dynamic_cast<ActivityBase *>( objWidget );
        if( actTemp )
        {
            actBase = actTemp;
            break;
        }

        objWidget = objWidget->parent();
    }

    return actBase;
}

//============================================================================
QWidget * GuiHelpers::findAppletContentFrame( QWidget * widget )
{
    ActivityBase * actBase = findLaunchWindow( widget );
    if( actBase )
    {
        return actBase->getContentItemsFrame();
    }

    return nullptr;
}

//============================================================================
ActivityBase * GuiHelpers::findLaunchWindow( QWidget * widget )
{
    QObject * objWidget = findParentActivity( widget );
    QObject * prevWidget = objWidget;
    while( objWidget )
    {
        if( dynamic_cast<VxFrame *>( objWidget ) )
        {
            return dynamic_cast<ActivityBase *>( prevWidget );
        }

        prevWidget = objWidget;
        objWidget = objWidget->parent();
    }

    return nullptr;
}

//============================================================================
bool GuiHelpers::widgetToPluginSettings( EPluginType pluginType, PluginSettingsWidget* settingsWidget, PluginSetting& pluginSetting )
{
    bool result = false;
    if( ePluginTypeInvalid != pluginType && settingsWidget )
    {
        pluginSetting.setPluginType( pluginType );
        pluginSetting.setContentRating( (EContentRating)settingsWidget->getContentRatingComboBox()->currentIndex() );
        pluginSetting.setLanguage( (ELanguageType)settingsWidget->getLanguageComboBox()->currentIndex() );
        pluginSetting.setGender( (EGenderType)settingsWidget->getGenderComboBox()->currentIndex() );
        pluginSetting.setAgeType( (EAgeType)settingsWidget->getAgeComboBox()->currentIndex() );
        pluginSetting.setPluginUrl( settingsWidget->getServiceUrlEdit()->text().toUtf8().constData() );
        pluginSetting.setTitle( settingsWidget->getServiceNameEdit()->text().toUtf8().constData() );
        pluginSetting.setGreetingMsg( settingsWidget->getGreetingEdit()->toPlainText().toUtf8().constData() );
        pluginSetting.setRejectMsg( settingsWidget->getRejectEdit()->toPlainText().toUtf8().constData() );

        pluginSetting.setThumnailId( settingsWidget->getThumbnailChooseWidget()->updateAndGetThumbnailId(), settingsWidget->getThumbnailChooseWidget()->getThumbnailIsCircular() );

        QString description =settingsWidget->getServiceDescriptionEdit()->toPlainText().trimmed();
        if( !description.isEmpty() )
        {
            pluginSetting.setDescription( description.toUtf8().constData() );
        }
        else
        {
            pluginSetting.setDescription( "" );
        }

        result = true;
    }

    return result;
}

//============================================================================
bool GuiHelpers::pluginSettingsToWidget( EPluginType pluginType, PluginSetting& pluginSetting, PluginSettingsWidget* settingsWidget )
{
    bool result = false;
    if( ePluginTypeInvalid != pluginType && settingsWidget )
    {
        settingsWidget->getAgeComboBox()->setCurrentIndex( GuiHelpers::ageToIndex( pluginSetting.getAgeType() ) );
        settingsWidget->getGenderComboBox()->setCurrentIndex( GuiHelpers::genderToIndex( pluginSetting.getGender() ) );
        settingsWidget->getContentRatingComboBox()->setCurrentIndex( GuiHelpers::contentRatingToIndex( pluginSetting.getContentRating() ) );
        settingsWidget->getLanguageComboBox()->setCurrentIndex( GuiHelpers::languageToIndex( pluginSetting.getLanguage() ) );

        settingsWidget->getServiceUrlEdit()->setText( pluginSetting.getPluginUrl().c_str() );
        settingsWidget->getServiceNameEdit()->setText( pluginSetting.getTitle().c_str() );
        settingsWidget->getServiceDescriptionEdit()->appendPlainText( pluginSetting.getDescription().c_str() );
        settingsWidget->getGreetingEdit()->appendPlainText( pluginSetting.getGreetingMsg().c_str() );
        settingsWidget->getRejectEdit()->appendPlainText( pluginSetting.getRejectMsg().c_str() );

        settingsWidget->getThumbnailChooseWidget()->loadThumbnail( pluginSetting.getThumnailId(), pluginSetting.getThumbnailIsCircular() );
 
        result = true;
    }

    return result;
}