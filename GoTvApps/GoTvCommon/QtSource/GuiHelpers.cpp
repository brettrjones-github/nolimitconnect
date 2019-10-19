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
// http://www.gotvptop.com
//============================================================================

#include "GuiHelpers.h"

#include "AppCommon.h"
#include "MyIconsDefs.h"

#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/ObjectCommon.h>

#include <QDesktopServices>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QLineEdit>
#include <QUrl>

//============================================================================
const char * GuiHelpers::describeResponse( EOfferResponse eOfferResponse )
{
	switch( eOfferResponse )
	{
	case eOfferResponseAccept:
		return "Offer Accepted";

	case eOfferResponseReject:
		return "Offer Rejected";

	case eOfferResponseBusy:
		return "Busy";

	case eOfferResponseCancelSession:
		return "Session Canceled";

	case eOfferResponseEndSession:
		return "User Ended Session";

	case eOfferResponseUserOffline:
		return "User Is Offline";

	default:
		return "Session Ended";
	}
}

//============================================================================
const char * GuiHelpers::describePluginPermission( EFriendState ePluginPermission )
{
	switch(ePluginPermission)
	{
	case eFriendStateIgnore:
		return "Disabled";
	case eFriendStateAnonymous:
		return "Anonymous";
	case eFriendStateGuest:
		return "Guest";
	case eFriendStateFriend:
		return "Friends";
	default:
		return "Unknown";
	}
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
QString GuiHelpers::describeEXferError( EXferError xferError )
{
	switch( xferError )
	{
	case eXferErrorNone:
		return QObject::tr("No Error");
	case eXferErrorDisconnected:
		return QObject::tr("Disconnected");
	case eXferErrorPermission:
		return QObject::tr("Permission Error");
	case eXferErrorFileNotFound:
		return QObject::tr("File Not Found");
	case eXferErrorCanceled:
		return QObject::tr("Canceled");
	case eXferErrorBadParam:
		return QObject::tr("Bad Param");
	case eXferErrorAtSrc:
		return QObject::tr("Error At Source");
	case eXferErrorBusy:
		return QObject::tr("Busy");
	case eXferErrorAlreadyDownloading:
		return QObject::tr("Already Downloading");
	case eXferErrorAlreadyDownloaded:
		return QObject::tr("Already Downloaded");
	case eXferErrorAlreadyUploading:
		return QObject::tr("Already Uploading");
	case eXferErrorFileCreateError:
		return QObject::tr("File Create Error");
	case eXferErrorFileOpenAppendError:
		return QObject::tr("File Append Error");
	case eXferErrorFileOpenError:
		return QObject::tr("File Open Error");
	case eXferErrorFileSeekError:
		return QObject::tr("Seek Error");
	case eXferErrorFileReadError:
		return QObject::tr("Read Error");
	case eXferErrorFileWriteError:
		return QObject::tr("Write Error");
	case eXferErrorFileMoveError:
		return QObject::tr("Move File Error");
	default:
		return QObject::tr("Unknown Error");
	}
}

//============================================================================
QString GuiHelpers::describeEXferState( EXferState xferState )
{
	switch( xferState )
	{
	case eXferStateUploadNotStarted:
		return QObject::tr("Upload Not Started");
	case eXferStateWaitingOfferResponse:
		return QObject::tr("Waiting Response");
	case eXferStateInUploadQue:
		return QObject::tr("In Upload Que");
	case eXferStateBeginUpload:
		return QObject::tr("Start Upload");
	case eXferStateInUploadXfer:
		return QObject::tr("Uploading");
	case eXferStateCompletedUpload:
		return QObject::tr("Completed Upload");
	case eXferStateUserCanceledUpload:
		return QObject::tr("Upload Canceled");
	case eXferStateUploadOfferRejected:
		return QObject::tr("Offer Rejected");
	case eXferStateUploadError:
		return QObject::tr("ERROR");
	case eXferStateDownloadNotStarted:
		return QObject::tr("Download Not Started");
	case eXferStateInDownloadQue:
		return QObject::tr("In Download Que");
	case eXferStateBeginDownload:
		return QObject::tr("Start Download");
	case eXferStateInDownloadXfer:
		return QObject::tr("Downloading");
	case eXferStateCompletedDownload:
		return QObject::tr("Completed Download");
	case eXferStateUserCanceledDownload:
		return QObject::tr("Download Canceled");
	case eXferStateDownloadError:
		return QObject::tr("ERROR");
	case eXferStateUnknown:
	default:
		return QObject::tr("Unknown State");
	}
}

//============================================================================
QString GuiHelpers::describeFileLength( uint64_t fileLen )
{
	std::string strLen;
	if( fileLen >= 1000000000000ULL )
	{
		StdStringFormat( strLen, "%3.1fTB ", (double)(fileLen) / 1000000000000.0);
	}
	else if( fileLen >= 1000000000ULL )
	{
		StdStringFormat( strLen, "%3.1fGB ", (double)(fileLen) / 1000000000.0);
	}
	else if( fileLen >= 1000000 )
	{
		StdStringFormat( strLen, "%3.1fMB ", (double)(fileLen) / 1000000.0);
	}
	else if( fileLen >= 1000 )
	{
		StdStringFormat( strLen, "%3.1fKB ", (double)(fileLen) / 1000.0);
	}
	else
	{
		StdStringFormat( strLen, "%3.1fBytes ", (double)fileLen );
	}

	QString strFormatedLen = strLen.c_str();
	return strFormatedLen;
}

//============================================================================
bool GuiHelpers::isAppletAService( EApplet applet )
{
    return ( ( eAppletServiceAboutMe == applet )
             || ( eAppletServiceAvatarImage == applet )
             || ( eAppletServiceConnectionTest == applet )
             || ( eAppletServiceHostGroup == applet )
             || ( eAppletServiceHostGroupListing == applet )
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
             || ( eAppletClientGroupHost == applet )
             || ( eAppletClientGroupListingHost == applet )
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
    case eAppletClientConnectionTest:       return ePluginTypeConnectTest;
    case eAppletClientGroupHost:            return ePluginTypeHostGroup;
    case eAppletClientGroupListingHost:     return ePluginTypeHostGroupListing;
    case eAppletClientNetworkHost:          return ePluginTypeHostNetwork;
    case eAppletClientRandomConnect:         return ePluginTypeRandomConnect;
    case eAppletClientRandomConnectRelay:    return ePluginTypeRandomConnectRelay;
    case eAppletClientShareFiles:           return ePluginTypeFileServer;
    case eAppletClientShareWebCam:          return ePluginTypeCamServer;
    case eAppletClientStoryboard:           return ePluginTypeStoryboard;
    case eAppletClientRelay:                return ePluginTypeRelay;

    case eAppletServiceAboutMe:              return ePluginTypeAboutMePage;
    case eAppletServiceAvatarImage:          return ePluginTypeAvatarImage;
    case eAppletServiceConnectionTest:       return ePluginTypeConnectTest;
    case eAppletServiceHostGroup:            return ePluginTypeHostGroup;
    case eAppletServiceHostGroupListing:     return ePluginTypeHostGroupListing;
    case eAppletServiceHostNetwork:          return ePluginTypeHostNetwork;
    case eAppletServiceRandomConnect:         return ePluginTypeRandomConnect;
    case eAppletServiceRandomConnectRelay:    return ePluginTypeRandomConnectRelay;
    case eAppletServiceShareFiles:           return ePluginTypeFileServer;
    case eAppletServiceShareWebCam:          return ePluginTypeCamServer;
    case eAppletServiceStoryboard:           return ePluginTypeStoryboard;
    case eAppletServiceRelay:                return ePluginTypeRelay;

    case eAppletSettingsAboutMe:            return ePluginTypeAboutMePage;
    case eAppletSettingsAvatarImage:        return ePluginTypeAvatarImage;
    case eAppletSettingsWebCamServer:       return ePluginTypeCamServer;
    case eAppletSettingsConnectTest:        return ePluginTypeConnectTest;
    case eAppletSettingsShareFiles:         return ePluginTypeFileServer;
    case eAppletSettingsFileXfer:           return ePluginTypeFileXfer;
    case eAppletSettingsHostGroup:          return ePluginTypeHostGroup;
    case eAppletSettingsHostGroupListing:   return ePluginTypeHostGroupListing;
    case eAppletSettingsHostNetwork:        return ePluginTypeHostNetwork;
    case eAppletSettingsMessenger:          return ePluginTypeMessenger;
    case eAppletSettingsRandomConnect:      return ePluginTypeRandomConnect;
    case eAppletSettingsRandomConnectRelay: return ePluginTypeRandomConnectRelay;
    case eAppletSettingsRelay:              return ePluginTypeRelay;
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
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeConnectTest:            return eAppletUnknown;
    case ePluginTypeFileServer:             return eAppletUnknown;
    case ePluginTypeFileXfer:               return eAppletUnknown;
    case ePluginTypeHostGroup:              return eAppletUnknown;
    case ePluginTypeHostGroupListing:       return eAppletUnknown;
    case ePluginTypeHostNetwork:            return eAppletUnknown;
    case ePluginTypeRandomConnect:          return eAppletUnknown;
    case ePluginTypeRandomConnectRelay:     return eAppletUnknown;
    case ePluginTypeStoryboard:             return eAppletUnknown;
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
    case ePluginTypeConnectTest:            return eAppletSettingsConnectTest;
    case ePluginTypeFileServer:             return eAppletSettingsShareFiles;
    case ePluginTypeFileXfer:               return eAppletSettingsFileXfer;
    case ePluginTypeHostGroup:              return eAppletSettingsHostGroup;
    case ePluginTypeHostGroupListing:       return eAppletSettingsHostGroupListing;
    case ePluginTypeHostNetwork:            return eAppletSettingsHostNetwork;
    case ePluginTypeMessenger:              return eAppletSettingsMessenger;
    case ePluginTypeRandomConnect:          return eAppletSettingsRandomConnect;
    case ePluginTypeRandomConnectRelay:     return eAppletSettingsRandomConnectRelay;
    case ePluginTypeRelay:                  return eAppletSettingsRelay;
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
EMyIcons GuiHelpers::pluginTypeToSettingsIcon( EPluginType pluginType )
{
    EMyIcons iconType = eMyIconUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eMyIconSettingsAboutMe;
    case ePluginTypeAvatarImage:            return eMyIconSettingsAvatarImage;
    case ePluginTypeCamServer:              return eMyIconSettingsShareWebCam;
    case ePluginTypeConnectTest:            return eMyIconSettingsConnectionTest;
    case ePluginTypeFileServer:             return eMyIconSettingsShareFiles;
    case ePluginTypeFileXfer:               return eMyIconSettingsFileXfer;
    case ePluginTypeHostGroup:              return eMyIconSettingsHostGroup;
    case ePluginTypeHostGroupListing:       return eMyIconSettingsHostGroupListing;
    case ePluginTypeHostNetwork:            return eMyIconSettingsHostNetwork;
    case ePluginTypeMessenger:              return eMyIconSettingsMessenger;
    case ePluginTypeRandomConnect:          return eMyIconSettingsRandomConnect;
    case ePluginTypeRandomConnectRelay:     return eMyIconSettingsRandomConnectRelay;
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
EApplet GuiHelpers::pluginTypeToViewApplet( EPluginType pluginType )
{
    EApplet appletType = eAppletUnknown;

    switch( pluginType )
    {
    case ePluginTypeAboutMePage:            return eAppletEditAboutMe;
    case ePluginTypeAvatarImage:            return eAppletEditAvatarImage;
    case ePluginTypeCamServer:              return eAppletUnknown;
    case ePluginTypeConnectTest:            return eAppletUnknown;
    case ePluginTypeFileServer:             return eAppletUnknown;
    case ePluginTypeFileXfer:               return eAppletUnknown;
    case ePluginTypeHostGroup:              return eAppletUnknown;
    case ePluginTypeHostGroupListing:       return eAppletUnknown;
    case ePluginTypeHostNetwork:            return eAppletUnknown;
    case ePluginTypeRandomConnect:          return eAppletUnknown;
    case ePluginTypeRandomConnectRelay:     return eAppletUnknown;
    case ePluginTypeStoryboard:             return eAppletUnknown;
    case ePluginTypeRelay:                  return eAppletUnknown;
    default:
        break;
    }

    return appletType;
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
	default:
		break;
	}

	return isSingleSessionPlugin;
}

//============================================================================
std::string GuiHelpers::describePlugin( EPluginType ePluginType, bool rmtInitiated )
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

    case ePluginTypeConnectTest:
        strPluginDesc = QObject::tr( "Connection Test Service" ).toUtf8().constData();
        break;

    case ePluginTypeFileXfer:
        strPluginDesc = QObject::tr( "Person To Person File Transfer" ).toUtf8().constData();
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

    case ePluginTypeHostGroup:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Hosted Group Service" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Group Host Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeHostGroupListing:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Hosted Group List Search Service" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Group Host List Search Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeMessenger:
        strPluginDesc = QObject::tr( "Messanger Service" ).toUtf8().constData();
        break;

    case ePluginTypeHostNetwork:
        strPluginDesc = QObject::tr( "Host GoTvPtoP Network Service" ).toUtf8().constData();
        break;

    case ePluginTypeRelay:
        strPluginDesc = QObject::tr( "Relay Service" ).toUtf8().constData();
        break;

    case ePluginTypeRandomConnect:
        strPluginDesc = QObject::tr( "Connect To Random Person Service" ).toUtf8().constData();
        break;

    case ePluginTypeRandomConnectRelay:
        strPluginDesc = QObject::tr( "Connect To Random Person Relay Service" ).toUtf8().constData();
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
        strPluginDesc = QObject::tr( "Truth Or Dare Game With Video Chat" ).toUtf8().constData();
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
QWidget * GuiHelpers::getParentPageFrame( QWidget * curWidget )
{
    QWidget * parentActivity = nullptr;
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
            parentActivity = dynamic_cast<QWidget *>( curParent );
            if( parentActivity )
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

    return parentActivity;
}


//============================================================================
bool GuiHelpers::validateUserName( QWidget * curWidget, QString strUserName )
{
    if( strUserName.contains( "GoTv PtoP Web" )
        || strUserName.contains( "GoTvPtoPWeb" )
        || strUserName.contains( "gotvptopweb" )
        || strUserName.contains( "gotv ptop web" )
        || strUserName.contains( "GoTv PtoP Web" )
        || strUserName.contains( "GoTvPtoPWeb" )
        || strUserName.contains( "gotvptopweb" )
        || strUserName.contains( "gotv ptop web" ) )
    {
        QMessageBox::warning( curWidget, QObject::tr( "Invalid User Name" ), QObject::tr( "User Name cannot have GoTv PtoP Web in name." ) );
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
QString GuiHelpers::describeGender( EGenderType gender )
{
    switch( gender )
    {
    case eGenderTypeUnspecified:
        return QObject::tr( "Any" );
    case eGenderTypeMale:
        return QObject::tr( "Male" );
    case eGenderTypeFemale:
        return QObject::tr( "Female" );
    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
void GuiHelpers::fillGender( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxGenderType; i++ )
        {
            comboBox->addItem( describeGender( (EGenderType)i ) );
        }
    }
}


//============================================================================
QString GuiHelpers::describePreferredContent( EContentType content )
{
    switch( content )
    {
    case eContentUnspecified:
        return QObject::tr( "Any" );
    case eContentFamily:
        return QObject::tr( "Family" );
    case eContentAdult:
        return QObject::tr( "Adult" );
    case eContentXXX:
        return QObject::tr( "XXX" );
    case eContentDarkWeb:
        return QObject::tr( "DarkWeb" );
    case eContentPersonal:
        return QObject::tr( "Personal" );

    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
void GuiHelpers::fillContentRating( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxContentType; i++ )
        {
            comboBox->addItem( describePreferredContent( (EContentType)i ) );
        }
    }
}

//============================================================================
QString GuiHelpers::describeLanguage( ELanguageType language )
{
    switch( language )
    {
    case eLanguageUnspecified:
        return QObject::tr( "Any" );
    case eLanguageEnglishUS:
        return QObject::tr( "English (USA)" );
    case eLanguageEnglishBritian:
        return QObject::tr( "English (Britain)" );

    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
void GuiHelpers::fillLanguage( QComboBox * comboBox )
{
    if( comboBox )
    {
        comboBox->clear();
        for( int i = 0; i < eMaxLanguageType; i++ )
        {
            comboBox->addItem( describeLanguage( (ELanguageType)i ) );
        }
    }
}

//============================================================================
QString GuiHelpers::describeAge( int age )
{
    QString ageStr( age );
    if( ( age > 80 ) || ( age < 0 ) )
    {
        ageStr = "Old";
    }
    else if( age == 0 )
    {
        ageStr = "Any";
    }

    return ageStr;
}

//============================================================================
void GuiHelpers::setValuesFromIdentity( QWidget * curWidget, VxNetIdent * ident, QLineEdit * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
{
    if( curWidget && ident && age && genderCombo && languageCombo && contentCombo )
    {
        QString ageStr = QString::number( ident->getAge() );
        age->setText( ageStr );
        genderCombo->setCurrentIndex( ident->getGender() );
        languageCombo->setCurrentIndex( ident->getPrimaryLanguage() );
        contentCombo->setCurrentIndex( ident->getPreferredContent() );
    }
}

//============================================================================
void GuiHelpers::setIdentityFromValues( QWidget * curWidget, VxNetIdent * ident, QLineEdit * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo )
{
    if( curWidget && ident && age && genderCombo && languageCombo && contentCombo )
    {
        int ageValue = age->text().toInt();
        if( ( 0 > ageValue ) || ( 120 < ageValue ) )
        {
            ageValue = 0;
        }

        ident->setAge( ageValue );

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
        if( ( 0 > contentValue ) || ( eMaxContentType <= genderValue ) )
        {
            contentValue = 0;
        }

        ident->setPreferredContent( contentValue );
    }
}
