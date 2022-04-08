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

#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/ObjectCommon.h>
#include <CoreLib/VxDebug.h>

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
#  include <QtCore/6.2.4/QtCore/private/qandroidextras_p.h>
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

int convertToRgb()
{
    QImage image( 100, 100, QImage::Format_RGB888 );
    image.fill( QColor( 50, 100, 200 ) );

    if( image.format() == QImage::Format_Invalid )
        return EXIT_FAILURE;
    QVideoFrameFormat video_frame_format( image.size(), QVideoFrameFormat::Format_RGBX8888 );
    QImage rgbx = image.convertToFormat( QVideoFrameFormat::imageFormatFromPixelFormat( video_frame_format.pixelFormat() ) );
    QVideoFrame video_frame( video_frame_format );
    if( !video_frame.isValid() || !video_frame.map( QVideoFrame::WriteOnly ) ) {
        qWarning() << "QVideoFrame is not valid or not writable";
        return EXIT_FAILURE;
    }
    int plane = 0;
    std::memcpy( video_frame.bits( plane ), rgbx.bits(), video_frame.mappedBytes( plane ) );
    video_frame.unmap();

    qDebug() << video_frame << rgbx.format();
    return 0;
}
