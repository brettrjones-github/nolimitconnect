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
// http://www.nolimitconnect.org
//============================================================================
#include "config_corelib.h"
#include "VxSettings.h"
#include "sqlite3.h"
#include "VxParse.h"
#include <memory.h>
#include <string.h>
#include <stdio.h>


#define VXSETTINGS_DB_VERSION 0x01

//============================================================================
VxSettings::VxSettings( const char * settingDbName )
: DbBase( settingDbName )
{
}

//============================================================================
//! override onCreateTables to create our tables
RCODE VxSettings::onCreateTables( int iDbVersion )
{
	m_DbMutex.lock();
	RCODE rc = sqlExec( (char*)"CREATE TABLE BOOL (key TEXT, setting TEXT PRIMARY KEY, value TINYINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE INT (key TEXT, setting TEXT PRIMARY KEY, value INTEGER)" );
	rc |= sqlExec( (char*)"CREATE TABLE int8_t (key TEXT, setting TEXT PRIMARY KEY, value TINYINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE uint8_t (key TEXT, setting TEXT PRIMARY KEY, value TINYINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE int16_t (key TEXT, setting TEXT PRIMARY KEY, value SMALLINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE uint16_t (key TEXT, setting TEXT PRIMARY KEY, value SMALLINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE int32_t (key TEXT, setting TEXT PRIMARY KEY, value INTEGER)" );
	rc |= sqlExec( (char*)"CREATE TABLE uint32_t (key TEXT, setting TEXT PRIMARY KEY, value INTEGER)" );
	rc |= sqlExec( (char*)"CREATE TABLE int64_t (key TEXT, setting TEXT PRIMARY KEY, value BIGINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE uint64_t (key TEXT, setting TEXT PRIMARY KEY, value BIGINT)" );
	rc |= sqlExec( (char*)"CREATE TABLE float (key TEXT, setting TEXT PRIMARY KEY, value REAL)" );
	rc |= sqlExec( (char*)"CREATE TABLE double (key TEXT, setting TEXT PRIMARY KEY, value REAL)" );
	rc |= sqlExec( (char*)"CREATE TABLE string (key TEXT, setting TEXT PRIMARY KEY, value TEXT)" );
	rc |= sqlExec( (char*)"CREATE TABLE vector_strings (key TEXT, setting TEXT PRIMARY KEY, value TEXT)" );
	rc |= sqlExec( (char*)"CREATE TABLE blob (key TEXT, setting TEXT PRIMARY KEY, value BLOB)" );
	if( rc )
	{
		LogMsg( LOG_ERROR, "VxSettings::DbCreateTables:ERROR %d creating table\n" );
	}

	m_DbMutex.unlock();
	return rc;
}
//============================================================================
//! override onDeleteTables to delete our tables
RCODE VxSettings::onDeleteTables( int oldDbVersion )
{
	m_DbMutex.lock();
	RCODE rc = sqlExec( (char*)"DROP TABLE BOOL" );
	rc |= sqlExec( (char*)"DROP TABLE INT" );
	rc |= sqlExec( (char*)"DROP TABLE int8_t" );
	rc |= sqlExec( (char*)"DROP TABLE uint8_t" );
	rc |= sqlExec( (char*)"DROP TABLE int16_t" );
	rc |= sqlExec( (char*)"DROP TABLE uint16_t" );
	rc |= sqlExec( (char*)"DROP TABLE int32_t" );
	rc |= sqlExec( (char*)"DROP TABLE uint32_t" );
	rc |= sqlExec( (char*)"DROP TABLE int64_t" );
	rc |= sqlExec( (char*)"DROP TABLE uint64_t" );
	rc |= sqlExec( (char*)"DROP TABLE float" );
	rc |= sqlExec( (char*)"DROP TABLE double" );
	rc |= sqlExec( (char*)"DROP TABLE string" );
	rc |= sqlExec( (char*)"DROP TABLE vector_strings" );
	rc |= sqlExec( (char*)"DROP TABLE blob" );

	m_DbMutex.unlock();
	return rc;
}

//============================================================================
//! startup Settings.. if database doesn't exist then create it and call DbCreateTables
RCODE VxSettings::vxSettingsStartup( const char * pDbFileName )
{
	RCODE rc = dbStartup( VXSETTINGS_DB_VERSION, pDbFileName );
	if( rc )
	{
		LogMsg( LOG_ERROR, "VxSettings::vxSettingsStartup:ERROR %d", rc );
	}
	return rc;
}

