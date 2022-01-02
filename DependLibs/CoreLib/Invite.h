#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <string>
#include <GuiInterface/IDefs.h>

class Invite
{
public:
    static const char * INVITE_HDR_DIRECT_CONNECT;
    static const char * INVITE_HDR_RELAYED;
    static const char * INVITE_HDR_MSG;
    static const char * INVITE_HDR_NET_SETTING;
    static const char * INVITE_END;
    static const char * PTOP_URL_PREFIX;        // ptop://
    static const char SUFFIX_CHAR_PERSON;       // P
    static const char SUFFIX_CHAR_GROUP;        // G
    static const char SUFFIX_CHAR_CHAT_ROOM;    // C
    static const char SUFFIX_CHAR_RANDOM_CONNECT; // R
    static const char SUFFIX_CHAR_NETWORK_HOST; // N
    static const char SUFFIX_CHAR_CONNECT_TEST; // T
    static const char SUFFIX_CHAR_UNKNOWN;      // ' '

    bool                        setInviteText( std::string inviteText );
    std::string&                getInviteText( void )                       { return m_InviteText; }

    bool                        canDirectConnect( void )                    { return m_CanDirectConnect; }
    bool                        setInviteUrl( EHostType hostType, std::string& url );
    std::string                 getInviteUrl( EHostType hostType );
    std::string                 getNetSettingUrl( EHostType hostType );

    static char                 getHostTypeSuffix( EHostType hostType );
    static EHostType            getHostTypeFromSuffix( const char suffix );
    static bool                 isValidHostTypeSuffix( const char suffix );

    static std::string          makeInviteUrl( EHostType hostType, std::string& onlineUrl ); // assumes vaild node url with online id

protected:
    bool                        parseInviteText( void );
    void                        clearInvite( void );

    bool                        m_CanDirectConnect{ false };
    std::string                 m_InviteText;

    std::string                 m_PersonUrl;
    std::string                 m_GroupUrl;
    std::string                 m_ChatRoomUrl;
    std::string                 m_RandomConnectUrl;

    std::string                 m_NetSettingGroupUrl;
    std::string                 m_NetSettingChatRoomUrl;
    std::string                 m_NetSettingRandomConnectUrl;
    std::string                 m_NetSettingNetworkHostUrl;
    std::string                 m_NetSettingConnectTestUrl;
};

