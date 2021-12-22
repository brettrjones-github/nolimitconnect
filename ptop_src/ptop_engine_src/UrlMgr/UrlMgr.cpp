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

#include "UrlMgr.h"

#include <NetLib/VxSktUtil.h>

//============================================================================
UrlMgr& GetUrlMgrInstance()
{
    static UrlMgr g_UrlMgr;
    return g_UrlMgr;
}

//============================================================================
void UrlInfo::updateOnlineId( VxGUID& onlineId )
{
    if( m_OnlineId == onlineId )
    {
        // no changes needed
        return;
    }

    m_OnlineId = onlineId;
    std::string url( "ptop://" );
    if( m_Url.size() > 7 && 0 == strncmp( m_Url.c_str(), "http://", 7 ) )
    {
        url = "http://";
    }

    url += m_IpAddr;
    url += ":";
    char as8Buf[32];
    sprintf( as8Buf, "%d", m_Port );
    url += as8Buf;
    url += "/";
    url += onlineId.toOnlineIdString();

    m_Url = url;
}


//============================================================================
bool UrlMgr::lookupOnlineId( std::string& hostUrl, VxGUID& onlineId )
{
    m_UrlMutex.lock();
    auto iter = m_UrlMap.find( hostUrl );
    if( iter != m_UrlMap.end() )
    {
        if( iter->second.m_OnlineId.isVxGUIDValid() )
        {
            onlineId = iter->second.m_OnlineId;
            m_UrlMutex.unlock();
            return true;
        }
    }

    m_UrlMutex.unlock();
    return false;
}

//============================================================================
void UrlMgr::setMyOnlineNodeUrl( std::string& myNodeUrl )
{
    UrlInfo urlInfo;
    if( fillUrlInfo( myNodeUrl, urlInfo ) )
    {
        m_MyNodeUrl = myNodeUrl;
        m_MyUrlInfo = urlInfo;
        m_UrlMutex.lock();
        for( auto iter = m_UrlMap.begin(); iter != m_UrlMap.end(); ++iter )
        {
            if( !iter->second.m_OnlineId.isVxGUIDValid() )
            {
                if( iter->second.m_Port == m_MyUrlInfo.m_Port && iter->second.m_IpAddr == m_MyUrlInfo.m_IpAddr )
                {
                    iter->second.updateOnlineId( m_MyUrlInfo.m_OnlineId );
                }
            }
        }

        m_UrlMutex.unlock();
    }
}

//============================================================================
std::string UrlMgr::resolveUrl( std::string& hostUrl )
{
    std::string url("");
    if( hostUrl.empty() )
    {
        return url;
    }

    m_UrlMutex.lock();
    auto iter = m_UrlMap.find( hostUrl );
    if( iter != m_UrlMap.end() )
    {
        url = iter->second.m_Url;
        m_UrlMutex.unlock();
        return url;
    }

    m_UrlMutex.unlock();

    UrlInfo urlInfo;
    if( fillUrlInfo( hostUrl, urlInfo ) )
    {
        url = urlInfo.m_Url;

        m_UrlMutex.lock();
        m_UrlMap[hostUrl] = urlInfo;
        m_UrlMutex.unlock();
    }

    return url;
}

//============================================================================
void UrlMgr::updateUrlCache( std::string& hostUrl, VxGUID& onlineId )
{
    m_UrlMutex.lock();
    auto iter = m_UrlMap.find( hostUrl );
    if( iter != m_UrlMap.end() )
    {
        if( onlineId != iter->second.m_OnlineId )
        {
            iter->second.updateOnlineId( onlineId );
        }

        m_UrlMutex.unlock();
        return;
    }

    m_UrlMutex.unlock();
    addUrlAndOnlineId( hostUrl, onlineId );
}

//============================================================================
bool UrlMgr::addUrl( std::string& hostUrl )
{
    bool urlAdded{ false };
    UrlInfo urlInfo;
    if( fillUrlInfo( hostUrl, urlInfo ) )
    {
        m_UrlMutex.lock();
        m_UrlMap[hostUrl] = urlInfo;
        m_UrlMutex.unlock();
        urlAdded = true;
    }

    return urlAdded;
}

//============================================================================
bool UrlMgr::addUrlAndOnlineId( std::string& hostUrl, VxGUID& onlineId )
{
    bool urlAdded{ false };
    UrlInfo urlInfo;
    if( fillUrlInfo( hostUrl, urlInfo ) )
    {
        urlInfo.m_OnlineId = onlineId;
        m_UrlMutex.lock();
        m_UrlMap[hostUrl] = urlInfo;
        m_UrlMutex.unlock();
        urlAdded = true;
    }

    return urlAdded;
}

//============================================================================
bool UrlMgr::fillUrlInfo( std::string& hostUrl, UrlInfo& urlInfo )
{
    std::string strHost;
    std::string strFile;
    uint16_t tcpPort{ 0 };

    bool result = VxSplitHostAndFile( hostUrl.c_str(), strHost, strFile, tcpPort );
    if( result )
    {
        bool resolvedUrlIp{ false };
        InetAddress inetAddr;
        urlInfo.m_Port = tcpPort;
        if( VxIsIPv4Address( strHost.c_str() ) || VxIsIPv6Address( strHost.c_str() ) )
        {
            urlInfo.m_IpAddr = strHost;
        }
        else if( VxResolveUrl( strHost.c_str(), tcpPort, inetAddr ) )
        {
            urlInfo.m_IpAddr = inetAddr.toStdString();
            resolvedUrlIp = true;
        }
        else
        {
            return false;
        }

        bool hadOnlineId = false;
        if( !strFile.empty() && strFile[0] == '!' )
        {
            // possibly has online id
            VxGUID onlineId;
            onlineId.fromOnlineIdString( strFile.c_str() );
            if( onlineId.isVxGUIDValid() )
            {
                urlInfo.m_OnlineId = onlineId;
                hadOnlineId = true;
            }
        }
        else if( urlInfo.m_Port == m_MyUrlInfo.m_Port && urlInfo.m_IpAddr == m_MyUrlInfo.m_IpAddr )
        {
            urlInfo.m_OnlineId = m_MyUrlInfo.m_OnlineId;
            strFile = urlInfo.m_OnlineId.toOnlineIdString();
            hadOnlineId = true;
        }

        std::string url( "ptop://" );
        if( hostUrl.size() > 7 && 0 == strncmp( hostUrl.c_str(), "http://", 7 ) )
        {
            url = "http://";
        }

        url += urlInfo.m_IpAddr;
        url += ":";
        char as8Buf[32];
        sprintf( as8Buf, "%d", tcpPort );
        url += as8Buf;
        if( hadOnlineId )
        {
            url += "/";
            url += strFile;
        }

        urlInfo.m_Url = url;
    }

    return result;
}