//============================================================================
//! shutdown Settings
void VxSettings::vxSettingsShutdown( void )
{
	RCODE rc = dbShutdown();
	if( rc )
	{
		LogMsg( LOG_ERROR, "VxSettings::vxSettingsShutdown:ERROR %d", rc );
	}
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, bool& bValue )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "BOOL", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, (int)bValue ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, int8_t& s8Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "int8_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, s8Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, uint8_t& u8Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "uint8_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, u8Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, int16_t& s16Value )
{
	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "int16_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, s16Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, uint16_t& u16Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "uint16_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, u16Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, int32_t& s32Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "int32_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, s32Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, uint32_t& u32Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "uint32_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int( poSqlStatement, 1, u32Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, int64_t& s64Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "int64_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int64( poSqlStatement, 1, s64Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, uint64_t& u64Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "uint64_t", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_int64( poSqlStatement, 1, u64Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, float& f32Value )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "float", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_double( poSqlStatement, 1, f32Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, double& f64Value )
{
	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "double", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_double( poSqlStatement, 1, f64Value ) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}
}

//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, std::string& strValue )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "string", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_blob( poSqlStatement, 1, strValue.c_str(), (int)strValue.length() + 1, SQLITE_TRANSIENT) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, const char * pValue )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "string", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_blob( poSqlStatement, 1, pValue, (int)strlen(pValue) + 1, SQLITE_TRANSIENT) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! set and save value to database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, std::vector<std::string>& aoStrValues )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "vector_strings", pKey, pSettingName ) )
	{
		std::string strCommaDelimStrings;
		std::vector<std::string>::iterator iter;
		for(iter = aoStrValues.begin(); iter != aoStrValues.end(); ++iter )
		{
			strCommaDelimStrings += *iter;
			strCommaDelimStrings += ",";
		}

		bool bError = (SQLITE_OK != sqlite3_bind_blob( poSqlStatement, 1, strCommaDelimStrings.c_str(), (int)strCommaDelimStrings.length() + 1, SQLITE_TRANSIENT) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! save a object as blob into the database
void VxSettings::setIniValue( const char * pKey, const char * pSettingName, void * pvObject, int iObjectLen )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 ==  prepareIniSet( &poSqlStatement, "blob", pKey, pSettingName ) )
	{
		bool bError = (SQLITE_OK != sqlite3_bind_blob( poSqlStatement, 1, pvObject, iObjectLen, SQLITE_TRANSIENT) );
		finalizeIniSetTransaction( poSqlStatement, bError );
	}

	m_DbMutex.unlock();
}
//============================================================================
//=== set value functions ===//
//============================================================================

//============================================================================
//! get value from database.. return default if doesn't exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, bool& bValue, bool bDefault )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "BOOL", pKey, pSettingName ) )
	{
		bValue = (0 == sqlite3_column_int( poSqlStatement, 0 ) ) ? false : true;
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		bValue = bDefault;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, int8_t& s8Value, int8_t s8Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "int8_t", pKey, pSettingName ) )
	{
		s8Value = (int8_t)sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		s8Value = s8Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, uint8_t& u8Value, uint8_t u8Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "uint8_t", pKey, pSettingName ) )
	{
		u8Value = (uint8_t)sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		u8Value = u8Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, int16_t& s16Value, int16_t s16Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "int16_t", pKey, pSettingName ) )
	{
		s16Value = (int16_t)sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		s16Value = s16Default;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, uint16_t& u16Value, uint16_t u16Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "uint16_t", pKey, pSettingName ) )
	{
		u16Value = (uint16_t)sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		u16Value = u16Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, int32_t& s32Value, int32_t s32Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "int32_t", pKey, pSettingName ) )
	{
		s32Value = (int32_t)sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		s32Value = s32Default;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, uint32_t& u32Value, uint32_t u32Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "uint32_t", pKey, pSettingName ) )
	{
		u32Value = sqlite3_column_int( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		u32Value = u32Default;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, int64_t& s64Value, int64_t s64Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "int64_t", pKey, pSettingName ) )
	{
		s64Value = (int64_t)sqlite3_column_int64( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		s64Value = s64Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, uint64_t& u64Value, uint64_t u64Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "uint64_t", pKey, pSettingName ) )
	{
		u64Value = (uint64_t)sqlite3_column_int64( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		u64Value = u64Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, float& f32Value, float f32Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "float", pKey, pSettingName ) )
	{
		f32Value = (float)sqlite3_column_double( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		f32Value = f32Default;
	}

	m_DbMutex.unlock();
}
//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, double& f64Value, double f64Default )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	if( 0 < prepareIniQuery( &poSqlStatement, "double", pKey, pSettingName ) )
	{
		f64Value = (double)sqlite3_column_double( poSqlStatement, 0 );
		finalizeIniGetTransaction( poSqlStatement );
	}
	else
	{
		f64Value = f64Default;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, std::string& strValue, const char * pDefault )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	int  iBlobBytes;
	RCODE rc = -1;
	if( 0 < ( iBlobBytes = prepareIniQuery( &poSqlStatement, "string", pKey, pSettingName ) ) )
	{
		int iStrLen = sqlite3_column_bytes( poSqlStatement, 0 );
		if( iStrLen )
		{
			strValue = (const char *)sqlite3_column_blob( poSqlStatement, 0 );
			rc = 0;
		}
		else
		{
			strValue = "";
		}

		finalizeIniGetTransaction( poSqlStatement );
	}
	if( rc )
	{
		strValue = pDefault;
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, char * pRetBuf, int iBufLen, const char * pDefault )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	int  iBlobBytes;
	RCODE rc = -1;
	if( 0 < ( iBlobBytes = prepareIniQuery( &poSqlStatement, "string", pKey, pSettingName ) ) )
	{
		if( iBlobBytes > iBufLen )
		{
			LogMsg( LOG_ERROR, "VxSettings::getIniValue:ERROR setting %s buffer to small\n", pSettingName  );
		}
		else
		{
			int iStrLen = sqlite3_column_bytes( poSqlStatement, 0 );
			memcpy( pRetBuf, sqlite3_column_blob( poSqlStatement, 0 ), iBlobBytes );
			pRetBuf[iStrLen] = 0;
			rc = 0;
		}

		finalizeIniGetTransaction( poSqlStatement );
	}
	if( rc )
	{
		strcpy( pRetBuf, pDefault );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! get value from database.. return default if doesnt exist
void VxSettings::getIniValue( const char * pKey, const char * pSettingName, std::vector<std::string>& aoStrValues )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	int  iBlobBytes;

	if( 0 < ( iBlobBytes = prepareIniQuery( &poSqlStatement, "vector_strings", pKey, pSettingName ) ) )
	{
		std::string strCommaDelimStrings;
		int iStrLen = sqlite3_column_bytes( poSqlStatement, 0 );
		if( iStrLen )
		{
			strCommaDelimStrings = (const char *)sqlite3_column_blob( poSqlStatement, 0 );
			StdStringSplit( strCommaDelimStrings, ',', aoStrValues );
		}

		finalizeIniGetTransaction( poSqlStatement );
	}

	m_DbMutex.unlock();
}

