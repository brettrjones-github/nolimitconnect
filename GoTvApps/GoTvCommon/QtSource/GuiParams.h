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
// http://www.nolimitconnect.com
//============================================================================
#pragma once

#include <QColor>
#include <QSize>

class GuiParams
{
public:
    GuiParams();
    virtual ~GuiParams() = default;

    static const int            MIN_TITLE_BAR_BUTTON_SIZE = 30;
    static const int            MIN_PUSHBUTTON_SIZE = 30;
    static const int            SMALL_PUSHBUTTON_SIZE = 32;
    static const int            MEDIUM_PUSHBUTTON_SIZE = 48;

    /// @brief initialize gui scaling etc
    static void                 initGuiParams();

    /// @brief get scaling required to make icons etc. usable on high dpi screens
    static int                  getGuiScale( void )  { return m_DisplayScale; }
    /// @brief get scaling required to make icons etc. usable on high dpi screens
    static int                  getButtonSize( void )  { return MIN_PUSHBUTTON_SIZE * m_DisplayScale; }

    static int                  getControlIndicatorWidth( void );
    /// @brief thumbnails are square so this is both width and height
    static QSize                getThumbnailSize( void );
    /// @brief camera video size mulitiplied by display scale
    static QSize                getSnapshotSize( void );

    static QString				describeAge( EAgeType gender );
    static QString				describeAge( int age );
    static QString				describeCommError( ECommErr commErr );
    static QString				describeContentCatagory( EContentCatagory content );
    static QString				describeContentRating( EContentRating content );
    static QString				describeFriendState( EFriendState eFriendState );
    static QString				describeGender( EGenderType gender );
    static QString				describeHostSearchStatus( EHostSearchStatus searchStatus );
    static QString				describeLanguage( ELanguageType language );
    //! Describe permission level ( and/or friend state )
    static QString				describePermissionLevel( EFriendState friendState );
    //! Describe type of plugin
    static QString				describePluginAccess( EPluginAccess accessState );
    //! Describe action user can take for given plugin and access
    static QString				describePluginAction( GuiUser * netIdent, EPluginType ePluginType, EPluginAccess ePluginAccess );
    static QString				describePluginPermission( EFriendState ePluginPermission );
    static QString				describePluginType( EPluginType ePluginType );
    static std::string			describePlugin( EPluginType ePluginType, bool rmtInitiated );
    static QString				describePluginOffer( EPluginType ePluginType );
    static QString				describeEXferState( EXferState xferState );
    static QString				describeEXferError( EXferError xferError );
    static QString				describeFileLength( uint64_t fileLen );
    static QString				describeOrientation( Qt::Orientation qtOrientation );

    static QString              describeStatus( EHostAnnounceStatus hostStatus );
    static QString              describeStatus( EHostJoinStatus hostStatus );
    static QString              describeStatus( EHostSearchStatus hostStatus );
    static QString				describeResponse( EOfferResponse eOfferResponse );
    static QString				describeFriendship( EFriendState friendState );

protected:

    static QColor				m_OnlineBkgColor;
    static QColor				m_OfflineBkgColor;
    static QColor				m_NearbyBkgColor;

    static int                  m_DisplayScale;
};
