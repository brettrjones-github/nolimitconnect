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
#include "Invite.h"

const char* Invite::INVITE_HDR_DIRECT_CONNECT = "!DirectConnectInvite!";
const char* Invite::INVITE_HDR_RELAYED = "!Invite!";
const char* Invite::INVITE_HDR_MSG = "!Message!";
const char* Invite::INVITE_HDR_NET_SETTING = "!NetworkSettings!";
const char* Invite::INVITE_END = "!End!";
const char Invite::SUFFIX_CHAR_PERSON = 'P'; // P
const char Invite::SUFFIX_CHAR_GROUP = 'G'; // G
const char Invite::SUFFIX_CHAR_CHAT_ROOM = 'C'; // C
const char Invite::SUFFIX_CHAR_RANDOM_CONNECT = 'R'; // R
const char Invite::SUFFIX_CHAR_NETWORK_HOST = 'N'; // N
const char Invite::SUFFIX_CHAR_CONNECT_TEST = 'T'; // T
const char Invite::SUFFIX_CHAR_UNKNOWN = ' '; // ' '

//============================================================================
bool Invite::setInviteText( std::string inviteText )
{
    m_InviteText = inviteText;
    return parseInviteText();
}

//============================================================================
std::string Invite::getInviteUrl( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return m_GroupUrl;

    case eHostTypeChatRoom:
        return m_ChatRoomUrl;

    case eHostTypeRandomConnect:
        return m_RandomConnectUrl;

    case eHostTypePeerUser:
        return m_PersonUrl;

    default:
        return std::string();
    }
}


//============================================================================
std::string Invite::getNetSettingUrl( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeNetwork:
        return m_NetSettingNetworkHostUrl;

    case eHostTypeConnectTest:
        return m_NetSettingConnectTestUrl;

    case eHostTypeGroup:
        return m_NetSettingGroupUrl;

    case eHostTypeChatRoom:
        return m_NetSettingChatRoomUrl;

    case eHostTypeRandomConnect:
        return m_NetSettingRandomConnectUrl;

    default:
        return std::string();
    }
}

//============================================================================
const char Invite::getHostTypeSuffix( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypePeerUser:
        return SUFFIX_CHAR_PERSON;

    case eHostTypeNetwork:
        return SUFFIX_CHAR_NETWORK_HOST;

    case eHostTypeConnectTest:
        return SUFFIX_CHAR_CONNECT_TEST;

    case eHostTypeGroup:
        return SUFFIX_CHAR_GROUP;

    case eHostTypeChatRoom:
        return SUFFIX_CHAR_CHAT_ROOM;

    case eHostTypeRandomConnect:
        return SUFFIX_CHAR_RANDOM_CONNECT;

    default:
        return SUFFIX_CHAR_UNKNOWN;
    }
}

//============================================================================
EHostType Invite::getHostTypeFromSuffix( const char suffix )
{
    switch( suffix )
    {
    case SUFFIX_CHAR_PERSON:
        return eHostTypePeerUser;

    case SUFFIX_CHAR_NETWORK_HOST:
        return eHostTypeNetwork;

    case SUFFIX_CHAR_CONNECT_TEST:
        return eHostTypeConnectTest;

    case SUFFIX_CHAR_GROUP:
        return eHostTypeGroup;

    case SUFFIX_CHAR_CHAT_ROOM:
        return eHostTypeChatRoom;

    case SUFFIX_CHAR_RANDOM_CONNECT:
        return eHostTypeRandomConnect;

    default:
        return eHostTypeUnknown;
    }
}

//============================================================================
bool Invite::parseInviteText( void )
{
    bool result = !m_InviteText.empty();



    return result;
}