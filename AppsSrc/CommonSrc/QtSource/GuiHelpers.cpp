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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiHelpers.h"
#include "GuiParams.h"
#include "GuiUser.h"

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
#include <CoreLib/VxDebug.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <VxVideoLib/VxVideoLib.h>

#include <QDesktopServices>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QUrl>


#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
# if defined (Q_OS_ANDROID)
#  include <QtAndroid>
# endif
#else
# if defined (Q_OS_ANDROID)
#  include <QtCore/private/qandroidextras_p.h>
# endif
#endif


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
QString GuiHelpers::browseForDirectory( QString startDir, QWidget* parent )
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
EApplet GuiHelpers::getAppletThatPlaysFile( AppCommon& myApp, AssetBaseInfo& assetInfo )
{
    return getAppletThatPlaysFile( myApp, GuiParams::assetTypeToFileType( assetInfo.getAssetType() ), assetInfo.getAssetName().c_str(), assetInfo.getAssetUniqueId() );
}

//============================================================================
EApplet GuiHelpers::getAppletThatPlaysFile( AppCommon& myApp, uint8_t fileType, QString fullFileName, VxGUID& assetId )
{
    EApplet applet = eAppletUnknown;
    if( fileType & VXFILE_TYPE_VIDEO )
    {
        if( myApp.getFromGuiInterface().fromGuiIsMyP2PWebVideoFile( fullFileName.toUtf8().constData() ) )
        {
            applet = eAppletPlayerCamClip;
        }
        else
        {
            // BRJ video player not ready yet.. launch external app instead
            // applet = eAppletPlayerVideo;
        }
    }
    else if( fileType & VXFILE_TYPE_PHOTO )
    {
        applet = eAppletPlayerPhoto;
    }

    // TODO: Vlc style audio player  

    return applet;
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
             || ( eAppletServiceShareFiles == applet )
             || ( eAppletServiceShareWebCam == applet )
             || ( eAppletServiceAboutMe == applet )
             || ( eAppletServiceStoryboard == applet )
             );
}

//============================================================================
bool GuiHelpers::isAppletAClient( EApplet applet )
{
    return ( ( eAppletAboutMeClient == applet )
             || ( eAppletClientAvatarImage == applet )
             || ( eAppletClientConnectionTest == applet )
             || ( eAppletClientHostGroup == applet )
             || ( eAppletClientHostGroupListing == applet )
             || ( eAppletClientRandomConnect == applet )
             || ( eAppletClientRandomConnectRelay == applet )
             || ( eAppletClientShareFiles == applet )
             || ( eAppletCamClient == applet )
             || ( eAppletStoryboardClient == applet )
             );
}

