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

#include "RcScanMatchedConnection.h"

#include <GuiInterface/IScan.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

#include <vector>

class VxSktBase;
class P2PEngine;
class RcScanPic;
class RcScan;

class RcScanAction
{
public:
	RcScanAction( P2PEngine& engine, RcScan& oScan );

	void						setScanType( EScanType eScanType )			{ m_eScanType = eScanType; };
	EScanType					getScanType( void )							{ return m_eScanType; }

	void						fromGuiStartScan( EScanType eScanType );
	void						fromGuiNextScan( EScanType eScanType );
	void						fromGuiStopScan( EScanType eScanType );

	void						onOncePer30Seconds( void );
	void						onOncePerMinute( void );
	void						searchConnectionsTimedOut( void );


	// handle case where BigListInfo is about to be deleted
	void						onIdentDelete( VxNetIdent * netIdent );

	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );	
	//! called when new better connection from user
	void						replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	void						addMatchedConnection( VxNetIdent * netIdent, VxSktBase * sktBase );

	void						doSearchResultActions( void );

	virtual void				onScanResultProfilePic(	VxNetIdent *	netIdent, 
															VxSktBase *		sktBase, 
															uint8_t *			pu8JpgData, 
															uint32_t				u32JpgDataLen );

	virtual void				onScanResultError(	EScanType			eScanType,
														VxNetIdent *		netIdent, 
														VxSktBase *			sktBase, 
														uint32_t				errCode ); 
protected:
	void						startSearchActionThread( void );
	void						stopSearchActionThread( void );

	void						searchActionPeopleSearch( VxNetIdent * netIdent, VxSktBase * sktBase );
	void						searchActionMoodMsgSearch( VxNetIdent * netIdent, VxSktBase * sktBase );
	void						searchActionFileSearch( VxNetIdent * netIdent, VxSktBase * sktBase );

	bool						getNextActionConnection( VxNetIdent ** netIdent, VxSktBase ** sktBase );
	
	void						setShouldSendNext( bool next )				{ m_bNextScan = next; }
	bool						getShouldSendNext( void )					{ return m_bNextScan; }

	void						nextCamServerToGui();
	void						nextPicToGui( void );
	void						cleanupScanResources( void );
	RcScanMatchedConnection *	findMatchedConnection( VxNetIdent * netIdent );
	void						removeIdent( VxNetIdent * netIdent );

	//=== vars ===//
	VxThread					m_SearchActionThread;
	std::vector<RcScanMatchedConnection>	m_MatchedConnectionsList;

	P2PEngine&					m_Engine;
	RcScan&						m_Scan;

	VxSemaphore					m_SearchActionSemaphore;
	VxMutex						m_SearchActionMutex;

	bool						m_bNextScan;
	EScanType					m_eScanType;
	bool						m_SearchConnectionsTimedOut;

};
