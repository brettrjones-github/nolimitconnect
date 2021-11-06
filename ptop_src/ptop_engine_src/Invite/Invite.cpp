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
#include <CoreLib/VxParse.h>
#include <CoreLib/VxUrl.h>

#include <algorithm>

const char* Invite::INVITE_HDR_DIRECT_CONNECT = "!DirectConnectInvite!";
const char* Invite::INVITE_HDR_RELAYED = "!Invite!";
const char* Invite::INVITE_HDR_MSG = "!Message!";
const char* Invite::INVITE_HDR_NET_SETTING = "!NetworkSettings!";
const char* Invite::INVITE_END = "!End!";
const char* Invite::PTOP_URL_PREFIX = "ptop://";
const char Invite::SUFFIX_CHAR_PERSON = 'P'; // P
const char Invite::SUFFIX_CHAR_GROUP = 'G'; // G
const char Invite::SUFFIX_CHAR_CHAT_ROOM = 'C'; // C
const char Invite::SUFFIX_CHAR_RANDOM_CONNECT = 'R'; // R
const char Invite::SUFFIX_CHAR_NETWORK_HOST = 'N'; // N
const char Invite::SUFFIX_CHAR_CONNECT_TEST = 'T'; // T
const char Invite::SUFFIX_CHAR_UNKNOWN = ' '; // ' '

//============================================================================
void Invite::clearInvite( void )
{

    m_CanDirectConnect = false;
    m_InviteText.clear();

    m_PersonUrl.clear();
    m_GroupUrl.clear();
    m_ChatRoomUrl.clear();
    m_RandomConnectUrl.clear();

    m_NetSettingGroupUrl.clear();
    m_NetSettingChatRoomUrl.clear();
    m_NetSettingRandomConnectUrl.clear();
    m_NetSettingNetworkHostUrl.clear();
    m_NetSettingConnectTestUrl.clear();
}

//============================================================================
bool Invite::setInviteText( std::string inviteText )
{
    clearInvite();
    m_InviteText = inviteText;
    return parseInviteText();
}

//============================================================================
bool Invite::setInviteUrl( EHostType hostType, std::string& url )
{
    bool result = false;
    switch( hostType )
    {
    case eHostTypeGroup:
        m_GroupUrl = url;
        result = !m_GroupUrl.empty();
        break;

    case eHostTypeChatRoom:
        m_ChatRoomUrl = url;
        result = !m_ChatRoomUrl.empty();
        break;

    case eHostTypeRandomConnect:
        m_RandomConnectUrl = url;
        result = !m_RandomConnectUrl.empty();
        break;

    case eHostTypePeerUser:
        m_PersonUrl = url;
        result = !m_PersonUrl.empty();
        break;

    default:
        break;
    }

    return result;
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
    std::vector<std::string> strList;
    if( result )
    {
        m_InviteText.erase( std::remove( m_InviteText.begin(), m_InviteText.end(), '\r' ), m_InviteText.end() );
        StdStringSplit( m_InviteText, '\n', strList );
        result &= !strList.empty();
    }

    if( result )
    {
        bool foundStart = false;
        bool foundEnd = false;
        bool foundUrl = false;
        for( auto& str : strList )
        {
            if( !foundStart )
            {
                if( str == INVITE_HDR_DIRECT_CONNECT )
                {
                    m_CanDirectConnect = true;
                    foundStart = true;
                }
                else if( str == INVITE_HDR_RELAYED )
                {
                    m_CanDirectConnect = false;
                    foundStart = true;
                }

                continue;
            }
            
            if( !foundEnd )
            {
                if( str == INVITE_HDR_MSG || str == INVITE_HDR_NET_SETTING || str == INVITE_END )
                {
                    foundEnd = true;
                    break;
                }
            }

            if( str.length() > 10 && str.rfind( PTOP_URL_PREFIX, 0 ) == 0 )
            {
                // a url that begins with ptop://
                // 
                // it should have a Suffix letter that indicates the host type
                char endChar = str.at( str.length() - 1 );
                EHostType hostType = getHostTypeFromSuffix( endChar );
                if( hostType != eHostTypeUnknown && getInviteUrl( hostType ).empty() )
                {
                    str.erase( str.begin() + str.length() - 1 );
                    VxUrl url( str.c_str() );
                    if( url.getPort() > 0 )
                    {
                        setInviteUrl( hostType, str );
                        foundUrl = true;
                    }
                }
            }
        }

        result &= foundStart && foundEnd && foundUrl;
    }

    return result;
}

