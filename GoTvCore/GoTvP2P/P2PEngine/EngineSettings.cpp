//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
#include "EngineSettings.h"
#include "EngineSettingsDefaultValues.h"
#include <NetLib/VxGetRandomPort.h>

#define MY_SETTINGS_DBVERSION 1
#define MY_SETTINGS_KEY "RCKEY"

//============================================================================
EngineSettings::EngineSettings()
: VxSettings( "EngineSettingsDb" )
, m_Initialized( false )
, m_CachedNetworkName("")
{
}

//============================================================================
EngineSettings::~EngineSettings()
{
	engineSettingsShutdown();
}

//============================================================================
RCODE EngineSettings::engineSettingsStartup( std::string& strDbFileName )
{
	if( m_Initialized )
	{
		engineSettingsShutdown();
	}

	RCODE rc = dbStartup(MY_SETTINGS_DBVERSION, strDbFileName.c_str());
	if( 0 == rc )
	{
		m_Initialized = true;
	}

	return rc;
}

//============================================================================
void EngineSettings::engineSettingsShutdown( void )
{
	if( m_Initialized )
	{
		dbShutdown();
		m_Initialized = false;
	}
}

//============================================================================
void EngineSettings::getNetSettings( NetSettings& netSettings )
{
    m_SettingsDbMutex.lock();
	std::string strValue;
	getNetworkKey( strValue );
	netSettings.setNetworkKey( strValue.c_str() );
	
	getNetHostWebsiteUrl( strValue );
	netSettings.setNetHostWebsiteUrl( strValue.c_str() );

	getNetServiceWebsiteUrl( strValue );
	netSettings.setNetServiceWebsiteUrl( strValue.c_str() );

	uint16_t u16Port = getTcpIpPort();
	netSettings.setMyTcpInPort( u16Port );

	getMulticastPort( u16Port );
	netSettings.setMyMulticastPort( u16Port );

	uint32_t usrCnt;
	uint32_t sysCnt;
	getMaxRelaysInUse( usrCnt, sysCnt );
	netSettings.setUserRelayPermissionCount( (int)usrCnt );
	netSettings.setSystemRelayPermissionCount( (int)sysCnt );
	netSettings.setAllowMulticastBroadcast( getAllowMulticastBroadcast() );
	netSettings.setAllowUserLocation( getAllowMulticastBroadcast() );
}

//============================================================================
void EngineSettings::setNetSettings( NetSettings& netSettings )
{
	setNetworkKey( netSettings.getNetworkKey() );
	setNetHostWebsiteUrl( netSettings.getNetHostWebsiteUrl() );
	setNetServiceWebsiteUrl( netSettings.getNetServiceWebsiteUrl() );

	setTcpIpPort( netSettings.getMyTcpInPort() );
	setMulticastPort( netSettings.getMyMulticastPort() );
	setMulticastEnable( netSettings.getMulticastEnable() );
	setUseUpnpPortForward( netSettings.getUseUpnpPortForward() );
	
	setMaxRelaysInUse( netSettings.getUserRelayPermissionCount(), netSettings.getSystemRelayPermissionCount() );
}

//============================================================================
EFriendViewType EngineSettings::getWhichContactsToView( void )
{
    m_SettingsDbMutex.lock();
	int32_t whichContactView = eFriendViewEverybody;
	getIniValue( MY_SETTINGS_KEY, "WhichContactsToView", whichContactView, eFriendViewEverybody );
    m_SettingsDbMutex.unlock();
	return (EFriendViewType)whichContactView;
}

//============================================================================
void EngineSettings::setWhichContactsToView( EFriendViewType eViewType )
{
    m_SettingsDbMutex.lock();
	int32_t whichContactView = (int32_t)eViewType;
	setIniValue( MY_SETTINGS_KEY, "WhichContactsToView", whichContactView );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getExternalIp( std::string& strIpAddress )
{
    strIpAddress = "";
    EngineSettings::EFirewallTestType firewallType = getFirewallTestSetting();
    if( eFirewallTestAssumeNoFirewall == firewallType )
    {
        // only get extern ip if set to assume can connect directly
        m_SettingsDbMutex.lock();
        getIniValue( MY_SETTINGS_KEY, "ExternalIp", strIpAddress, "" );
        m_SettingsDbMutex.unlock();
    }  
}

//============================================================================
void EngineSettings::setExternalIp( std::string& strIpAddress )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "ExternalIp", strIpAddress );
    m_SettingsDbMutex.unlock();
}