//============================================================================
EPluginType GuiHelpers::getAppletAssociatedPlugin( EApplet applet )
{
    EPluginType pluginType = ePluginTypeInvalid;
    switch( applet )
    {
    case eAppletAboutMeClient:              return ePluginTypeAboutMePageClient;
    case eAppletCamClient:                  return ePluginTypeCamServer;
    case eAppletClientAvatarImage:          return ePluginTypeClientPeerUser;
    case eAppletClientConnectionTest:       return ePluginTypeClientConnectTest;
    case eAppletClientHostGroup:            return ePluginTypeHostGroup;
    case eAppletClientHostGroupListing:     return ePluginTypeNetworkSearchList;
    case eAppletClientHostNetwork:          return ePluginTypeHostNetwork;
    case eAppletClientRandomConnect:        return ePluginTypeClientRandomConnect;
    case eAppletClientRandomConnectRelay:   return ePluginTypeHostRandomConnect;
    case eAppletClientShareFiles:           return ePluginTypeFileShareServer;

    case eAppletFileShareClientView:        return ePluginTypeFileShareClient;

    case eAppletServiceAboutMe:              return ePluginTypeAboutMePageServer;
    case eAppletServiceAvatarImage:          return ePluginTypeHostPeerUser;
    case eAppletServiceConnectionTest:       return ePluginTypeHostConnectTest;
    case eAppletHostChatRoomAdmin:           return ePluginTypeHostChatRoom;
    case eAppletGroupHostAdmin:              return ePluginTypeHostGroup;
    case eAppletServiceHostNetwork:          return ePluginTypeHostNetwork;
    case eAppletServiceShareFiles:           return ePluginTypeFileShareServer;
    case eAppletServiceShareWebCam:          return ePluginTypeCamServer;
    case eAppletServiceStoryboard:           return ePluginTypeStoryboardServer;

    case eAppletSettingsAboutMe:            return ePluginTypeAboutMePageServer;
    case eAppletSettingsAvatarImage:        return ePluginTypeHostPeerUser;
    case eAppletSettingsWebCamServer:       return ePluginTypeCamServer;
    case eAppletSettingsConnectTest:        return ePluginTypeHostConnectTest;
    case eAppletSettingsShareFiles:         return ePluginTypeFileShareServer;
    case eAppletSettingsFileXfer:           return ePluginTypePersonFileXfer;

    case eAppletSettingsHostChatRoom:       return ePluginTypeHostChatRoom;
    case eAppletSettingsHostGroup:          return ePluginTypeHostGroup;
    case eAppletSettingsHostNetwork:        return ePluginTypeHostNetwork;
    case eAppletSettingsHostRandomConnect:  return ePluginTypeHostRandomConnect;

    case eAppletSettingsMessenger:          return ePluginTypeMessenger;
    case eAppletSettingsRandomConnect:      return ePluginTypeClientRandomConnect;

    case eAppletSettingsStoryboard:         return ePluginTypeStoryboardServer;
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
    case ePluginTypeAboutMePageServer:      return eAppletEditAboutMe;
    case ePluginTypeHostPeerUser:           return eAppletEditAvatarImage;
    case ePluginTypeStoryboardServer:       return eAppletEditStoryboard;
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeHostConnectTest:        return eAppletSettingsConnectTest;
    case ePluginTypeFileShareServer:        return eAppletUnknown;
    case ePluginTypePersonFileXfer:         return eAppletUnknown;
    case ePluginTypeHostGroup:              return eAppletUnknown;
    case ePluginTypeNetworkSearchList:      return eAppletUnknown;
    case ePluginTypeHostNetwork:            return eAppletUnknown;
    case ePluginTypeClientRandomConnect:    return eAppletUnknown;
    case ePluginTypeHostRandomConnect:      return eAppletUnknown;
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
    case ePluginTypeAboutMePageServer:      return eAppletSettingsAboutMe;
    case ePluginTypeHostPeerUser:           return eAppletSettingsAvatarImage;
    case ePluginTypeCamServer:              return eAppletSettingsWebCamServer;
    case ePluginTypeHostConnectTest:        return eAppletSettingsConnectTest;
    case ePluginTypeFileShareServer:        return eAppletSettingsShareFiles;
    case ePluginTypePersonFileXfer:         return eAppletSettingsFileXfer;

    case ePluginTypeHostChatRoom:           return eAppletSettingsHostChatRoom;
    case ePluginTypeHostGroup:              return eAppletSettingsHostGroup;
    case ePluginTypeHostNetwork:            return eAppletSettingsHostNetwork;
    case ePluginTypeHostRandomConnect:      return eAppletSettingsHostRandomConnect;

    case ePluginTypeMessenger:              return eAppletSettingsMessenger;
    case ePluginTypePushToTalk:             return eAppletSettingsPushToTalk;
    case ePluginTypeClientRandomConnect:    return eAppletSettingsRandomConnect;

    case ePluginTypeStoryboardServer:       return eAppletSettingsStoryboard;
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
    case ePluginTypeAboutMePageServer:      return eAppletEditAboutMe;
    case ePluginTypeHostPeerUser:           return eAppletEditAvatarImage;
    case ePluginTypeStoryboardServer:       return eAppletEditStoryboard;
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeHostConnectTest:        return eAppletSettingsConnectTest;
    case ePluginTypeFileShareServer:             return eAppletUnknown;
    case ePluginTypePersonFileXfer:               return eAppletUnknown;
    case ePluginTypeHostGroup:              return eAppletUnknown;
    case ePluginTypeNetworkSearchList:      return eAppletUnknown;
    case ePluginTypeHostNetwork:            return eAppletUnknown;
    case ePluginTypeClientRandomConnect:    return eAppletUnknown;
    case ePluginTypeHostRandomConnect:      return eAppletUnknown;

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
    case ePluginTypeAboutMePageServer:     return eAppletEditAboutMe;
    case ePluginTypeAboutMePageClient:     return eAppletAboutMeClient;
    case ePluginTypeHostPeerUser:          return eAppletEditAvatarImage;
    //case ePluginTypeCamServer:              return eAppletSettingsWebCamServer;
    // case ePluginTypeHostConnectTest:     return eAppletSettingsConnectTest;
    //case ePluginTypeFileShareServer:             return eAppletShareFiles;
    // case ePluginTypePersonFileXfer:               return eAppletSettingsFileXfer;

    case ePluginTypeHostChatRoom:           return eAppletSettingsHostChatRoom;
    case ePluginTypeHostGroup:              return eAppletSettingsHostGroup;
    case ePluginTypeHostNetwork:            return eAppletSettingsHostNetwork;
    case ePluginTypeHostRandomConnect:      return eAppletSettingsHostRandomConnect;

    //case ePluginTypeMessenger:              return eAppletSettingsMessenger;
    //case ePluginTypeClientRandomConnect:          return eAppletServiceRandomConnect;

    case ePluginTypeStoryboardServer:       return eAppletEditStoryboard;
    case ePluginTypeStoryboardClient:       return eAppletStoryboardClient;
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
    case ePluginTypeAboutMePageServer:      return eMyIconSettingsAboutMe;
    case ePluginTypeHostPeerUser:           return eMyIconSettingsAvatarImage;
    case ePluginTypeCamServer:              return eMyIconSettingsShareWebCam;
    case ePluginTypeHostConnectTest:        return eMyIconSettingsConnectionTest;
    case ePluginTypeFileShareServer:             return eMyIconSettingsShareFiles;
    case ePluginTypePersonFileXfer:               return eMyIconSettingsFileXfer;
    case ePluginTypeHostGroup:              return eMyIconSettingsHostGroup;
    case ePluginTypeNetworkSearchList:      return eMyIconSettingsHostGroupListing;
    case ePluginTypeHostNetwork:            return eMyIconSettingsHostNetwork;
    case ePluginTypeMessenger:              return eMyIconSettingsMessenger;
    case ePluginTypeClientRandomConnect:    return eMyIconSettingsRandomConnect;
    case ePluginTypeHostRandomConnect:      return eMyIconSettingsRandomConnectRelay;
    case ePluginTypeStoryboardServer:       return eMyIconSettingsShareStoryboard;
    case ePluginTypeTruthOrDare:            return eMyIconSettingsTruthOrDare;
    case ePluginTypeVideoPhone:             return eMyIconSettingsVideoPhone;
    case ePluginTypeVoicePhone:             return eMyIconSettingsVoicePhone;
    case ePluginTypePushToTalk:             return eMyIconSettingsPushToTalk;

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
    case ePluginTypePushToTalk:
	case ePluginTypeVideoPhone:
	case ePluginTypeTruthOrDare:
	case ePluginTypeMessenger:
	case ePluginTypeAdmin:
    case ePluginTypeAboutMePageClient:
    case ePluginTypeStoryboardClient:
    case ePluginTypeFileShareClient:
    case ePluginTypeCamClient:   
		isSingleSessionPlugin = true;
		break;
	case ePluginTypeAboutMePageServer:
	case ePluginTypeCamServer:
	case ePluginTypeStoryboardServer: 
	case ePluginTypeFileShareServer:
	case ePluginTypePersonFileXfer:
    case ePluginTypeClientChatRoom:
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
    case ePluginTypeAdmin:
    case ePluginTypeAboutMePageServer:
    case ePluginTypeVoicePhone:
    case ePluginTypeVideoPhone:
    case ePluginTypeTruthOrDare:
    case ePluginTypeMessenger:
    case ePluginTypeCamServer:
    case ePluginTypeStoryboardServer:
    case ePluginTypeFileShareServer:
    case ePluginTypePersonFileXfer:
    case ePluginTypeHostGroup:
    case ePluginTypeHostChatRoom:
    case ePluginTypeHostNetwork:
    case ePluginTypeHostRandomConnect:
    case ePluginTypeHostConnectTest:
    case ePluginTypePushToTalk:
        isPrimaryPlugin = true;
        break;

    case ePluginTypeNetworkSearchList:
    case ePluginTypeClientConnectTest:
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
    case ePluginTypeHostNetwork:
        secondaryPlugins.push_back( ePluginTypeNetworkSearchList );
        secondaryPlugins.push_back( ePluginTypeHostConnectTest );
        break;

    case ePluginTypeHostChatRoom:
        secondaryPlugins.push_back( ePluginTypeHostConnectTest );
        break;

    case ePluginTypeHostGroup:
        secondaryPlugins.push_back( ePluginTypeHostConnectTest );
        break;

    case ePluginTypeHostRandomConnect:
        secondaryPlugins.push_back( ePluginTypeHostConnectTest );
        break;

    default:
        break;
    }

    return !secondaryPlugins.isEmpty();
}

