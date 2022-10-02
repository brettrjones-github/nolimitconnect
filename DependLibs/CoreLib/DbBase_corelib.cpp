//============================================================================
// Copyright (C) 2003 Brett R. Jones
//
// Issued to public domain 2013
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "DbBase.h"
#include "VxFileUtil.h"
#include "VxDebug.h"

#include "sqlite3.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace
{
	enum ESqlExecStep
	{
		eSqlExecStepDbOpen				= 0,
		eSqlExecStepPrepStatement		= 1,
		eSqlExecStepGetFirstParam		= 2,
		eSqlExecStepBindParams			= 3,
		eSqlExecStepFinalize			= 4,
		eMaxExecSqlStep					= 5,
	};

	VxMutex			g_DbBaseStartupMutex;
}


//============================================================================
DbBindParam::DbBindParam( void )
: m_EDbBindType( eDbBindTypeNone )
, m_AsText( "" )
, m_AsInt( 0 )
, m_AsS64( 0 )
, m_AsBlob( 0 )
, m_BlobLen( 0 )
{
}

//============================================================================
DbBindParam::DbBindParam( const char* text )
: m_EDbBindType( eDbBindTypeText )
, m_AsInt( 0 )
, m_AsS64( 0 )
, m_AsBlob( 0 )
, m_BlobLen( 0 )
{
	m_AsText = ( 0 == text ) ? "" : text;
}

//============================================================================
DbBindParam::DbBindParam( int asInt )
: m_EDbBindType( eDbBindTypeInt )
, m_AsText( "" )
, m_AsInt( asInt )
, m_AsS64( 0 )
, m_AsBlob( 0 )
, m_BlobLen( 0 )
{
}

//============================================================================
DbBindParam::DbBindParam( int64_t asS64 )
: m_EDbBindType( eDbBindTypeS64 )
, m_AsText( "" )
, m_AsInt( 0 )
, m_AsS64( asS64 )
, m_AsBlob( 0 )
, m_BlobLen( 0 )
{
}

//============================================================================
DbBindParam::DbBindParam( void * blob, int len )
: m_EDbBindType( eDbBindTypeBlob )
, m_AsText( "" )
, m_AsInt( 0 )
, m_AsS64( 0 )
, m_AsBlob( blob )
, m_BlobLen( len )
{
}

//============================================================================
EDbBindType DbBindParam::getType( void )
{
	return( m_EDbBindType );
}

//============================================================================
const char* DbBindParam::getText( void )
{
	vx_assert( eDbBindTypeText == m_EDbBindType );
	return( m_AsText.c_str() );
}

//============================================================================
unsigned short DbBindParam::getTextLen( void )
{
	vx_assert( eDbBindTypeText == m_EDbBindType );
	return (unsigned short)( m_AsText.length() );
}

//============================================================================
int DbBindParam::getInt( void )
{
	vx_assert( eDbBindTypeInt == m_EDbBindType );
	return( m_AsInt );
}

//============================================================================
int64_t DbBindParam::getS64( void )
{
	vx_assert( eDbBindTypeS64 == m_EDbBindType );
	return( m_AsS64 );
}

//============================================================================
void * DbBindParam::getBlob( int& len )
{
	vx_assert( eDbBindTypeBlob == m_EDbBindType );
	len = m_BlobLen;
	return m_AsBlob;
}

