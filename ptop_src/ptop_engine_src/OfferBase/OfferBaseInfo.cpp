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
// http://www.nolimitconnect.org
//============================================================================

#include "OfferBaseInfo.h"

#include <PktLib/PktBlobEntry.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxTimer.h>

//============================================================================
OfferBaseInfo::OfferBaseInfo( const OfferBaseInfo& rhs )
	: AssetInfo( rhs )
	, m_PluginType( rhs.m_PluginType )
	, m_OfferMgr( rhs.m_OfferMgr )
	, m_OfferId( rhs.m_OfferId )
	, m_OfferExpireTime( rhs.m_OfferExpireTime )
	, m_OfferMsg( rhs.m_OfferMsg )
	, m_OfferResponse( rhs.m_OfferResponse )
{
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( std::string fileName )
	: AssetInfo( VxFileNameToAssetType( fileName ), fileName )
{
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( std::string fileName, uint64_t assetLen, uint16_t assetType )
	: AssetInfo( VxFileNameToAssetType( fileName ), fileName, assetLen )
{
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( FileInfo& fileInfo )
	: AssetInfo( fileInfo )
{
}

//============================================================================
OfferBaseInfo& OfferBaseInfo::operator=( const OfferBaseInfo& rhs )
{
	if( this != &rhs )
	{
		*((AssetInfo*)this) = rhs;
		m_PluginType = rhs.m_PluginType;
		m_OfferMgr = rhs.m_OfferMgr;
		m_OfferId = rhs.m_OfferId;
		m_OfferExpireTime = rhs.m_OfferExpireTime;
		m_OfferMsg = rhs.m_OfferMsg;
		m_OfferResponse = rhs.m_OfferResponse;
	}

	return *this;
}

//============================================================================
bool OfferBaseInfo::addToBlob( PktBlobEntry& blob )
{
	blob.resetWrite();
	if( AssetInfo::addToBlob( blob ) )
	{
		bool result = blob.setValue( m_PluginType );
		result &= blob.setValue( m_OfferMgr );
		result &= blob.setValue( m_OfferId );
		result &= blob.setValue( m_OfferExpireTime );
		result &= blob.setValue( m_OfferMsg );
		result &= blob.setValue( m_OfferResponse );

		return result;
	}

	return false;
}

//============================================================================
bool OfferBaseInfo::extractFromBlob( PktBlobEntry& blob )
{
	blob.resetRead();
	if( AssetInfo::extractFromBlob( blob ) )
	{
		bool result = blob.getValue( m_PluginType );
		result &= blob.getValue( m_OfferMgr );
		result &= blob.getValue( m_OfferId );
		result &= blob.getValue( m_OfferExpireTime );
		result &= blob.getValue( m_OfferMsg );
		result &= blob.getValue( m_OfferResponse );

		return result;
	}

	return false;
}

//============================================================================
void OfferBaseInfo::fillOfferSend( EPluginType pluginType, VxNetIdent& netIdent )
{
	m_PluginType = pluginType;
	m_OfferResponse = eOfferResponseNotSet;
	m_OfferMgr = eOfferMgrHost;
	setOnlineId( netIdent.getMyOnlineId() );
	setCreatorId( netIdent.getMyOnlineId() );
	setHistoryId( netIdent.getMyOnlineId() );
	m_UniqueId.assureIsValidGUID();
	if( IsPluginSingleSession( pluginType ) )
	{
		setOfferId( netIdent.getMyOnlineId() );
	}
	else
	{
		m_OfferId.assureIsValidGUID();
	}	
}

//============================================================================
bool OfferBaseInfo::isExpiredOffer( void )
{
	return m_OfferExpireTime && GetHighResolutionTimeMs()  < m_OfferExpireTime;
}

//============================================================================
bool OfferBaseInfo::isValid( void )
{
	return ePluginTypeInvalid != m_PluginType && eOfferMgrNotSet != m_OfferMgr && m_UniqueId.isVxGUIDValid() && m_OfferId.isVxGUIDValid() && getOnlineId().isVxGUIDValid();
}

//============================================================================
bool OfferBaseInfo::isSessionMatch( OfferBaseInfo& rhs )
{
	return getOfferId() == rhs.getOfferId() && m_PluginType == rhs.getPluginType() && getOnlineId() == rhs.getOnlineId() && getOfferMgr() == rhs.getOfferMgr();
}