//============================================================================
//! copy blob from database.. if length doesnt match will return false
bool VxSettings::getIniValue( const char * pKey, const char * pSettingName, void * pvRetBuf, int iBufLen )
{
	m_DbMutex.lock();

	sqlite3_stmt * poSqlStatement;
	int  iBlobBytes;
	bool bSuccess = false;
	if( 0 < ( iBlobBytes = prepareIniQuery( &poSqlStatement, "blob", pKey, pSettingName ) ) )
	{
		if( iBlobBytes > iBufLen )
		{
			LogMsg( LOG_ERROR, "VxSettings::getIniValue:ERROR setting %s blob buffer len to small\n", pSettingName  );
		}
		else
		{
			memcpy( pvRetBuf, sqlite3_column_blob(poSqlStatement, 0 ), iBlobBytes );
			bSuccess = true;
		}

		finalizeIniGetTransaction( poSqlStatement );
	}

	m_DbMutex.unlock();

	return bSuccess;
}

//============================================================================
//! prepare sql statement set value
RCODE VxSettings::prepareIniSet(	sqlite3_stmt ** ppoRetSqlStatement, 
									const char * pTableName,
									const char * pKey, 
									const char * pSettingName )
{
	char SQL_Statement[2048];
	if( 0 ==  dbOpen() )
	{
		sprintf(SQL_Statement, "DELETE FROM %s WHERE key='%s' AND setting='%s'", pTableName, pKey, pSettingName );
		if( SQLITE_OK != sqlite3_exec( m_Db, SQL_Statement, NULL, NULL, NULL ) )
		{
			LogMsg( LOG_VERBOSE, "VxSettings::prepareIniSet:ERROR %s", sqlite3_errmsg(m_Db) );
		}
		sprintf( SQL_Statement, "INSERT INTO %s (key, setting, value) VALUES ('%s','%s',?)", pTableName, pKey, pSettingName ); 
		if( SQLITE_OK == sqlite3_prepare_v2(m_Db, SQL_Statement, (int)strlen(SQL_Statement)+1, ppoRetSqlStatement, NULL) )
		{
			return 0;
		}
		else
		{
			LogMsg( LOG_ERROR, "VxSettings::prepareIniSet:ERROR %s", sqlite3_errmsg(m_Db) );
		}
	}

    sqlite3_finalize( *ppoRetSqlStatement );
	sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	dbClose();
	return -1;
}


