//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#pragma once

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>
#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxFileTypeMasks.h>

#include <QColor>
#include <QRect>
#include <QSize>
#include <QCamera>

enum EButtonSize
{
    eButtonSizeTiny,
    eButtonSizeSmall,
    eButtonSizeMedium,
    eButtonSizeLarge,

    eMaxButtonSize
};

class GuiUser;

class GuiParams
{
public:
    GuiParams();
    virtual ~GuiParams() = default;

    static const int            MIN_TITLE_BAR_BUTTON_SIZE = 30;

    /// @brief initialize gui scaling etc
    static void                 initGuiParams(int defaultFontHeight);

    static int                  getDefaultFontHeight( void )        { return m_DefaultFontHeight; }
    static int                  getMaxWidgetSize( void )            { return 16777215; }

    /// @brief get scaling required to make icons etc. usable on high dpi screens
    static float                getGuiScale( void )                 { return m_DisplayScale; }
    /// @brief calculate widget scaling required to make icons etc. usable on high dpi screens
    static int                  getScaled( int param )              { return (int)(m_DisplayScale * param); }
    /// @brief calculate widget scaling required to make icons etc. usable on high dpi screens
    static QSize                getScaled( QSize size )             { return QSize( getScaled( size.width() ), getScaled( size.height() ) ); }
    /// @brief calculate widget scaling required to make icons etc. usable on high dpi screens
    static QRect                getScaled( QRect rect )             { return QRect( rect.left(), rect.top(), getScaled( rect.width() ), getScaled( rect.height() ) ); }

    /// @brief get scaling required to make icons etc. usable on high dpi screens
    static QSize                getButtonSize( EButtonSize buttonSize = eButtonSizeSmall );
    /// @brief get maximum tiled icon size
    static int                  getMaxTiledIconSize( void );

    /// @brief get height scaling required to list file entries
    static int                  getFileListEntryHeight( void );

    static int                  getControlIndicatorWidth( void );
    /// @brief thumbnails are square so this is both width and height
    static QSize                getThumbnailSize( void );
    /// @brief camera video size desired for processing
    static QSize                getSnapshotDesiredSize( void );
    /// @brief camera video size mulitiplied by display scale
    static QSize                getSnapshotScaledSize( void );

    static bool                 canFitIcons( EButtonSize buttonSize, int iconCnt, int sizeWidth );

    static QString				describeAge( EAgeType gender );
    static QString				describeAge( int age );

    static QString				describeCommError( ECommErr commErr );
    static QString				describeContentCatagory( EContentCatagory content );
    static QString				describeContentRating( EContentRating content );
    static QString				describeFriendState( EFriendState eFriendState );
    static QString				describeGender( EGenderType gender );
    static QString				describeHostType( EHostType hostType );
    static EHostType			hostTypeToEnum( QString hostType );

    static QString				describeHostSearchStatus( EHostSearchStatus searchStatus );
    static QString				describeLanguage( ELanguageType language );
    //! Describe permission level ( and/or friend state )
    static QString				describePermissionLevel( EFriendState friendState );
    //! Describe type of plugin
    static QString				describePluginAccess( EPluginAccess accessState );
    //! Describe action user can take for given plugin and access
    static QString				describePluginAction( GuiUser* guiUser, EPluginType ePluginType, EPluginAccess ePluginAccess );
    static QString              describePluginMsg( EPluginMsgType pluginMsgType );
    static QString				describePluginPermission( EFriendState ePluginPermission );
    static QString				describePluginType( EPluginType ePluginType );
    static std::string			describePlugin( EPluginType ePluginType, bool rmtInitiated );
    static QString				describePluginOffer( EPluginType ePluginType );
    static QString				describeEXferState( EXferState xferState );
    static QString				describeEXferError( EXferError xferError );
    static QString				describeFileLength( uint64_t fileLen );
    static QString				describeOfferState( EOfferState offerState );
    static QString				describeOfferType( EOfferType offerType );
    static QString				describeOrientation( Qt::Orientation qtOrientation );

    static QString              describeUserViewType( EUserViewType showFriendType );

    static QString              describeStatus( EHostAnnounceStatus hostStatus );
    static QString              describeStatus( EHostJoinStatus hostStatus );
    static QString              describeStatus( EHostSearchStatus hostStatus );
    static QString				describeResponse( EOfferResponse eOfferResponse );
    static QString				describeFriendship( EFriendState friendState, bool inGroup );

    static QString				describeJoinState( EJoinState joinState );

    static QString				describeFileFilter( EFileFilterType fileFilterType );
    static EFileFilterType		fileFilterToEnum( QString fileFilterName );

    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    static QString              describeCamStatus(QCamera::Status camStatus);
    static QString              describeCamState(QCamera::State camState);
    #endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

    static uint8_t              assetTypeToFileType( EAssetType assetType );
    static EAssetType           fileTypeToAssetType( uint8_t fileTypeFlags );

    static QString              describeFileType( uint8_t fileType );

protected:

    static QColor				m_OnlineBkgColor;
    static QColor				m_OfflineBkgColor;
    static QColor				m_NearbyBkgColor;

    static int                  m_DefaultFontHeight;
    static float                m_DisplayScale;
    static int                  m_TinyPushButtonSize;
    static int                  m_SmallPushButtonSize;
    static int                  m_MediumPushButtonSize;
    static int                  m_LargePushButtonSize;
};
