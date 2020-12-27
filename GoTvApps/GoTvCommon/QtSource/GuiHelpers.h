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
// http://www.nolimitconnect.com
//============================================================================

#include "config_gotvapps.h"
#include "AppDefs.h"

#include <NetLib/VxXferDefs.h>
#include <PktLib/VxCommon.h>

#include <QString>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

class AppCommon;
class QWidget;
class QFontMetrics;
class QComboBox;
class QLineEdit;
class VxNetIdent;
class ActivityBase;
class AppletBase;

class GuiHelpers
{
public:
    static bool				    isCameraSourceAvailable();

    static QString				getAvailableStorageSpaceText();
    static QString				getJustFileName( QString& fileNameAndPath );

	static QString				browseForDirectory( QString startDir = "", QWidget * parent = 0 );
    static int					calculateTextHeight( QFontMetrics& fontMetrics, QString textStr );
    static bool					copyResourceToOnDiskFile( QString resourcePath, QString fileNameAndPath );

	static bool				    playFile( AppCommon& myApp, QString fullFileName, int pos0to100000 );
    static void					splitPathAndFileName( QString& fileNameAndPath, QString& retFileName, QString& retPath );

    static bool                 isAppletAService( EApplet applet );
    static bool                 isAppletAClient( EApplet applet );

    static EPluginType          getAppletAssociatedPlugin( EApplet applet );
    static EApplet              pluginTypeToEditApplet( EPluginType pluginType );
    static EApplet              pluginTypeToSettingsApplet( EPluginType pluginType );
    static EMyIcons             pluginTypeToSettingsIcon( EPluginType pluginType );
    static EApplet              pluginTypeToUserApplet( EPluginType pluginType );
    static EApplet              pluginTypeToViewApplet( EPluginType pluginType );

    static bool                 isPluginSingleSession( EPluginType ePluginType );
    //! which plugins to show in permission list
    static bool                 isPluginAPrimaryService( EPluginType ePluginType );
    static bool                 getSecondaryPlugins( EPluginType ePluginType, QVector<EPluginType> secondaryPlugins );

    static std::string			describePlugin( EPluginType ePluginType, bool rmtInitiated );
	static const char *			describeResponse( EOfferResponse eOfferResponse );
	static const char *			describePluginPermission( EFriendState ePluginPermission );
	static QString				describeEXferState( EXferState xferState );
	static QString				describeEXferError( EXferError xferError );
	static QString				describeFileLength( uint64_t fileLen );
    static QString				describeOrientation( Qt::Orientation qtOrientation );

    /// @brief get home or messenger topmost widget
    static QFrame *             getParentPageFrame( QWidget * curWidget );
    /// @brief messenger frame topmost widget
    static QFrame *             getMessengerPageFrame( QWidget * curWidget );
    /// @brief get launch frame topmost widget
    static QFrame *             getLaunchPageFrame( QWidget * curWidget );
    /// @brief get opposite page frame of current widget's frame
    static QFrame *             getOppositePageFrame( QWidget * curWidget );

    static bool                 validateUserName( QWidget * curWidget, QString strUserName );
    static bool                 validateMoodMessage( QWidget * curWidget, QString strMoodMsg );
    static bool                 validateAge( QWidget * curWidget, int age );

    static void                 setValuesFromIdentity( QWidget * curWidget, VxNetIdent * ident, QLineEdit * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo );
    static void                 setIdentityFromValues( QWidget * curWidget, VxNetIdent * ident, QLineEdit * age, QComboBox * genderCombo, QComboBox * languageCombo, QComboBox * contentCombo );

    static void                 fillPermissionComboBox( QComboBox * permissionComboBox );
    static EFriendState         comboIdxToFriendState( int comboIdx );
    static int                  friendStateToComboIdx( EFriendState friendState );

    static QString              describeAge( int age );

    static void                 fillGender( QComboBox * comboBox );
    static QString              describeGender( EGenderType gender );

    static void                 fillLanguage( QComboBox * comboBox );
    static QString              describeLanguage( ELanguageType language );
    static uint16_t             languageToIndex( ELanguageType language );

    static void                 fillContentRating( QComboBox * comboBox );
    static QString              describeContentRating( EContentRating content );
    static uint16_t             contentRatingToIndex( EContentRating content );

    static void                 fillContentCatagory( QComboBox * comboBox );
    static QString              describeContentCatagory( EContentCatagory content );
    static uint16_t             contentCatagoryToIndex( EContentCatagory content );

    static ActivityBase *       findParentActivity( QWidget * widget );
    static QWidget *            findAppletContentFrame( QWidget * widget );
    static ActivityBase *       findLaunchWindow( QWidget * widget );
    static AppletBase *         findParentApplet( QWidget * widget );


};