//============================================================================
uint16_t EngineSettings::getTcpIpPort( bool bGetRandomIfDoesntExist )
{
    uint16_t u16IpPort;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "TcpIpPort", u16IpPort, 0 );
    m_SettingsDbMutex.unlock();
	if( 0 == u16IpPort )
	{
        if( bGetRandomIfDoesntExist )
        {
            u16IpPort = VxGetRandomTcpPort();
        }
        else
        {
            u16IpPort = NET_DEFAULT_NETSERVICE_PORT;
        }

		setTcpIpPort( u16IpPort );
	}

    LogMsg( LOG_DEBUG, "BRJ EngineSettings::getTcpIpPort %d", u16IpPort );
    return u16IpPort;
}

//============================================================================
void EngineSettings::setTcpIpPort( uint16_t u16IpPort )
{
    m_SettingsDbMutex.lock();
    LogMsg( LOG_DEBUG, "BRJ EngineSettings::setTcpIpPort %d", u16IpPort );
    setIniValue( MY_SETTINGS_KEY, "TcpIpPort", u16IpPort );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getMulticastPort( uint16_t& u16IpPort )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "UdpIpPort", u16IpPort, DEFAULT_UDP_PORT );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setMulticastPort( uint16_t u16IpPort )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "UdpIpPort", u16IpPort );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setMulticastEnable( bool enableMulticast )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "MulticastEnable", enableMulticast );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getMulticastEnable( bool& enableMulticast )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "MulticastEnable", enableMulticast, false );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getNetServiceWebsiteUrl( std::string& strWebsiteUrl )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "NetServiceUrl", strWebsiteUrl, NET_DEFAULT_NETSERVICE_URL );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setNetServiceWebsiteUrl( std::string& strWebsiteUrl )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "NetServiceUrl", strWebsiteUrl );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getNetHostWebsiteUrl( std::string& strWebsiteUrl )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "HostUrl", strWebsiteUrl, NET_DEFAULT_NET_HOST_URL );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setNetHostWebsiteUrl( std::string& strWebsiteUrl )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "HostUrl", strWebsiteUrl );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setIsThisNodeAnNetHostOld( bool isHost )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "IsNodeHost", isHost );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getIsThisNodeAnNetHostOld( void )
{
	bool isHost = false;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "IsNodeHost", isHost, false );
    m_SettingsDbMutex.unlock();
	return isHost;
}

//============================================================================
void EngineSettings::setExcludeMeFromNetHostList( bool excludeFromHostList )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "ExcludeFromHostList", excludeFromHostList );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getExcludeMeFromNetHostList( void )
{
	bool excludeMe = false;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "ExcludeFromHostList", excludeMe, false );
    m_SettingsDbMutex.unlock();
	return excludeMe;
}

//============================================================================
void EngineSettings::getNetworkKey( std::string& strNetworkName )
{
	if( m_CachedNetworkName.length() )
	{
		strNetworkName = m_CachedNetworkName;
	}
	else
	{
        m_SettingsDbMutex.lock();
		getIniValue( MY_SETTINGS_KEY, "NetworkName", strNetworkName, NET_DEFAULT_NETWORK_NAME );
		m_CachedNetworkName = strNetworkName;
        m_SettingsDbMutex.unlock();
    }
}

//============================================================================
void EngineSettings::setNetworkKey( std::string& strNetworkName )
{
    m_SettingsDbMutex.lock();
	m_CachedNetworkName = strNetworkName;
	setIniValue( MY_SETTINGS_KEY, "NetworkName", strNetworkName );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getUseUpnpPortForward( void )
{
	bool bUseUpnp = true;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "UseUpnp", bUseUpnp, false );
    m_SettingsDbMutex.unlock();

	return bUseUpnp;
}

//============================================================================
void EngineSettings::setUseUpnpPortForward( bool bUseUpnpPortForward )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "UseUpnp", bUseUpnpPortForward );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getUseNatPortForward( void )
{
	bool bUseNat = true;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "UseNat", bUseNat, false );
    m_SettingsDbMutex.unlock();
	return bUseNat;
}

//============================================================================
void EngineSettings::setUseNatPortForward( bool bUseNatPortForward )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "UseNat", bUseNatPortForward );
    m_SettingsDbMutex.unlock();
}

//============================================================================
EngineSettings::EFirewallTestType EngineSettings::getFirewallTestSetting( void )
{
	uint16_t u16Setting;
	getIniValue( MY_SETTINGS_KEY, "FirewallTest", u16Setting, 0 );
	return (EngineSettings::EFirewallTestType)u16Setting;
}

//============================================================================
void EngineSettings::setFirewallTestSetting( EngineSettings::EFirewallTestType eFirewallTestType )
{
	uint16_t u16Setting = (uint16_t)eFirewallTestType;
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "FirewallTest", u16Setting );
    m_SettingsDbMutex.unlock();
}

