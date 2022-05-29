#pragma once
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


#include <GuiInterface/IDefs.h>
#include <CoreLib/VxGUID.h>

#include <QString>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

class ActivityBase;
class AppCommon;
class AppletBase;
class PluginSetting;
class PluginSettingsWidget;
class VxNetIdent;

class QComboBox;
class QFrame;
class QFontMetrics;
class QImage;
class QPixmap;
class QWidget;

class GuiHelpers
{
public:
    static QString				getAvailableStorageSpaceText();
    static QString				getJustFileName( QString& fileNameAndPath );

	static QString				browseForDirectory( QString startDir = "", QWidget * parent = 0 );
    static int					calculateTextHeight( QFontMetrics& fontMetrics, QString textStr );
    static bool					copyResourceToOnDiskFile( QString resourcePath, QString fileNameAndPath );

    static void					splitPathAndFileName( QString& fileNameAndPath, QString& retFileName, QString& retPath );

    static bool                 validateUserName( QWidget * curWidget, QString strUserName );
    static bool                 validateMoodMessage( QWidget * curWidget, QString strMoodMsg );
    static bool                 validateAge( QWidget * curWidget, int age );

    static void                 fillPermissionComboBox( QComboBox * permissionComboBox );
    static EFriendState         comboIdxToFriendState( int comboIdx );
    static int                  friendStateToComboIdx( EFriendState friendState );

    static void                 fillAge( QComboBox * comboBox );
    static EAgeType             getAge( QComboBox * comboBox );
    static bool                 setAge( QComboBox * comboBox, EAgeType ageType );
    static uint8_t              ageToIndex( EAgeType age );

    static void                 fillGender( QComboBox * comboBox );
    static EGenderType          getGender( QComboBox * comboBox );
    static bool                 setGender( QComboBox * comboBox, EGenderType gender );
    static uint8_t              genderToIndex( EGenderType gender );


    static void                 fillContentRating( QComboBox * comboBox );
    static EContentRating       getContentRating( QComboBox * comboBox );
    static bool                 setContentRating( QComboBox * comboBox, EContentRating contentRating );
    static uint8_t              contentRatingToIndex( EContentRating content );

    static void                 fillContentCatagory( QComboBox * comboBox );
    static uint8_t              contentCatagoryToIndex( EContentCatagory content );

    static void                 fillJoinRequest( QComboBox* comboBox );
    static EJoinState           comboIdxToJoinState( int comboIdx );
    static uint8_t              joinRequestToIndex( EJoinState joinState );

    static bool                 createThumbFileName( VxGUID& assetGuid, QString& retFileName );
    static bool                 makeCircleImage( QImage& image );
    static bool                 makeCircleImage( QPixmap& targetPixmap );
    static uint64_t             saveToPngFile( QImage& image, QString& fileName ); // returns file length
    static uint64_t             saveToPngFile( QPixmap& pixmap, QString& fileName ); // returns file length

    static bool                 checkUserPermission( QString permissionName ); // returns false if user denies permission to use android hardware
};

