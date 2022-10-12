#ifndef ZIPPER_H
#define ZIPPER_H

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

// BRJ NOTE: for other languages see
// http://zlibce.codeplex.com/SourceControl/latest#zlibSampleCS/Program.cs
// for wrappers of zlib

#include <NlcDependLibrariesConfig.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxThread.h>

class IZipperCallback
{
public:
	virtual void				zipOperationBegan( const char * zipFileName ) = 0;
	virtual void				zipOperationCompleted( const char * zipFileName ) = 0;
	virtual void				unzipOperationBegan( const char * unzipFileName ) = 0;
	virtual void				unzipOperationCompleted( const char * unzipFileName ) = 0;
};

class Zipper : public VxFileUtil
{
public:
	class ToZipFile
	{
	public:
		ToZipFile( const char * fromDir, const char * filePathAndName );
		ToZipFile(const ToZipFile& rhs);

		ToZipFile& operator=(const ToZipFile& rhs);

		std::string				m_FromDirectory;
		std::string				m_ToZipFile;

	private:
		ToZipFile();
	};

	Zipper( IZipperCallback& toCaller );
	virtual ~Zipper();

	void						clearZipFileList( void );
	void						addToZipDirectory( const char * dir  );

	void						addToZipFile(	const char * file,
												const char * dirToTruncate );

	void						zipFiles( const char * zipfileName, bool compressUsingThread = false  );
	void						unzipFile(	const char *	zipfileName,
											const char *	directoryToUnzipTo	= NULL,
											bool			unzipUsingThread	= false );
	void						abortOperation( void );

	void						setCompressionRate( int rate0to9_0faster_9better  );

	int							getUnzipProgress( void );

	// these methods normally only called from zlib
	void						getNextFileToZip( void );
	int							getToZipFileCount( void );

	void						doZipOperation( void );
	void						doUnzipOperation( void );

protected:
	IZipperCallback&			m_ToCaller;
	VxThread					m_ZipThread;
	VxThread					m_UnzipThread;
	std::vector<ToZipFile>		m_ToZipFiles;
	unsigned int				m_uiZipFilesIndex;
	int							m_iCompressionRate;

	std::string					m_ZipfileName;
	std::string					m_UnzipfileName;
	std::string					m_UnzipDirectory;
};

#endif // ZIPPER_H