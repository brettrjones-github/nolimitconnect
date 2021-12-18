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
// http://www.nolimitconnect.com
//============================================================================


#include "VxResourceToRealFile.h"

#include <CoreLib/VxGlobals.h>

#include <QFileInfo>
#include <QDir>
#include <QDebug>

//============================================================================
VxResourceToRealFile::VxResourceToRealFile( const QString & resPath, QObject * parent )
: QFile( parent )
{
    setObjectName( "VxResourceToRealFile" );
	QFileInfo resPathInfo( resPath );
	std::string appResDirStr = VxGetAppNoLimitDataDirectory() + "appres/";
	QString appResDirQString( appResDirStr.c_str() );
	QDir appResDir;
	if( !appResDir.cd( appResDirQString ) )
	{
		appResDir.mkdir( appResDirQString );
		appResDir.cd( appResDirQString );
	}

	QString onDiskFileName = appResDir.filePath( resPathInfo.fileName() );
	setFileName( onDiskFileName );
	if( !exists() )
	{
		QFile resfile( resPath );
		if( resfile.open( QIODevice::ReadOnly ) )
		{
			if( open( QIODevice::ReadWrite ) )
			{
				if( 0 == write( resfile.readAll() ) )
				{
					qWarning() << "Could not write app resource file " << onDiskFileName;
				}

				close();
			}
			else
			{
				qWarning() << "Could not open app resource file for writing " << onDiskFileName;
			}

			resfile.close();
		}
		else
		{
			qWarning() << "Could not open resource file " << resPathInfo.fileName();
		}
	}
}