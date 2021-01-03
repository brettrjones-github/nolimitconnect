#pragma once
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

#include <NetLib/NetSettings.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxSettings.h>

class EngineSettings : public VxSettings
{
public:

	EngineSettings();
	virtual ~EngineSettings();

	RCODE						engineSettingsStartup( std::string& strDbFileName );
	void						engineSettingsShutdown( void );

    void						getNetHostSettings( NetHostSetting& netSettings );
    void						setNetHostSettings( NetHostSetting& netSettings );

	void						getNetSettings( NetSettings& netSettings );
	void						setNetSettings( NetSettings& netSettings );

	void						setWhichContactsToView( EFriendViewType eViewType );
	EFriendViewType				getWhichContactsToView( void );

	void						setExternalIp( std::string& strIpAddress );
	void						getExternalIp( std::string& strIpAddress );

	void						setTcpIpPort( uint16_t u16IpPort );
    uint16_t					getTcpIpPort( bool bGetRandomIfDoesntExist = true );

	void						setMulticastPort( uint16_t u16IpPort );
	void						getMulticastPort( uint16_t& u16IpPort );

	void						setMulticastEnable( bool enableMulticast );
	void						getMulticastEnable( bool& enableMulticast );

	void						setNetworkKey( std::string& strNetworkName );
	void						getNetworkKey( std::string& strNetworkName );

	void						setNetworkHostUrl( std::string& strWebsiteUrl );
	void						getNetworkHostUrl( std::string& strWebsiteUrl );

    void						setExternalIpAddr( std::string& externIp );
    void						getExternalIpAddr( std::string& externIp );

	void						setConnectTestType( int32_t testType );
    int32_t					    getConnectTestType( void );

    void						setUseUpnp( bool useUpnp );
    bool						getUseUpnp( void );

	void						setExcludeMeFromNetHostList( bool excludeFromHostList );
	bool						getExcludeMeFromNetHostList( void );

	void						setConnectTestUrl( std::string& strWebsiteUrl );
	void						getConnectTestUrl( std::string& strWebsiteUrl );

    void						setRandomConnectUrl( std::string& strWebsiteUrl );
    void						getRandomConnectUrl( std::string& strWebsiteUrl );

    void						setGroupHostUrl( std::string& strWebsiteUrl );
    void						getGroupHostUrl( std::string& strWebsiteUrl );

    void						setChatRoomHostUrl( std::string& strWebsiteUrl );
    void						getChatRoomHostUrl( std::string& strWebsiteUrl );

	void						setUseUpnpPortForward( bool bUseUpnpPortForward );
	bool						getUseUpnpPortForward( void );
	void						setUseNatPortForward( bool bUseNatPortForward );
	bool						getUseNatPortForward( void );

	void						setFirewallTestSetting( FirewallSettings::EFirewallTestType eFirewallTestType );
    FirewallSettings::EFirewallTestType           getFirewallTestSetting( void );

	void						setPreferredNetworkAdapterIp( const char * wirelessIpAddress );
	std::string					getPreferredNetworkAdapterIp( void );

	//=========================================================================
	//=== file share settings ===//
	void						setCompletedDnldsDir( std::string& strCompletedDir );
	void						getCompletedDnldsDir( std::string& strCompletedDir );
	void						setIncompleteDnldsDir( std::string& strIncompleteDir );
	void						getIncompleteDnldsDir( std::string& strIncompleteDir );
	void						setDnldsBandwidth( uint32_t& u32Bandwidth );
	void						getDnldsBandwidth( uint32_t& u32Bandwidth );
	void						setMaxDownloadingFiles( uint32_t& u32MaxDownloadingFiles );
	void						getMaxDownloadingFiles( uint32_t& u32MaxDownloadingFiles );

	void						setSharedFilesDirs( std::vector<std::string>& strSharedDirs );
	void						getSharedFilesDirs( std::vector<std::string>& strSharedDirs );
	void						setUpldsBandwidth( uint32_t& u32Bandwidth );
	void						getUpldsBandwidth( uint32_t& u32Bandwidth );
	void						setMaxUploadingFiles( uint32_t& u32MaxUploadingFiles );
	void						getMaxUploadingFiles( uint32_t& u32MaxUploadingFiles );

	void						setMaxRelaysInUse( uint32_t userRelays, uint32_t systemRelays );
	void						getMaxRelaysInUse( uint32_t& userRelays, uint32_t& systemRelays );

	void						setAllowUserLocation( bool enable )	;
	bool						getAllowUserLocation( void );
	void						setAllowMulticastBroadcast( bool enable );
	bool						getAllowMulticastBroadcast( void );

	void						setLastFirewallPort( uint16_t u16IpPort );
	uint16_t					getLastFirewallPort( void );

private:
	bool						m_Initialized;
	std::string					m_CachedNetworkName; // network is used so often we keep a cached copy
    VxMutex                     m_SettingsDbMutex; // protection from multiple thread access
};
