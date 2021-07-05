//==========================================================================
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

#include <config_gotvcore.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListDb.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IGoTv.h>

#include <CoreLib/sqlite3.h>
#include <CoreLib/VxMacros.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>


//============================================================================
//! thread function to load all nodes in big list
static void * BigListLoadThreadFunction( void * pvParam )
{
    RCODE rc = 0;
    VxThread * poThread = (VxThread *)pvParam;
    poThread->setIsThreadRunning( true );
    BigListMgr * poMgr = (BigListMgr *)poThread->getThreadUserParam();
    if( poMgr )
    {
        // load all lists urls from database
        rc = poMgr->dbRestoreAll( poMgr->getNetworkKey().c_str() );
        if( rc )
        {
            LogMsg( LOG_ERROR, "BigListLoadThreadFunction: Restore Error %d", rc );
        }

        GetPtoPEngine().onBigListLoadComplete( rc );
    }

    poThread->threadAboutToExit();
    return nullptr;
}


//============================================================================
BigListDb::BigListDb( BigListMgr& bigListMgr )
: DbBase( "BigListDb" )
, BigList()
, m_BigListMgr( bigListMgr )
, m_NetworkName( "" )
, m_BigListDbInitialized( false )
{
}

//============================================================================
//! create tables in the database
RCODE BigListDb::onCreateTables( int iDbVersion )
{
	RCODE rc = sqlExec( "CREATE TABLE BigList ( online_id TEXT PRIMARY KEY, NetworkName TEXT, ConnectTime BIGINT, Object BLOB)" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
//! create tables in the database
RCODE BigListDb::onDeleteTables( int oldVersion )
{
	RCODE rc = sqlExec( (char *)"DROP TABLE BigList" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
//! startup
RCODE BigListDb::bigListDbStartup(  const char * pDbFileName )
{
	if( m_BigListDbInitialized )
	{
		bigListDbShutdown();
	}

	RCODE rc = dbStartup(BIG_LIST_INFO_VERSION, pDbFileName );
#ifdef DEBUG_BIGLIST_DB
    if( 0 == rc )
	{
		LogMsg( LOG_INFO, "bigListDbStartup  before Restore All\n");
	}

	LogMsg( LOG_INFO, "bigListDbStartup  result %d\n", rc );
#endif // DEBUG_BIGLIST_DB
    m_BigListLoadThread.startThread( (VX_THREAD_FUNCTION_T)BigListLoadThreadFunction, &m_BigListMgr );

	m_BigListDbInitialized = true;
	return rc;
}

//============================================================================
//! shutdown
RCODE BigListDb::bigListDbShutdown( void )
{
	RCODE rc = 0;
	if( m_BigListDbInitialized )
	{
		m_BigListDbInitialized = false;
		m_BigListLoadThread.killThread();

		removeAllInfos();
		rc = dbShutdown();
	}

	return rc;
}

//============================================================================
//! restore all of given network to lists from database
RCODE BigListDb::dbRestoreAll( const char * networkName )
{
	int iRestoredCount = 0;
	std::string strNetworkName = networkName;
	if( getNetworkKey() != networkName )
	{
#ifdef DEBUG_BIGLIST_DB
        LogMsg( LOG_INFO, "BigListDb::dbRestoreAll changing networks %s to %s\n", getNetworkKey().c_str(), networkName );
#endif // DEBUG_BIGLIST_DB
		m_NetworkName = strNetworkName;
	}

	removeAllInfos();
	DbCursor * cursor = startQuery( "SELECT Object, ConnectTime FROM BigList WHERE NetworkName=?", networkName );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			int blobLen = 0;
			void * pTempBlob = cursor->getBlob( 0, &blobLen );
			int64_t sessionTimeMs = cursor->getS64( 1 );
			if (pTempBlob)
			{
				BigListInfo * poInfo = new BigListInfo();
				if( 0 == restoreBigListInfoFromBlob( (uint8_t *)pTempBlob, blobLen, poInfo, sessionTimeMs ) )
				{
					// clear temporary flags
					poInfo->m_u32BigListTempFlags = 0;
					poInfo->setIsNearby(false);
					poInfo->setIsOnline(false);
					poInfo->setIsInDatabase( true );
					poInfo->setLastSessionTimeMs( sessionTimeMs );

					bigInsertInfo( poInfo->m_DirectConnectId, poInfo );
					iRestoredCount++;
					GetPtoPEngine().onBigListInfoRestored( poInfo );
				}
			}
		}

		cursor->close();
		//LogMsg( LOG_DEBUG, "restoreBigListInfoFromBlob: restored %d\n", iRestoredCount );
	}

	return 0;
}

//============================================================================
RCODE BigListDb::dbUpdateSessionTime( VxGUID& onlineId, int64_t lastSessionTime, const char * networkName )
{
	char SQL_Statement[1024];
	//char *SQL_Error;
	int retval;
	sqlite3_stmt *pStatement;   //pointer to prepared statement
	RCODE rc = dbOpen();
	if( rc )
	{
		vx_assert( false );
		return rc;
	}

	std::string strHexOnlineId = onlineId.toHexString();
	sprintf(SQL_Statement, "UPDATE BigList SET ConnectTime=%lld WHERE online_id='%s'", lastSessionTime, strHexOnlineId.c_str() );
	retval = sqlite3_prepare( m_Db, SQL_Statement, (int)strlen(SQL_Statement), &pStatement, NULL );
	if( !( SQLITE_OK == retval ) )
	{
		LogMsg( LOG_ERROR, "BigListDb::dbUpdateSessionTime:sqlite3_prepare:%s\n", sqlite3_errmsg( m_Db ) );
		dbClose();
		return -1;
	}

	retval = sqlite3_step( pStatement );
	if( SQLITE_ERROR == retval )
	{
		LogMsg( LOG_ERROR, "BigListDb::dbUpdateSessionTime:sqlite3_step:%s\n", sqlite3_errmsg( m_Db ) );
		sqlite3_finalize( pStatement );
		dbClose();
		return -1;
	}

	sqlite3_finalize( pStatement );
	dbClose();
	return 0;
}

//============================================================================
//! if not in db insert BigListInfo else update database
RCODE BigListDb::dbUpdateBigListInfo( BigListInfo * poInfo, const char * networkName )
{
	RCODE rc = 0;
	if( poInfo->isInDatabase() )
	{
		rc = dbUpdateBigListInfoInDb( poInfo, networkName );
	}
	else 
	{
		rc = dbInsertBigListInfoIntoDb( poInfo, networkName );
	}

	return rc;
}

//============================================================================
//! remove friend by id
RCODE BigListDb::dbRemoveBigListInfo( VxGUID& onlineId )
{
	char SQL_Statement[1024];
    //char *SQL_Error;
	int retval;
	sqlite3_stmt *pStatement;   //pointer to prepared statement
	RCODE rc = dbOpen();
	if( rc )
	{
		vx_assert( false );
		return rc;
	}

	std::string strHexOnlineId = onlineId.toHexString();
	// make statement
	sprintf(SQL_Statement, "DELETE FROM BigList WHERE online_id='%s'", 
		strHexOnlineId.c_str() );
    //int iLen = (int)strlen( SQL_Statement );
    //vx_assert( iLen > 0 && iLen < sizeof( SQL_Statement ) );
	retval = sqlite3_prepare(m_Db,SQL_Statement,(int)strlen(SQL_Statement),&pStatement, NULL);
	if (!(SQLITE_OK == retval))
	{
		LogMsg( LOG_ERROR, "BigListDb::removeFriendFromDb:sqlite3_prepare:%s\n",sqlite3_errmsg(m_Db));
		dbClose();
		vx_assert( false );
		return -1;
	}

	retval = sqlite3_step(pStatement);
	if (SQLITE_ERROR == retval)
	{
		LogMsg( LOG_ERROR, "BigListDb::removeFriendFromDb:sqlite3_step:%s\n",sqlite3_errmsg(m_Db));
		sqlite3_finalize(pStatement);
		dbClose();
		vx_assert( false );
		return -1;
	}

	sqlite3_finalize(pStatement);
	dbClose();
	return 0;
}

//============================================================================
//! insert big list info node into database
RCODE BigListDb::dbInsertBigListInfoIntoDb( BigListInfo * poInfo, const char * networkName )
{
	// there is a possibility the isInDatabase flag did not get set so remove first
	dbRemoveBigListInfo( poInfo->getMyOnlineId() );
	poInfo->setIsInDatabase( true );

	int				    retval;
	sqlite3_stmt *	    pStatement;   //pointer to prepared statement
	uint8_t *			pu8Blob			= 0;
	int				    iBlobLen		= 0;
	int64_t				s64LastContactMs;
	// make big list info into blob
	RCODE rc = saveBigListInfoIntoBlob( poInfo, &pu8Blob, &iBlobLen );
	if( rc )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:Make Blob Error:%d\n",rc);
		vx_assert( false );
		poInfo->setIsInDatabase( false );
		return -1;
	}

	vx_assert( pu8Blob );
	vx_assert( iBlobLen );
	std::string strOnlineIdHex = poInfo->getMyOnlineId().toHexString();
    const char * SQL_Statement = "INSERT INTO BigList (online_id,NetworkName,ConnectTime,Object) VALUES (?,?,?,?)";
	rc = dbOpen();
	if( rc )
	{
		vx_assert( false );
		poInfo->setIsInDatabase( false );
		return rc;
	}

	// make statement
    retval = sqlite3_prepare( m_Db, SQL_Statement, -1, &pStatement, NULL);
    if( !(SQLITE_OK == retval) )
    {
        LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_prepare:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
    }

	if( SQLITE_OK != sqlite3_bind_text( pStatement, 1, strOnlineIdHex.c_str(), -1, SQLITE_TRANSIENT ) )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	if( SQLITE_OK != sqlite3_bind_text( pStatement, 2, networkName, -1, SQLITE_TRANSIENT ) )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	s64LastContactMs = poInfo->getLastSessionTimeMs();
	if( SQLITE_OK != sqlite3_bind_int64( pStatement, 3, s64LastContactMs ) ) 
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	if( SQLITE_OK != sqlite3_bind_blob( pStatement, 4, pu8Blob, iBlobLen, SQLITE_TRANSIENT ) )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	if( SQLITE_DONE != sqlite3_step( pStatement ) )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbInsertBigListInfoIntoDb:sqlite3_step:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	sqlite3_finalize( pStatement );
	dbClose();

	delete pu8Blob;
	return 0;

error_exit:
	poInfo->setIsInDatabase( false );
	dbClose();
	delete pu8Blob;
	return -1;
}

//============================================================================
//! update big list info node in database
RCODE BigListDb::dbUpdateBigListInfoInDb( BigListInfo * poInfo, const char * networkName )
{
	int64_t s64LastContact;
	uint8_t * pu8Blob = 0;
	int iBlobLen = 0;
	poInfo->setIsInDatabase( true );
	// make big list info into blob
	RCODE rc = saveBigListInfoIntoBlob( poInfo, &pu8Blob, &iBlobLen );
	if( rc )
	{
		LogMsg( LOG_ERROR,"BigListDb::InsertBlob:Make Blob Error:%d\n",rc);
		vx_assert( false );
		return -1;
	}
	
	vx_assert( pu8Blob );
	vx_assert( iBlobLen );

	char SQL_Statement[4096];
    //char *SQL_Error;
	int retval;
	sqlite3_stmt *pStatement;   //pointer to prepared statement
	rc = dbOpen();
	if( rc )
	{
		vx_assert( false );
		return rc;
	}

	// make statement
	std::string onlineIdHex = poInfo->getMyOnlineId().toHexString();
	sprintf(SQL_Statement, "UPDATE BigList SET ConnectTime=?,Object=? WHERE online_id='%s' AND NetworkName='%s'", 
		onlineIdHex.c_str(), 
		networkName );
	retval = sqlite3_prepare( m_Db, SQL_Statement,(int)strlen(SQL_Statement),&pStatement, NULL);
	if (!(SQLITE_OK == retval))
	{
		LogMsg( LOG_ERROR,"BigListDb::dbUpdateBigListInfoInDb:sqlite3_prepare:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	s64LastContact = poInfo->getLastSessionTimeMs();
	if( SQLITE_OK != sqlite3_bind_int64( pStatement, 1, s64LastContact ) ) 
	{
		LogMsg( LOG_ERROR,"BigListDb::dbUpdateBigListInfoInDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	if( SQLITE_OK != sqlite3_bind_blob( pStatement, 2, pu8Blob, iBlobLen, SQLITE_TRANSIENT) )
	{
		LogMsg( LOG_ERROR,"BigListDb::dbUpdateBigListInfoInDb:sqlite3_bind:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}

	if (SQLITE_DONE != sqlite3_step( pStatement ))
	{
		LogMsg( LOG_ERROR,"BigListDb::dbUpdateBigListInfoInDb:sqlite3_step:%s\n", sqlite3_errmsg( m_Db ) );
		goto error_exit;
	}
	
	sqlite3_finalize( pStatement );

	dbClose();
	delete pu8Blob;
	return 0;

error_exit:
	poInfo->setIsInDatabase( false );
	dbClose();
	delete pu8Blob;
	return -1;
}
//============================================================================
//! restore big list info from blob
RCODE BigListDb::restoreBigListInfoFromBlob( uint8_t * pu8Temp, int iDataLen, BigListInfo * poInfo, uint64_t lastSessionTime )
{
	int i;
	int iCnt;
	VxPktHdr * pktHdr;
	RCODE rc = 0;
	uint16_t u16PktLen = 0;
	// NOTE: don't bother with encryption. maybe later will use user login key for encryption and separate Info databases
	//// encrypt the blob
	////NOTE: the data length must be a multiple of the key length
	//rc = VxSymDecrypt(	&g_oGlobals.m_DefaultAdminKey, // Sym key
	//					(char *)pu8Temp,		// buffer to encrypt
	//					iDataLen,		// data length ( must be multiple of key length )
	//					NULL );			// if null then encrypted data put in pInBuf

	// read all we can with memcpy
    if( iDataLen < (int)sizeof( BigListInfoBase ) )
	{
		LogMsg( LOG_ERROR, "restoreBigListInfoFromBlob: invalid BigListInfoBase length\n");
		rc = -1;
	}
	else
	{
		memcpy( ((BigListInfoBase *) poInfo), pu8Temp, sizeof( BigListInfoBase ));
		pu8Temp += sizeof( BigListInfoBase );
        iDataLen -= (int)sizeof( BigListInfoBase );

		//restore incoming packet que
		//next 2 bytes is number of input que packets
		iCnt = *((uint32_t *)pu8Temp );
		if( (iCnt < 0) || 
			(iCnt > 1000) || 
			((iCnt * 16) > iDataLen ) )
		{
			LogMsg( LOG_ERROR, "restoreBigListInfoFromBlob: invalid incoming packet que length\n");
			rc = -2;
		}
		else
		{
			poInfo->m_aoInQue.resize( iCnt );
			pu8Temp += 4;
			//restore input que
			for( i = 0; i < iCnt; i++ )
			{
				u16PktLen = *((uint16_t *)pu8Temp);
				pktHdr = (VxPktHdr *)new char[ u16PktLen ];
				memcpy( pktHdr, pu8Temp, u16PktLen );
				poInfo->m_aoInQue[i] =  pktHdr ;
				pu8Temp += u16PktLen;
				iDataLen -= iDataLen;
				if( iDataLen < 0 )
				{
					LogMsg( LOG_ERROR, "restoreBigListInfoFromBlob: invalid incoming packet length\n");
					rc = -3;
					break;
				}
			}

			if( 0 == rc )
			{
				//restore outgoing packet que
				//next 2 bytes is number of output que packets
				iCnt = *((uint32_t *)pu8Temp );
				if( (iCnt < 0) || 
					(iCnt > 1000) || 
					((iCnt * 16) > iDataLen ) )
				{
					LogMsg( LOG_ERROR, "restoreBigListInfoFromBlob: invalid outgoing packet que length\n");
					rc = -4;
				}
				else
				{
					poInfo->m_aoOutQue.resize( iCnt );
					pu8Temp += 4;
					//restore Output que
					for( i = 0; i < iCnt; i++ )
					{
						u16PktLen = *((uint16_t *)pu8Temp);
						pktHdr = (VxPktHdr *)new char[ u16PktLen ];
						memcpy( pktHdr, pu8Temp, u16PktLen );
						poInfo->m_aoOutQue[i] =  pktHdr ;
						pu8Temp += u16PktLen;
						iDataLen -= iDataLen;
						if( iDataLen < 0 )
						{
							LogMsg( LOG_ERROR, "restoreBigListInfoFromBlob: invalid outgoing packet length\n");
							rc = -5;
							break;
						}
					}
				}
			}
		}
	}

	return rc;
}

//============================================================================
//! make big list info into blob
RCODE BigListDb::saveBigListInfoIntoBlob( BigListInfo * poInfo, uint8_t * * ppu8RetBlob, int * piRetBlobLen )
{
	VxPktHdr * pktHdr;

    //uint32_t u32Mask = 0xffffffff; //FLAG_USE_TRACKER_SERVICE | FLAG_IS_PERM_CONTACT;
	uint8_t * pu8Data;
	int i;
	int iCnt;
	int iLen = poInfo->CalcStoredLen();

	//NOTE: header size was already added to total in CalcStoredLen

	//round total length to 16 byte boundary for crypto
	uint32_t u32BlobLen = ROUND_TO_16BYTE_BOUNDRY( iLen );
	pu8Data = new unsigned char[ u32BlobLen ];
	uint8_t * pu8Temp = (unsigned char *)pu8Data;

	memcpy( pu8Temp, poInfo, sizeof( BigListInfoBase ) );
	pu8Temp += sizeof( BigListInfoBase );

	//store incoming packet que
	iCnt = (int)poInfo->m_aoInQue.size();
	//next 2 bytes is number of input que packets
	*((uint32_t *)pu8Temp ) = (uint32_t)iCnt;
	pu8Temp += 4;
	//store input que
	for( i = 0; i < iCnt; i++ )
	{
		pktHdr = (VxPktHdr *)poInfo->m_aoInQue[ i ];
		memcpy( pu8Temp, pktHdr, pktHdr->getPktLength() );
		pu8Temp += pktHdr->getPktLength();
	}

	//store outgoing packet que
	iCnt = (int)poInfo->m_aoOutQue.size();
	//next 2 bytes is number of output que packets
	*((uint32_t *)pu8Temp ) = (uint32_t)iCnt;
	pu8Temp += 4;
	//store output que
	for( i = 0; i < iCnt; i++ )
	{
		pktHdr = (VxPktHdr *)poInfo->m_aoOutQue[ i ];
		memcpy( pu8Temp, pktHdr, pktHdr->getPktLength() );
		pu8Temp += pktHdr->getPktLength();
	}
	
	uint32_t u32RealLen = (uint32_t)ROUND_TO_16BYTE_BOUNDRY( (pu8Temp - pu8Data ) );
	vx_assert(  u32RealLen <= u32BlobLen );
	* ppu8RetBlob = pu8Data;
	* piRetBlobLen = u32BlobLen;
	return 0;
}

