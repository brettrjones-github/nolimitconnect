#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxGUID.h>

class AppCommon;
class QWidget;
class VxMyFileInfo : public QObject
{
	Q_OBJECT
public:
	VxMyFileInfo(QWidget *parent = 0 );
	VxMyFileInfo( const VxFileInfo& rhs );
	VxMyFileInfo( const VxMyFileInfo& rhs );
	VxMyFileInfo( QString fileName, uint8_t fileType, uint64_t fileLen, VxGUID& assetId, VxSha1Hash& fileHashId );
	VxMyFileInfo( QString fileName, uint8_t fileType, uint64_t fileLen, VxGUID& assetId, uint8_t * fileHashData );
	virtual ~VxMyFileInfo();

	void						setFullFileName( QString fileName );
	QString&					getFullFileName( void )					{ return m_FullFileName; }
	void						setFileType( uint8_t	fileType )		{ m_FileType = fileType; }
	uint8_t						getFileType( void ) const				{ return m_FileType; }
	void						setFileLength( uint64_t fileLen )		{ m_FileLen = fileLen; }
	uint64_t					getFileLength( void ) const				{ return m_FileLen; }
	void						setFileHashId( VxSha1Hash& id )			{ m_FileHashId = id; }
	void						setFileHashId( uint8_t * fileHashData )	{ m_FileHashId.setHashData( fileHashData ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHashId; }

	void						setAssetId( VxGUID& assetId )			{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )						{ return m_AssetId; }

	void						setIsInLibrary( bool inLibaray )		{ m_IsInLibrary = inLibaray; }
	bool						getIsInLibrary( void ) const			{ return m_IsInLibrary; }
	void						setIsShared( bool isShared )			{ m_IsShared = isShared; }
	bool						getIsShared( void ) const				{ return m_IsShared; }

	void						setJustFileName( QString fileName )		{ m_JustFileName = fileName; }
	QString&					getJustFileName( void )					{ return m_JustFileName; }
	void						setFilePath( QString filePath )			{ m_FilePath = filePath; }
	QString&					getFilePath( void )						{ return m_FilePath; }

	bool						isDirectory( void )						{ return ( m_FileType & VXFILE_TYPE_DIRECTORY ) ? true : false; }
	QString						describeFileLength( void );
	QString						describeFileType( void );
	void						playFile( AppCommon& myApp );

	VxMyFileInfo&				operator =( const VxFileInfo& a );
	VxMyFileInfo&				operator =( const VxMyFileInfo& a );
	bool						operator == ( const VxMyFileInfo& a ) const;
	bool						operator != ( const VxMyFileInfo& a ) const;
	bool						operator < ( const VxMyFileInfo& b ) const;
	bool						operator <= ( const VxMyFileInfo& b ) const;
	bool						operator > ( const VxMyFileInfo& b ) const;
	bool						operator >= ( const VxMyFileInfo& b ) const;

private:
	void						updateJustFileName( void );
	void						generateAssetId( bool ifNotValid = true );

	QString						m_FullFileName;
	uint8_t						m_FileType;
	uint64_t					m_FileLen{ 0 };
	VxSha1Hash					m_FileHashId;
	QString						m_JustFileName;
	QString						m_FilePath;
	bool						m_IsInLibrary{ false };
	bool						m_IsShared{ false };
	VxGUID						m_AssetId;
};