//=========================================================================
//=== file share settings ===//
//============================================================================
void EngineSettings::getCompletedDnldsDir( std::string& strCompletedDir )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "CompletedDnldsDir", strCompletedDir, "" );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setCompletedDnldsDir( std::string& strCompletedDir )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "CompletedDnldsDir", strCompletedDir );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getIncompleteDnldsDir( std::string& strIncompleteDir )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "IncompleteDnldsDir", strIncompleteDir, "" );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setIncompleteDnldsDir( std::string& strIncompleteDir )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "IncompleteDnldsDir", strIncompleteDir );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getDnldsBandwidth( uint32_t& u32Bandwidth )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "DnldsBandwidth", u32Bandwidth, 0 );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setDnldsBandwidth( uint32_t& u32Bandwidth )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "DnldsBandwidth", u32Bandwidth );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getMaxDownloadingFiles( uint32_t& u32MaxDownloadingFiles )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "MaxDownloadingFiles", u32MaxDownloadingFiles, DEFAULT_MAX_DOWNLOADING_FILES );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setMaxDownloadingFiles( uint32_t& u32MaxDownloadingFiles )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "MaxDownloadingFiles", u32MaxDownloadingFiles );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getSharedFilesDirs( std::vector<std::string>& strSharedDirs )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "SharedDirs", strSharedDirs );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setSharedFilesDirs( std::vector<std::string>& strSharedDirs )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "SharedDirs", strSharedDirs );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getUpldsBandwidth( uint32_t& u32Bandwidth )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "UpldsBandwidth", u32Bandwidth, 0 );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setUpldsBandwidth( uint32_t& u32Bandwidth )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "UpldsBandwidth", u32Bandwidth );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getMaxUploadingFiles( uint32_t& u32MaxUploadingFiles )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "MaxUploadingFiles", u32MaxUploadingFiles, DEFAULT_MAX_UPLOADING_FILES );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setMaxUploadingFiles( uint32_t& u32MaxUploadingFiles )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "MaxUploadingFiles", u32MaxUploadingFiles );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setMaxRelaysInUse( uint32_t userRelays, uint32_t anonRelays )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "MaxUserRelays", userRelays );
	setIniValue( MY_SETTINGS_KEY, "MaxSystemRelays", anonRelays );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::getMaxRelaysInUse( uint32_t& userRelays, uint32_t& anonRelays )
{
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "MaxUserRelays", userRelays, DEFAULT_USER_RELAYS );
	getIniValue( MY_SETTINGS_KEY, "MaxSystemRelays", anonRelays, DEFAULT_ANON_RELAYS );
    m_SettingsDbMutex.unlock();
}

//============================================================================
void EngineSettings::setAllowUserLocation( bool allowUserLocation )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "AllowUserLocation", allowUserLocation );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getAllowUserLocation( void )
{
	bool allowUserLocation = true;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "AllowUserLocation", allowUserLocation, true );
    m_SettingsDbMutex.unlock();
	return allowUserLocation;
}

//============================================================================
void EngineSettings::setAllowMulticastBroadcast( bool allowBroadcast )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "AllowBroadcast", allowBroadcast );
    m_SettingsDbMutex.unlock();
}

//============================================================================
bool EngineSettings::getAllowMulticastBroadcast( void )
{
	bool allowBroadcast = true;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "AllowBroadcast", allowBroadcast, false );
    m_SettingsDbMutex.unlock();
	return allowBroadcast;
}

//============================================================================
void EngineSettings::setPreferredNetworkAdapterIp( const char * wirelessIpAddress )
{
	std::string strVal = "";
	if( wirelessIpAddress )
	{
		strVal = wirelessIpAddress;
	}

    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "AdapterIp", wirelessIpAddress );
    m_SettingsDbMutex.unlock();
}

//============================================================================
std::string EngineSettings::getPreferredNetworkAdapterIp( void )
{
	std::string strVal;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "AdapterIp", strVal, "" );
    m_SettingsDbMutex.unlock();
	return strVal;
}

//============================================================================
void EngineSettings::setLastFirewallPort( uint16_t u16IpPort )
{
    m_SettingsDbMutex.lock();
	setIniValue( MY_SETTINGS_KEY, "firewallPort", u16IpPort );
    m_SettingsDbMutex.unlock();
}

//============================================================================
uint16_t EngineSettings::getLastFirewallPort( void )
{
	uint16_t u16IpPort;
    m_SettingsDbMutex.lock();
	getIniValue( MY_SETTINGS_KEY, "firewallPort", u16IpPort, 0 );
    m_SettingsDbMutex.unlock();
	return u16IpPort;
}


