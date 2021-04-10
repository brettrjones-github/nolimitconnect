#ifndef VX_SETTINGS_H
#define VX_SETTINGS_H

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

#include "DbBase.h"

#include <string>
#include <vector>

//! implements settings like ini file using sqlite database
class VxSettings : public DbBase
{
public:
	//=== constants ===//
	static const int	MAX_VALUE_STRING_LEN = 2047;

	VxSettings( const char * settingDbName );
	virtual ~VxSettings() = default;

	bool				isValid( void )							{ return m_bIsValid; }
	void				setIsValid( bool bValid )				{ m_bIsValid = bValid; }

	//=== overrides ===//
	//! override onCreateTables to create our tables
	virtual RCODE		onCreateTables( int iDbVersion );
	//! override onDeleteTables to delete our tables
	virtual RCODE		onDeleteTables( int oldDbVersion );

	//=== methods ===//
	//! startup Settings.. if database doesnt exist then create it and call DbCreateTables
	RCODE				vxSettingsStartup( const char * pDbFileName );
	//! shutdown Settings
	void				vxSettingsShutdown( void );

	void				setIniValue( const char * pKey, const char * pSettingName, bool& bValue );
	void				setIniValue( const char * pKey, const char * pSettingName, int8_t& s8Value );
	void				setIniValue( const char * pKey, const char * pSettingName, uint8_t& u8Value );
	void				setIniValue( const char * pKey, const char * pSettingName, int16_t& s16Value );
	void				setIniValue( const char * pKey, const char * pSettingName, uint16_t& u16Value );
	void				setIniValue( const char * pKey, const char * pSettingName, int32_t& s32Value );
	void				setIniValue( const char * pKey, const char * pSettingName, uint32_t& u32Value );
	void				setIniValue( const char * pKey, const char * pSettingName, int64_t& s64Value );
	void				setIniValue( const char * pKey, const char * pSettingName, uint64_t& u64Value );
	void				setIniValue( const char * pKey, const char * pSettingName, float& f32Value );
	void				setIniValue( const char * pKey, const char * pSettingName, double& f64Value );
	void				setIniValue( const char * pKey, const char * pSettingName, std::string& strValue );
	void				setIniValue( const char * pKey, const char * pSettingName, const char * pValue );
	void				setIniValue( const char * pKey, const char * pSettingName, std::vector<std::string>& aoStrValues );
	void				setIniValue( const char * pKey, const char * pSettingName, void * pvObject, int iObjectLen );

	//! get value from database.. return default if doesnt exist
	void				getIniValue( const char * pKey, const char * pSettingName, bool& bValue, bool bDefault = false );
	void				getIniValue( const char * pKey, const char * pSettingName, int8_t& s8Value, int8_t s8Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, uint8_t& u8Value, uint8_t u8Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, int16_t& s16Value, int16_t s16Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, uint16_t& u16Value, uint16_t u16Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, int32_t& s32Value, int32_t s32Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, uint32_t& u32Value, uint32_t u32Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, int64_t& s64Value, int64_t s64Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, uint64_t& u64Value, uint64_t u64Default = 0 );
	void				getIniValue( const char * pKey, const char * pSettingName, float& f32Value, float f32Default = 0.0f );
	void				getIniValue( const char * pKey, const char * pSettingName, double& f64Value, double f64Default = 0.0 );
	void				getIniValue( const char * pKey, const char * pSettingName, std::string& strValue, const char * pDefault = "" );
	void				getIniValue( const char * pKey, const char * pSettingName, char * pRetBuf, int iBufLen, const char * pDefault = "" );
	void				getIniValue( const char * pKey, const char * pSettingName, std::vector<std::string>& aoStrValues );
	bool				getIniValue( const char * pKey, const char * pSettingName, void * pvRetBuf, int iBufLen );

private:
	//! prepare sql statement set value
	RCODE				prepareIniSet(	sqlite3_stmt ** ppoRetSqlStatement, 
										const char *	pTableName,
										const char *	pKey, 
										const char *	pSettingName ); 

	//! prepare sql statement to query value
	RCODE				prepareIniQuery(	sqlite3_stmt ** ppoRetSqlStatement, 
											const char *	pTableName,
											const char *	pKey, 
											const char *	pSettingName ); 

	//! finalize and close db after setting 
	RCODE				finalizeIniSetTransaction(	sqlite3_stmt * poSqlStatement, bool bErrorOccured = false ); 

	//! finalize and close db
	RCODE				finalizeIniGetTransaction(	sqlite3_stmt * poSqlStatement, bool bErrorOccured = false ); 

	//=== vars ===//
	bool				m_bIsValid;
};

#endif // VX_SETTINGS_H