//============================================================================
//! prepare sql statement to query value
RCODE VxSettings::prepareIniQuery(	sqlite3_stmt ** ppoRetSqlStatement, 
									const char *	pTableName,
									const char *	pKey, 
									const char *	pSettingName )
{
	char SQL_Statement[2048];
	RCODE rc;
	if( 0 == dbOpen() )
	{
		sprintf(SQL_Statement, "SELECT value FROM %s WHERE key='%s' AND setting='%s'", pTableName, pKey, pSettingName );
		if( SQLITE_OK == sqlite3_prepare_v2(m_Db, SQL_Statement, (int)strlen(SQL_Statement)+1, ppoRetSqlStatement, NULL) )
		{
			if( SQLITE_ROW == (rc = sqlite3_step( * ppoRetSqlStatement ) ) )
			{
				int iBlobBytes = sqlite3_column_bytes( * ppoRetSqlStatement, 0);
				if( iBlobBytes > 0 )
				{
					return iBlobBytes;
				}
				else
				{
					LogMsg( LOG_ERROR, "VxSettings::prepareIniQuery:ERROR %s column bytes\n", sqlite3_errmsg(m_Db) );
				}
			}
			else if( SQLITE_DONE == rc ) 
			{
				//LogMsg( LOG_VERBOSE, "VxSettings::prepareIniQuery: string key %s, setting %s  NOT FOUND\n", pKey, pSettingName );
			}
			else 
			{
				LogMsg( LOG_ERROR, "VxSettings::prepareIniQuery:ERROR %s stepping\n", sqlite3_errmsg(m_Db) );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "VxSettings::prepareIniQuery:ERROR %s preparing\n", sqlite3_errmsg(m_Db) );
		}
	}

    sqlite3_finalize( *ppoRetSqlStatement );
	sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	dbClose();
	return 0;
}

//============================================================================
//! finalize and close db
RCODE VxSettings::finalizeIniSetTransaction(	sqlite3_stmt * poSqlStatement, bool bErrorOccured )
{
	RCODE rc = -1;
	if( false == bErrorOccured )
	{
		if( SQLITE_DONE != sqlite3_step(poSqlStatement) )
		{
            sqlite3_finalize( poSqlStatement );
			LogMsg( LOG_ERROR, "VxSettings::finalizeIniSetTransaction:ERROR %s stepping\n", sqlite3_errmsg(m_Db) );
		}
		else
		{
			if( SQLITE_OK == ( rc = sqlite3_finalize(poSqlStatement) ) )
			{
				rc = 0;
			}
			else
			{
                sqlite3_finalize( poSqlStatement );
				LogMsg( LOG_ERROR, "VxSettings::finalizeIniSetTransaction:ERROR %s finalize\n", sqlite3_errmsg(m_Db) );
			}
		}
	}
	else
	{
        sqlite3_finalize( poSqlStatement );
		LogMsg( LOG_ERROR, "VxSettings::finalizeIniSetTransaction:ERROR %s\n", sqlite3_errmsg(m_Db) );
	}

   
	sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	dbClose();
	return rc;
}

//============================================================================
//! finalize and close db
RCODE VxSettings::finalizeIniGetTransaction( sqlite3_stmt * poSqlStatement, bool bErrorOccured )
{
	RCODE rc = -1;
	if( false == bErrorOccured )
	{
		if( SQLITE_OK == ( rc = sqlite3_finalize(poSqlStatement) ) )
		{
			rc = 0;
		}
		else
		{
			LogMsg( LOG_ERROR, "VxSettings::finalizeTransaction:ERROR %s\n", sqlite3_errmsg(m_Db) );
		}
	}

	sqlite3_exec(m_Db,"END",NULL,NULL,NULL);
	dbClose();
	return rc;
}
