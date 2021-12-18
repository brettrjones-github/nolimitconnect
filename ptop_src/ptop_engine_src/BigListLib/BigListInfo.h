#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <config_appcorelibs.h>
#include <PktLib/PktAnnounce.h>
#include <CoreLib/VxGlobals.h>

//Lock Start Inst 700
//Lock End Inst 718
#define BIG_LIST_INFO_VERSION	0x01	// version of BigListInfo structure
#define BIG_LIST_DB_VERSION		0x01	// version of BigList database

#define MAX_INQUE_PKTS			10	//maximum number of Pkts in inque
#define MAX_OUTQUE_PKTS			10	//maximum number of Pkts in OutQue

//=== announce flags ===//
#define FLAG_IS_TCP_ANNOUNCE			0x0001
#define FLAG_IS_BROADCAST_ANNOUNCE		0x0002
#define FLAG_IS_TOP_TEN					0x0004
#define FLAG_IS_TOP_TEN_CLIENT			0x0008
//#define FLAG_IS_ONLINE					0x0010 //set when user is online
#define FLAG_IS_ANN_LIST				0x0020
#define FLAG_IS_ANCHOR_LIST				0x0040	// anchor lists do not contain plugin info


#define FLAG_FILE_SERVER_SEARCH_DONE	0x8000000 //has been searched for file servers
#define FLAG_FILE_SEARCH_DONE			0x4000000 //has been searched for files

//! connection stats for friend contact
class RcConnectStats
{
public:

	RcConnectStats();

	//! return true if still is a chance we might connect
	bool						        isLikelyToConnect( void );


	uint64_t							getTimeFirstConnectAttempt( void );

	void						        setTimeLastConnectSuccess( uint64_t u32TimeSec );
	uint64_t							getTimeLastConnectSuccess( void );
	uint64_t							getTimeFirstConnectSuccess( void );

	void						        setConnectErrCnt( uint32_t u32ErrCnt );
	uint32_t							getConnectErrCnt( void );

	void						        setConnectSuccessCnt( uint32_t u32SuccessCnt );
	uint32_t							getConnectSuccessCnt( void );
	int							        getConnectSuccessPercent( void );


	//=== vars ===//
	uint64_t							m_u64TimeFirstConnectAttempt;		// time of first attempt to connect
	uint64_t							m_u64TimeFirstConnectSuccess;		// time of first successful connect
	uint64_t							m_u64TimeLastConnectSuccess;		// time of last successful attempt to connect
	uint32_t							m_u32ConnectErrCnt;					// error count of last failed attempts
	uint32_t							m_u32ConnectSuccessCnt;				// number of times successfully connected
	uint32_t							m_u32ConnectRes;					// reserved
};

// permanent flags
#define BIGLIST_FLAG_IS_MY_PROXY				0x00000001
#define BIGLIST_FLAG_IS_IN_DB					0x00000002

// temp flags
#define BIGLIST_TEMP_FLAG_IS_CONNECTED			0x00000001
#define BIGLIST_TEMP_FLAG_NEED_SAVED_T0_DB		0x00000002

//! base object of biglist info.. contains all that can be memcopied and stored
class BigListInfoBase : public PktAnnounce, public RcConnectStats 
{
public:

	BigListInfoBase()
		: m_u8BigListInfoVersion(BIG_LIST_INFO_VERSION)		// version of big list info in use when this was stored
		, m_u8BigListRes1(0)		
		, m_u8BigListRes2(0)			
		, m_u8BigListRes3(0)		
		, m_u32BigListPermFlags(0)	
		, m_u32BigListTempFlags(0)			
		, m_u64LastUdpAnnounce(0)
		, m_u32Res4(0)				// reserved for future use
		, m_u32Res5(0)				// reserved for future use
		, m_u32Res6(0)				// reserved for future use
	{
	}

	// perm flags
	bool						isMyRelay( void )						{ return (m_u32BigListPermFlags & BIGLIST_FLAG_IS_MY_PROXY)?1:0; }
	void						setIsMyRelay( bool bIsMyRelay )			{ setNeedSavedToDb(true); if( bIsMyRelay ) m_u32BigListPermFlags |= BIGLIST_FLAG_IS_MY_PROXY; else m_u32BigListPermFlags &= ~BIGLIST_FLAG_IS_MY_PROXY; }
	bool						isInDatabase( void )					{ return (m_u32BigListPermFlags & BIGLIST_FLAG_IS_IN_DB)?1:0; }
	void						setIsInDatabase( bool bIsInDb )			{ if( bIsInDb ) m_u32BigListPermFlags |= BIGLIST_FLAG_IS_IN_DB; else m_u32BigListPermFlags &= ~BIGLIST_FLAG_IS_IN_DB; }

	// temp flags
	bool						isConnected( void )						{ return (m_u32BigListTempFlags & BIGLIST_TEMP_FLAG_IS_CONNECTED)?1:0; }
	void						setIsConnected( bool bIsConnected )		{ if( bIsConnected ) m_u32BigListTempFlags |= BIGLIST_TEMP_FLAG_IS_CONNECTED; else m_u32BigListTempFlags &= ~BIGLIST_TEMP_FLAG_IS_CONNECTED; }
	bool						needSavedToDb( void )					{ return (m_u32BigListTempFlags & BIGLIST_TEMP_FLAG_NEED_SAVED_T0_DB)?1:0; }
	void						setNeedSavedToDb( bool bSaveDb )		{ if( bSaveDb ) m_u32BigListTempFlags |= BIGLIST_TEMP_FLAG_NEED_SAVED_T0_DB; else m_u32BigListTempFlags &= ~BIGLIST_TEMP_FLAG_NEED_SAVED_T0_DB; }
	
	void						setTimeLastConnectAttempt2( uint64_t u32TimeMs );
	void						contactWasAttempted( bool wasSuccessfull );

	//=== stored variables ===//
	uint8_t						m_u8BigListInfoVersion;	// version of big list info in use when this was stored
	uint8_t						m_u8BigListRes1;		
	uint8_t						m_u8BigListRes2;			
	uint8_t					    m_u8BigListRes3;				
	uint32_t					m_u32BigListPermFlags;		
	uint32_t					m_u32BigListTempFlags;
	uint64_t					m_u64LastUdpAnnounce;
	uint32_t					m_u32Res4;				// reserved for future use
	uint32_t					m_u32Res5;				// reserved for future use
	uint32_t					m_u32Res6;				// reserved for future use

};

class BigListInfo : public BigListInfoBase 
{
public:
	BigListInfo();
	~BigListInfo();

	bool						isSafeToDelete( void );		
	bool						RemoveIfTemp( int iListIdx );
	//! calc length when stored as blob
	uint32_t					CalcStoredLen( void );


	void						CopyTo( BigListInfo & gInfo );
	void						DescribeAuth( std::string & csRetAuth );
	BigListInfo *				makeCopy( void );

	void						AddInQueItem(  VxPktHdr * poPkt );
	VxPktHdr *					GetInQueItem( void );
	void						AddOutQueItem( VxPktHdr * poPkt );
	VxPktHdr *					GetOutQueItem( void );

	//=== vars ===//
	// stored arrays
	std::vector<VxPktHdr *>		m_aoInQue;
	std::vector<VxPktHdr *>		m_aoOutQue;
	// not stored vars
	uint64_t					m_u64TimeLastPingAttempt;

};