//============================================================================
bool GuiHelpers::isMessagerFrame( QWidget* curWidget )
{
    bool isMessengerFrame{ false };
    
    QObject * curParent = curWidget;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( objName == messengerPageObjName )
        {
            isMessengerFrame = true;
            break;
        }


        curParent = dynamic_cast<QObject *>( curParent->parent() );
        if( !curParent )
        {
            break;
        }
    }

    return isMessengerFrame;

}

//============================================================================
QFrame* GuiHelpers::getParentPageFrame( QWidget* curWidget )
{
    QFrame* pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( ( objName == launchPageObjName ) || ( objName == messengerPageObjName ) )
        {
            pageFrame = dynamic_cast<QFrame*>( curParent );
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
QFrame* GuiHelpers::getMessengerPageFrame( QWidget* curWidget )
{
    QFrame* pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( (objName == launchPageObjName) || (objName == messengerPageObjName) )
        {
            if( objName == messengerPageObjName )
            {
                pageFrame = dynamic_cast<QFrame*>(curParent);
                if (pageFrame)
                {
                    break;
                }
            }
            else
            {
                bool foundMessengerFrame = false;
                QWidget* baseFrame = dynamic_cast<QWidget*>(curParent->parent());
                if( baseFrame )
                {
                    QObjectList childList = baseFrame->children();
                    for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                    {
                        QFrame*childFrame = dynamic_cast<QFrame*>(*iter);
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
QFrame* GuiHelpers::getLaunchPageFrame( QWidget* curWidget )
{
    QFrame* pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while (curParent)
    {
        QString objName = curParent->objectName();
        if ((objName == launchPageObjName) || (objName == messengerPageObjName))
        {
            if( objName == launchPageObjName )
            {
                pageFrame = dynamic_cast<QFrame*>(curParent);
                if (pageFrame)
                {
                    break;
                }
            }
            else
            {
                bool foundLaunchFrame = false;
                QWidget* baseFrame = dynamic_cast<QWidget*>(curParent->parent());
                if( baseFrame )
                {
                    QObjectList childList = baseFrame->children();
                    for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                    {
                        QFrame* childFrame = dynamic_cast<QFrame*>(*iter);
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
QFrame* GuiHelpers::getOppositePageFrame( QWidget* curWidget )
{
    QFrame* pageFrame = nullptr;
    QObject * curParent = curWidget;

    QString launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while (curParent)
    {
        QString objName = curParent->objectName();
        if ((objName == launchPageObjName) || (objName == messengerPageObjName))
        {
            QString otherPageObjeName = (objName == launchPageObjName) ? messengerPageObjName : launchPageObjName;

            bool foundOtherFrame = false;
            QWidget* baseFrame = dynamic_cast<QWidget*>(curParent->parent());
            if( baseFrame )
            {
                QObjectList childList = baseFrame->children();
                for( auto iter = childList.begin();  iter != childList.end(); ++iter )
                {
                    QFrame* childFrame = dynamic_cast<QFrame*>(*iter);
                    if( childFrame && childFrame->objectName() == otherPageObjeName && childFrame == GetAppInstance().getHomePage().getMessengerParentFrame() )
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
AppletBase * GuiHelpers::findParentApplet( QWidget* parent )
{
    AppletBase * appletBase = nullptr;
    if( parent )
    {
        appletBase = dynamic_cast< AppletBase * >( parent );
        while( parent && !appletBase )
        {
            parent = dynamic_cast< QWidget* >( parent->parent() );
            if( parent )
            {
                appletBase = dynamic_cast< AppletBase * >( parent );
            }
        }
    }

    return appletBase;
}

//============================================================================
bool GuiHelpers::validateUserName( QWidget* curWidget, QString strUserName )
{
    if( strUserName.contains( "NoLimitConnect" )
        || strUserName.contains( "nolimitconnect" )
        || strUserName.contains( "No Limit Connect" )
        || strUserName.contains( "no limit connect" )
        || strUserName.contains( "NoLimitConnectWeb" )
        || strUserName.contains( "NlcPtoP" )
        || strUserName.contains( "Nlc PtoP Web" )
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

    if( strUserName.length() > 27 )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name is too long (maximum 27 chars)." ) );
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
bool GuiHelpers::validateMoodMessage( QWidget* curWidget, QString strMoodMsg )
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
bool GuiHelpers::validateAge( QWidget* curWidget, int age )
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
void GuiHelpers::fillJoinRequest( QComboBox* comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        comboBox->addItem( GuiParams::describeJoinState( eJoinStateJoinRequested ) );
        comboBox->addItem( GuiParams::describeJoinState( eJoinStateJoinIsGranted ) );
        comboBox->addItem( GuiParams::describeJoinState( eJoinStateJoinDenied ) );
    }
}

//============================================================================
EJoinState GuiHelpers::comboIdxToJoinState( int comboIdx )
{
    switch( comboIdx )
    {
    case 0: return eJoinStateJoinRequested;
    case 1: return eJoinStateJoinIsGranted;
    default: return  eJoinStateJoinDenied;
    }
}

//============================================================================
uint8_t GuiHelpers::joinRequestToIndex( EJoinState joinState )
{
    switch( joinState )
    {
    case eJoinStateJoinRequested:
        return 0;
    case eJoinStateJoinIsGranted:
        return 1;
    default:
        return 2;
    }
}

//============================================================================
void GuiHelpers::setValuesFromIdentity( QWidget* curWidget, VxNetIdent * ident, QComboBox *  ageCombo, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
{
    if( curWidget && ident && ageCombo && genderCombo && languageCombo && contentCombo )
    {
        ageCombo->setCurrentIndex( ident->getAgeType() );
        genderCombo->setCurrentIndex( ident->getGender() );
        languageCombo->setCurrentIndex( ident->getPrimaryLanguage() );
        contentCombo->setCurrentIndex( ident->getPreferredContent() );
    }
}

//============================================================================
void GuiHelpers::setIdentityFromValues( QWidget* curWidget, VxNetIdent * ident, QComboBox * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
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
ActivityBase * GuiHelpers::findParentActivity( QWidget* widget )
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
QWidget* GuiHelpers::findAppletContentFrame( QWidget* widget )
{
    ActivityBase * actBase = findLaunchWindow( widget );
    if( actBase )
    {
        return actBase->getContentItemsFrame();
    }

    return nullptr;
}

//============================================================================
QWidget* GuiHelpers::findParentPage( QWidget* parent ) // this should return home or messenger page
{
    // from title bar find the Home page or messenger
    QFrame* pageFrame = nullptr;
    QObject* curParent = parent;

    QString launchPageObjName = OBJNAME_FRAME_LAUNCH_PAGE;
    QString messengerPageObjName = OBJNAME_FRAME_MESSAGER_PAGE;

    while( curParent )
    {
        QString objName = curParent->objectName();
        if( (objName == launchPageObjName) || (objName == messengerPageObjName) )
        {
            pageFrame = dynamic_cast<QFrame*>(curParent);
            if( pageFrame )
            {
                break;
            }
        }

        if( !curParent->parent() )
        {
            LogMsg( LOG_WARNING, "Object %s has no parent", objName.toUtf8().constData() );
            break;
        }
        else
        {
            curParent = curParent->parent();
        }
    }

    return pageFrame;
}

//============================================================================
QWidget* GuiHelpers::findParentContentFrame( QWidget* parent )
{
    ActivityBase* actBase = findParentActivity( parent );
    if( actBase )
    {
        return actBase->getContentItemsFrame();
    }
    else
    {
        return findParentPage( parent );
    }

    return nullptr;
}

//============================================================================
ActivityBase * GuiHelpers::findLaunchWindow( QWidget* widget )
{
    QObject * objWidget = findParentActivity( widget );
    QObject * prevWidget = objWidget;
    while( objWidget )
    {
        if( dynamic_cast<VxFrame*>( objWidget ) )
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
        pluginSetting.setTitle( settingsWidget->getServiceTitleEdit()->text().toUtf8().constData() );
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
        settingsWidget->getServiceTitleEdit()->setText( pluginSetting.getTitle().c_str() );
        settingsWidget->getServiceDescriptionEdit()->appendPlainText( pluginSetting.getDescription().c_str() );
        settingsWidget->getGreetingEdit()->appendPlainText( pluginSetting.getGreetingMsg().c_str() );
        settingsWidget->getRejectEdit()->appendPlainText( pluginSetting.getRejectMsg().c_str() );

        settingsWidget->getThumbnailChooseWidget()->loadThumbnail( pluginSetting.getThumnailId(), pluginSetting.getThumbnailIsCircular() );
 
        result = true;
    }

    return result;
}

//============================================================================
bool GuiHelpers::createThumbFileName( VxGUID& assetGuid, QString& retFileName )
{
    bool validFileName = false;
    if( assetGuid.isVxGUIDValid() )
    {
        retFileName = VxGetAppDirectory( eAppDirThumbs ).c_str();
        if( !retFileName.isEmpty() )
        {
            retFileName += assetGuid.toHexString().c_str();
            retFileName += ".nlt"; // use extension not known as image so thumbs will not be scanned by android image gallery etc
            validFileName = true;
        }
    }

    return validFileName;
}

//============================================================================
bool GuiHelpers::makeCircleImage( QImage& image )
{
    QPixmap target( image.width(), image.height() );
    target.fill( Qt::transparent );

    QPainter painter( &target );

    // Set clipped region (circle) in the center of the target image
    QRegion clipRegion( QRect( 0, 0, image.width(), image.height() ), QRegion::Ellipse );
    painter.setClipRegion( clipRegion );

    painter.drawImage( 0, 0, image );
    image = target.toImage();
    return !image.isNull();
}

//============================================================================
bool GuiHelpers::makeCircleImage( QPixmap& targetPixmap )
{
    QPixmap target( targetPixmap.width(), targetPixmap.height() );
    target.fill( Qt::transparent );

    QPainter painter( &target );

    // Set clipped region (circle) in the center of the target image
    QRegion clipRegion( QRect( 0, 0, targetPixmap.width(), targetPixmap.height() ), QRegion::Ellipse );
    painter.setClipRegion( clipRegion );

    painter.drawPixmap( 0, 0, targetPixmap );
    targetPixmap = target;
    return !targetPixmap.isNull();
}

//============================================================================
uint64_t GuiHelpers::saveToPngFile( QImage& image, QString& fileName ) // returns file length
{
    bool result = !image.isNull() && !fileName.isEmpty();
    if( result )
    {
        result = image.save( fileName, "PNG" );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHelpers::saveToPngFile Invalid Param" );
        return 0;
    }

    if( result )
    {
        return VxFileUtil::fileExists( fileName.toUtf8().constData() );
    }

    return 0;
}

//============================================================================
uint64_t GuiHelpers::saveToPngFile( QPixmap& bitmap, QString& fileName ) // returns file length
{
    bool result = !bitmap.isNull() && !fileName.isEmpty();
    if( result )
    {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if( !bitmap.isNull() )
        {
            result = bitmap.save( fileName, "PNG" );
#else
        const QPixmap* bitmap = m_ThumbPixmap;
        if( bitmap )
        {
            result = bitmap->save( fileName, "PNG" );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHelpers::saveToPngFile Invalid Param" );
        return 0;
    }

    if( result )
    {
        return VxFileUtil::fileExists( fileName.toUtf8().constData() );
    }  

    return 0;
}

//============================================================================
bool GuiHelpers::checkUserPermission( QString permissionName ) // returns false if user denies permission to use android hardware
{

#if defined (Q_OS_ANDROID)

    if( QtAndroidPrivate::Authorized != QtAndroidPrivate::checkPermission(permissionName).result() )
    {
        if( QtAndroidPrivate::Authorized != QtAndroidPrivate::requestPermission(permissionName).result() )
        {
            return false;
        }
    }


    return true;
#else
    return true;
#endif // defined (Q_OS_ANDROID)
}

//============================================================================
void GuiHelpers::fillHostType( QComboBox* comboBox, bool excludePeerHost )
{
    comboBox->addItem( GuiParams::describeHostType( eHostTypeGroup ) );
    comboBox->addItem( GuiParams::describeHostType( eHostTypeChatRoom ) );
    comboBox->addItem( GuiParams::describeHostType( eHostTypeRandomConnect ) );
    comboBox->addItem( GuiParams::describeHostType( eHostTypeNetwork ) );
    comboBox->addItem( GuiParams::describeHostType( eHostTypeConnectTest ) );

    if( !excludePeerHost )
    { 
        comboBox->addItem( GuiParams::describeHostType( eHostTypePeerUserDirect ) );
    }
}

//============================================================================
EHostType GuiHelpers::comboIdxToHostType( int comboIdx )
{
    switch( comboIdx )
    {
    case 0:
        return eHostTypeGroup;
    case 1:
        return eHostTypeChatRoom;
    case 2:
        return eHostTypeRandomConnect;
    case 3:
        return eHostTypeNetwork;
    case 4:
        return eHostTypeConnectTest;
    case 5:
        return eHostTypePeerUserDirect;

    default:
        return eHostTypeUnknown;
    }
}

//============================================================================
void GuiHelpers::userUnavailableMessageBox( QWidget* parent, GuiUser* user )
{
    QMessageBox::information( parent, QObject::tr( "User Unavailable" ), QObject::tr( "User " ) + user->getOnlineName().c_str() + QObject::tr( " Is Unavailable" ), QMessageBox::Ok );
}