//============================================================================
DbBindList::DbBindList( void )
{
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
DbBindList::DbBindList( const char* strVal )
{
	add( strVal );
}

//============================================================================
DbBindList::DbBindList( void * blob, int blobLen )
{
	add( blob, blobLen );
}

//============================================================================
DbBindList::DbBindList( int intVal )
{
	DbBindParam* pBindParam;

	pBindParam = new DbBindParam( intVal );
	vx_assert( ( DbBindParam* )0 != pBindParam );
	m_ParamList.push_back( pBindParam );
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
DbBindList::DbBindList( int64_t s64Val )
{
	DbBindParam* pBindParam;

	pBindParam = new DbBindParam( s64Val );
	vx_assert( ( DbBindParam* )0 != pBindParam );
	m_ParamList.push_back( pBindParam );
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
DbBindList::~DbBindList( void )
{
	DbBindParam* pBindParam;

	while( ! m_ParamList.empty() )
	{
		pBindParam = m_ParamList.back();
		vx_assert( ( DbBindParam* )0 != pBindParam );
		m_ParamList.pop_back();
		delete pBindParam;
	}

}

//============================================================================
void DbBindList::add( const char* strVal )
{
	DbBindParam* pBindParam;

	pBindParam = new DbBindParam( strVal );
	vx_assert( ( DbBindParam* )0 != pBindParam );
	m_ParamList.push_back( pBindParam );
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
void DbBindList::add( int64_t s64Val )
{
	DbBindParam* pBindParam;

	pBindParam = new DbBindParam( s64Val );
	vx_assert( ( DbBindParam* )0 != pBindParam );
	m_ParamList.push_back( pBindParam );
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
void DbBindList::add( void * blob, int blobLen )
{
	DbBindParam* pBindParam;

	pBindParam = new DbBindParam( blob, blobLen );
	vx_assert( ( DbBindParam* )0 != pBindParam );
	m_ParamList.push_back( pBindParam );
	m_CurrParamIter = m_ParamList.begin();
}

//============================================================================
bool DbBindList::getFirst( DbBindParam** ppBindParam )
{
	bool retVal = false;

	vx_assert( ( DbBindParam** )0 != ppBindParam );
	m_CurrParamIter = m_ParamList.begin();

	if( m_ParamList.end() != m_CurrParamIter )
	{
		*ppBindParam = *m_CurrParamIter;
		vx_assert( ( DbBindParam* )0 != *ppBindParam );
		retVal = true;
	}

	return( retVal );
}

//============================================================================
bool DbBindList::getNext( DbBindParam** ppBindParam )
{
	bool retVal = false;

	vx_assert( ( DbBindParam** )0 != ppBindParam );
	vx_assert( m_ParamList.end() != m_CurrParamIter );
	m_CurrParamIter++;

	if( m_ParamList.end() == m_CurrParamIter )
	{
		m_CurrParamIter = m_ParamList.begin();
	}
	else
	{
		*ppBindParam = *m_CurrParamIter;
		vx_assert( ( DbBindParam* )0 != *ppBindParam );
		retVal = true;
	}

	return( retVal );
}

//============================================================================
DbBase::DbBase( const char* databaseName ) 
: m_strDatabaseName( databaseName )
, m_bDbInitialized(0)
, m_iDbVersion(0)
, m_Db(0)
{
}

//============================================================================
//! Initialize the database.. if doesn't exist then call DbCreateDatabase and DbCreateTables
RCODE DbBase::dbStartup( int iDbVersion, const char* pDbName )
{
	// in theory although sqlite is not thread safe you should be able to access multiple separate instances as same time by different threads.
	// Just to make sure we are only allowing one startup of database at a time
	g_DbBaseStartupMutex.lock();
	RCODE rc = 0;
	char tmpDir[ VX_MAX_PATH ];
	char* pTemp;

	vx_assert( pDbName );
	m_strDbFileName = pDbName;
	m_iDbVersion = iDbVersion;

    //LogMsg( LOG_INFO, "DbBase::dbStartup %s\n", pDbName );

	// create paths and database if necessary
	if( ! VxFileUtil::fileExists( m_strDbFileName.c_str() ) )
	{
		//create db 
		// make directory for db if needed
		strcpy( tmpDir, m_strDbFileName.c_str() );
		pTemp = strrchr( tmpDir, '/' );
		if( NULL != pTemp )
		{
			pTemp[0] = '\0';
			VxFileUtil::makeDirectory( tmpDir );
		}

        if( ! VxFileUtil::directoryExists( tmpDir ) )
        {
            LogMsg( LOG_DEBUG, "ERROR DbBase::dbStartup could not create directory for db %s", tmpDir);
        }

		rc = onCreateDatabase(iDbVersion);
		if( 0 != rc )
		{
			handleSqlError( 0,"DbBase:Cannot create database %s", m_strDbFileName.c_str() );
			g_DbBaseStartupMutex.unlock();
			return rc;
		}
	}
	else
	{
		// check the version .. upgrade if necessary
		int iOldDbVersion = readDatabaseVersion();
		if( iOldDbVersion != m_iDbVersion )
		{
			onUpgradeDatabase( iOldDbVersion, m_iDbVersion );
		}
	}

	m_bDbInitialized = true;
	g_DbBaseStartupMutex.unlock();
	return 0;
}

//============================================================================
RCODE DbBase::dbShutdown( void )
{
	return 0;
}

//============================================================================
//! create initial database
RCODE DbBase::onCreateDatabase( int iDbVersion )
{
	sqlite3 *db;
	RCODE rc = sqlite3_open( m_strDbFileName.c_str(), &db);
	if( SQLITE_OK != rc )
	{
		handleSqlError( LOG_ERROR, "DbCreateDatabase:ERROR %d Unable to create database %s", rc, m_strDbFileName.c_str() );
		sqlite3_close(db);
		return rc;
	}
	// close database.. Create tables will reopen
	sqlite3_close(db);
	writeDatabaseVersion( iDbVersion );

	// make tables in database
	rc = onCreateTables( iDbVersion );
	if( rc )
	{
		handleSqlError( LOG_ERROR, "CreateDataBase:ERROR %d Unable to create Table in db %s", rc, m_strDbFileName.c_str() );
		VxFileUtil::deleteFile( m_strDbFileName.c_str() );
	}
	// create version table
	return rc;
}

//============================================================================
//! upgrade db from old version to new version
RCODE DbBase::onUpgradeDatabase(int iOldDbVersion, int iNewDbVersion)
{
	RCODE rc = onDeleteTables( iOldDbVersion );
	if( 0 == rc )
	{
		rc = onCreateTables( iNewDbVersion );
	}
	if( rc )
	{
		handleSqlError( LOG_ERROR, "onUpgradeDatabase:ERROR %d in db %s", rc, m_strDbFileName.c_str() );
	}
	else
	{
		//default behavior should write new version to database table otherwise will upgrade each time initialized
		writeDatabaseVersion( iNewDbVersion );
	}

	return rc;
}

//============================================================================
//! open the database
RCODE DbBase::dbOpen( void )
{
	if( 0 == m_strDbFileName.size() )
	{
		LogMsg( LOG_ERROR, "ERROR Attempted DbBase::dbOpen %s without a file name", m_strDatabaseName.c_str() );
		vx_assert( m_strDbFileName.size() );
		return -1;
	}

	int retval = sqlite3_open( m_strDbFileName.c_str(), &m_Db );
	if (!(SQLITE_OK == retval))
	{
		handleSqlError( LOG_ERROR, "DbBase:Unable to open db %s", m_strDbFileName.c_str() );
		sqlite3_close(m_Db);
		m_Db = NULL;
		return -1;
	}

	vx_assert( m_Db );
	return 0;
}

//============================================================================
//! close the database
RCODE DbBase::dbClose( void )
{
	if( m_Db )
	{
		sqlite3_close(m_Db);
		m_Db = NULL;
		return 0;
	}

	handleSqlError( 0, "DbBase:Tried to close already closed db %s", m_strDbFileName.c_str() );
	return -1;
}

//============================================================================
//! read database version from version table 
int DbBase::readDatabaseVersion( void )
{
	std::string prepString = "SELECT * FROM DBBASE_VERSION";
	dbOpen();
	int iVersion =0;
	sqlite3_stmt * poSqlStatement = NULL;
	int iResult = sqlite3_prepare_v2( m_Db, prepString.c_str(), (int)( prepString.length() + 1 ), &poSqlStatement, NULL );
	if( SQLITE_OK != iResult ) 
	{
		handleSqlError( 0, "DbBase::readDatabaseVersion: error %s", sqlite3_errmsg(m_Db) );
		return 0;
	}

	if( SQLITE_ROW == sqlite3_step( poSqlStatement ) )
	{
		iVersion = sqlite3_column_int(poSqlStatement, 0 );
	}

	sqlite3_finalize(poSqlStatement);
	sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	dbClose();
	return iVersion;
}

//============================================================================
//! write database version to version table 
RCODE DbBase::writeDatabaseVersion( int iDbVersion )
{
	DbBindList bindList( iDbVersion );
	if( dbTableExists( "DBBASE_VERSION" ))
	{

		return sqlExec( "UPDATE DBBASE_VERSION SET db_version=?", bindList );
	}
	else
	{
		sqlExec("CREATE TABLE DBBASE_VERSION (db_version INTEGER)");

		return sqlExec( "INSERT INTO DBBASE_VERSION (db_version) VALUES (?)", bindList );
	}
}

//============================================================================
//! return true if table exists
bool DbBase::dbTableExists( const char* pTableName )
{
	dbOpen();
	bool bHasTable = false;
	sqlite3_stmt * poSqlStatement = NULL;
	char as8Buf[ 256 ];

	sprintf( as8Buf, "SELECT name FROM sqlite_master WHERE type='table' AND name=?");

	if( SQLITE_OK == sqlite3_prepare_v2( m_Db, as8Buf, (int)strlen( as8Buf )+1, &poSqlStatement, NULL ) ) 
	{
		int result;
		result = sqlite3_bind_text( poSqlStatement       , 
								 1                    , 
								 pTableName           , 
								 strlen( pTableName ) , 
								 SQLITE_TRANSIENT     );

		if( SQLITE_OK == result )
		{
			if( SQLITE_ROW == sqlite3_step( poSqlStatement ) )
			{
				bHasTable = true;
			}
		}
	}

	sqlite3_finalize( poSqlStatement );
	dbClose();

	return bHasTable;
}


//============================================================================
RCODE DbBase::sqlExec( const char*		SQL_Statement, 
                       DbBindList&		bindList )
{
	EDbBindType			bindType;
	const char*			bindTypeDesc		= "";
	char				stepDescBuf[ 128 ];
	const char*         stepDesc			= "";
	bool				executing;
	bool				needToFinalize		= false;
	bool				needToClose			= false;
	bool				finishedStepping	= false;
	bool				incrExStep			= true;
	bool				paramObtained		= false;
	const char*			srcStr				= "DbBase:sqlite3_exec: error";
	DbBindParam*		pBindParam;
	RCODE				expectedResult		= SQLITE_OK;
	RCODE				retVal				= SQLITE_OK;
    RCODE				result				= 0;
	sqlite3_stmt*		pSqlStatement		= ( sqlite3_stmt* )0;
	unsigned short		bindNum				= 1;
	unsigned short		exStep				= 0;

	do
	{
		executing = false;
		if( eSqlExecStepDbOpen != exStep && !m_Db )
		{
			LogMsg( LOG_ERROR, "DbBase::sqlExec ERROR null m_Db. Did you forget a mutex Lock?" );
			return SQLITE_ERROR;
		}

		switch( exStep )
		{
		case eSqlExecStepDbOpen:
			stepDesc = "OPEN";
			result = dbOpen();
			if( expectedResult == result )
			{
				needToClose = true;
			}
			break;

		case eSqlExecStepPrepStatement:
			stepDesc = "PREPARE";
			result = sqlite3_prepare_v2( m_Db                             , 
										SQL_Statement                      , 
										( int )strlen( SQL_Statement ) + 1 , 
										&pSqlStatement                     , 
										 NULL                               );
			if( SQLITE_OK == result )
			{
				needToFinalize = true;
			}
			break;

		case eSqlExecStepGetFirstParam:
			paramObtained = bindList.getFirst( &pBindParam );
			break;

		case eSqlExecStepBindParams:
			if( false == paramObtained )
			{
				incrExStep = true;
			}
			else
			{
				vx_assert( ( DbBindParam* )0 != pBindParam );
				bindType = pBindParam->getType();
				vx_assert( eDbBindTypeNone != bindType );

				switch( bindType )
				{
				case eDbBindTypeText:
					bindTypeDesc = "TEXT";
					result = sqlite3_bind_text(	pSqlStatement, 
												bindNum, 
												pBindParam->getText(), 
												pBindParam->getTextLen(), 
												SQLITE_TRANSIENT );
					break;

				case eDbBindTypeInt:
					bindTypeDesc = "INT";
					result = sqlite3_bind_int(	pSqlStatement, 
												bindNum, 
												pBindParam->getInt() );
					break;

				case eDbBindTypeS64:
					bindTypeDesc = "int64_t";
					result = sqlite3_bind_int64(	pSqlStatement, 
													bindNum, 
													pBindParam->getS64() );
					break;

				case eDbBindTypeBlob:
					{
						bindTypeDesc = "BLOB";
						int len = 0;
						void * pvBlob = pBindParam->getBlob( len );
						result = sqlite3_bind_blob( pSqlStatement, 
													bindNum, 
													pvBlob, 
													len,
													SQLITE_TRANSIENT );
					}

					break;

                default:
                    LogMsg( LOG_ERROR,"DbBase::sqlExec UNKNOWN bind type");
                    dbClose();
                    return -1;
				}

				sprintf( stepDescBuf, "BIND %s %d", bindTypeDesc, bindNum );
				stepDesc = stepDescBuf;
				
				bindNum += 1;
				paramObtained = bindList.getNext( &pBindParam );
				incrExStep = false;
				if( false == paramObtained )
				{
					incrExStep = true;
				}
			}

			break;

		case eSqlExecStepFinalize:
			stepDesc = "FINALIZE";
			needToFinalize = true;

			retVal = -1;
			if( SQLITE_DONE != sqlite3_step(pSqlStatement) )
			{
				LogMsg( LOG_ERROR, "DbBase::finalizeIniSetTransaction:ERROR %s while stepping", sqlite3_errmsg(m_Db) );
			}
			else
			{
				needToFinalize = false;
				if( SQLITE_OK == ( retVal = sqlite3_finalize(pSqlStatement) ) )
				{
					retVal = 0;
				}
				else
				{
					LogMsg( LOG_ERROR, "DbBase::finalizeIniSetTransaction:ERROR %s in finalize", sqlite3_errmsg(m_Db) );
				}
			}

			if( true == needToFinalize )
			{
				sqlite3_finalize( pSqlStatement );
				needToFinalize = false;
			}

			sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
			result = expectedResult = SQLITE_OK;

		default:
			finishedStepping = true;
		}

		if( expectedResult == result )
		{
			if( true != finishedStepping )
			{
				if( true == incrExStep )
				{
					exStep += 1;
				}

				executing = true;
			}
		}
		else
		{
			if( SQLITE_OK == expectedResult )
			{
				retVal = result;
			}
			else
			{
				retVal = -1;
			}

			LogMsg( LOG_ERROR, "%s %s %s", 
			        srcStr, 
			        stepDesc, 
			        sqlite3_errmsg( m_Db ) );
		}

	} while( true == executing );

	if( true == needToFinalize )
	{
		sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	}

	if( true == needToClose )
	{
		result = dbClose();
	}

	if ( SQLITE_OK == retVal )
	{
		retVal = result;
	}

	return( retVal );
}

//============================================================================
RCODE DbBase::sqlExec( std::string& statement )
{
	return sqlExec( statement.c_str() );
}

//============================================================================
RCODE DbBase::sqlExec( const char* SQL_Statement )
{
	char *SQL_Error;
	int retval;
	RCODE rc = dbOpen();
	if( 0 == rc )
	{
		retval = sqlite3_exec( m_Db, SQL_Statement, NULL, NULL, &SQL_Error );
		if (!(SQLITE_OK == retval))
		{
			handleSqlError( 0, "DbBase:sqlite3_exec: error %s executing %s", SQL_Error, SQL_Statement );
		
			sqlite3_free(SQL_Error);
			dbClose();
			return -1;
		}

		rc = dbClose();
	}

	return rc;
}

//============================================================================
void DbBase::handleSqlError( RCODE rc, const char* errMsg, ... )
{
	char szBuffer[1024];
	va_list arg_ptr;
	va_start(arg_ptr, errMsg);
	vsprintf(szBuffer, errMsg, arg_ptr);
	va_end(arg_ptr);
	LogMsg( LOG_ERROR, szBuffer );
	onSqlError( rc, szBuffer );
}

//============================================================================
//! start query and use DbCursor to access column's data.. be sure to call DbCursor.close() when done
DbCursor * DbBase::startQuery( const char* pSqlString )
{
	vx_assert( pSqlString );
	RCODE rc = dbOpen();
	if( 0 == rc )
	{
		sqlite3_stmt * poSqlStatement = NULL;
		int iResult = sqlite3_prepare_v2( m_Db, pSqlString, (int)strlen( pSqlString ), &poSqlStatement, NULL );
		if( SQLITE_OK != iResult ) 
		{
			const char* sqliteErrMsg = sqlite3_errmsg(m_Db);
			handleSqlError( LOG_ERROR, "DbBase::StartDataQuery: error %s statement %s", sqliteErrMsg, pSqlString );
			return NULL;
		}
		DbCursor * poCursor = new DbCursor();
		poCursor->m_DbBase = this;
		poCursor->m_Stmt = poSqlStatement;
		return poCursor;
	}

	return NULL;
}

//============================================================================
DbCursor* DbBase::startQuery( const char* pSqlString, const char* textParam )
{
	vx_assert( pSqlString );
	const char*   srcStr = "DbBase::StartDataQueryTxt: error";
	DbCursor*     retVal = NULL;
	int           iResult;

	sqlite3_stmt* poSqlStatement;

	if( 0 == dbOpen() )
	{
		poSqlStatement = NULL;
		iResult = sqlite3_prepare_v2(	m_Db, 
										pSqlString, 
										( int )strlen( pSqlString ), 
										&poSqlStatement, 
										NULL );

		if( SQLITE_OK == iResult ) 
		{
			iResult = sqlite3_bind_text(	poSqlStatement, 
											1, 
											textParam, 
                                            strlen( textParam ),
											SQLITE_TRANSIENT );

			if( SQLITE_OK == iResult )
			{
				retVal = new DbCursor();
				retVal->m_DbBase = this;
				retVal->m_Stmt = poSqlStatement;
			}
			else
			{
				LogMsg( LOG_ERROR, "ERROR: %s BIND TEXT %s", srcStr, sqlite3_errmsg( m_Db ) );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ERROR: %s %s statement %s", srcStr, sqlite3_errmsg( m_Db ), pSqlString );
		}
	}

	return retVal;
}

//============================================================================
DbCursor* DbBase::startQuery( const char* pSqlString, const char* textParam, int secondParam )
{
	vx_assert( pSqlString );
	const char* srcStr = "DbBase::StartDataQueryTxtInt: error";
	DbCursor* retVal = NULL;
	int           iResult;

	sqlite3_stmt* poSqlStatement;

	if( 0 == dbOpen() )
	{
		poSqlStatement = NULL;
		iResult = sqlite3_prepare_v2( m_Db,
			pSqlString,
			( int )strlen( pSqlString ),
			&poSqlStatement,
			NULL );

		if( SQLITE_OK == iResult )
		{
			iResult = sqlite3_bind_text( poSqlStatement,
				1,
				textParam,
				strlen( textParam ),
				SQLITE_TRANSIENT );

			if( SQLITE_OK == iResult )
			{
				iResult = sqlite3_bind_int( poSqlStatement,
					2,
					secondParam);

				if( SQLITE_OK == iResult )
				{
					retVal = new DbCursor();
					retVal->m_DbBase = this;
					retVal->m_Stmt = poSqlStatement;
				}
				else
				{
					LogMsg( LOG_ERROR, "ERROR: %s BIND INTEGER %s", srcStr, sqlite3_errmsg( m_Db ) );
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "ERROR: %s BIND TEXT %s", srcStr, sqlite3_errmsg( m_Db ) );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ERROR: %s %s statement %s", srcStr, sqlite3_errmsg( m_Db ), pSqlString );
		}
	}

	return retVal;
}

//============================================================================
DbCursor* DbBase::startQuery( const char* pSqlString, const char* textParam1, const char* textParam2, int thirdParam )
{
	vx_assert( pSqlString );
	const char* srcStr = "DbBase::StartDataQueryTxtInt: error";
	DbCursor* retVal = NULL;
	int           iResult;

	sqlite3_stmt* poSqlStatement;

	if( 0 == dbOpen() )
	{
		poSqlStatement = NULL;
		iResult = sqlite3_prepare_v2( m_Db,
			pSqlString,
			( int )strlen( pSqlString ),
			&poSqlStatement,
			NULL );

		if( SQLITE_OK == iResult )
		{
			iResult = sqlite3_bind_text( poSqlStatement,
				1,
				textParam1,
				strlen( textParam1 ),
				SQLITE_TRANSIENT );

			if( SQLITE_OK == iResult )
			{
				iResult = sqlite3_bind_text( poSqlStatement,
					1,
					textParam2,
					strlen( textParam2 ),
					SQLITE_TRANSIENT );

				if( SQLITE_OK == iResult )
				{
					iResult = sqlite3_bind_int( poSqlStatement,
						2,
						thirdParam );

					if( SQLITE_OK == iResult )
					{
						retVal = new DbCursor();
						retVal->m_DbBase = this;
						retVal->m_Stmt = poSqlStatement;
					}
					else
					{
						LogMsg( LOG_ERROR, "ERROR: %s BIND INTEGER %s", srcStr, sqlite3_errmsg( m_Db ) );
					}
				}
				else
				{
					LogMsg( LOG_ERROR, "ERROR: %s BIND TEXT2 %s", srcStr, sqlite3_errmsg( m_Db ) );
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "ERROR: %s BIND TEXT1 %s", srcStr, sqlite3_errmsg( m_Db ) );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ERROR: %s %s statement %s", srcStr, sqlite3_errmsg( m_Db ), pSqlString );
		}
	}

	return retVal;
}

//============================================================================
//! start query and use DbCursor to access column's data.. be sure to call DbCursor.close() when done
// TODO use bind list to avoid possible injection
DbCursor * DbBase::startQueryInsecure( const char* pSqlString, ... )
{
	char SQL_Statement[4095];

	va_list arg_ptr;
	va_start(arg_ptr, pSqlString);
	vsnprintf(SQL_Statement, 4095, pSqlString, arg_ptr);
	SQL_Statement[4094] = 0;
	va_end(arg_ptr);
	return startQuery( SQL_Statement );
}

//============================================================================
//! get the row id of the last inserted row
int64_t DbBase::getLastInsertId( void )
{
	if( m_Db )
	{
		return sqlite3_last_insert_rowid( m_Db );
	}

	return 0;
}

//============================================================================
DbCursor::DbCursor()
	: m_DbBase(0)
	, m_Stmt(0)
{
}

//============================================================================
//! close database and delete ourself
void DbCursor::close( void )
{
	int iResult = sqlite3_finalize( m_Stmt );

	if( SQLITE_OK != iResult ) 
	{
		LogMsg( LOG_ERROR, "DatabaseClass::CloseQuery: error %s", sqlite3_errmsg(m_DbBase->m_Db) );
	}

	vx_assert( ( DbBase* )0 != m_DbBase );
	m_DbBase->dbClose();
	delete this;
}

//============================================================================
bool DbCursor::getNextRow( void )
{
	int iResult = sqlite3_step( m_Stmt );
	switch( iResult ) 
	{
	case SQLITE_ROW:
		return true;
		break;
	case SQLITE_DONE:
		return false;
		break;
	default:
		LogMsg( LOG_ERROR, "DbBase::GetDbRow: error %s", sqlite3_errmsg(m_DbBase->m_Db) );
		break;
	}
	return false;
}

//============================================================================
uint8_t DbCursor::getByte( int iColumnIdx )
{
	return (uint8_t)sqlite3_column_int( m_Stmt, iColumnIdx );
}

//============================================================================
int32_t DbCursor::getS32( int iColumnIdx )
{
	return (int32_t)sqlite3_column_int( m_Stmt, iColumnIdx );
}

//============================================================================
int64_t DbCursor::getS64( int iColumnIdx )
{
	return sqlite3_column_int64( m_Stmt, iColumnIdx );
}

//============================================================================
float DbCursor::getF32( int iColumnIdx )
{
	return (float)sqlite3_column_double( m_Stmt, iColumnIdx );
}

//============================================================================
double DbCursor::getF64( int iColumnIdx )
{
	return (double)sqlite3_column_double( m_Stmt, iColumnIdx );
}

//============================================================================
const char* DbCursor::getString(int iColumnIdx )
{
	return (const char*)sqlite3_column_text( m_Stmt, iColumnIdx );
}

//============================================================================
//! return blob from column.. 
//! if( piMaxLen != null ) then return length of blob in piMaxLen 
void *	 DbCursor::getBlob( int iColumnIdx, int * piRetLen )
{
	if( NULL != piRetLen )
	{
		*piRetLen =  sqlite3_column_bytes( m_Stmt, iColumnIdx );
	}

	return (void *)sqlite3_column_blob( m_Stmt, iColumnIdx );
}


