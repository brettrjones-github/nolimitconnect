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

#include "ActivityShowHelp.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>

#include <QMessageBox>

//============================================================================
ActivityShowHelp::ActivityShowHelp( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_SHOW_HELP, app, 0 )
{
	Q_UNUSED(parent);
	ui.setupUi(this);
    connectBarWidgets();

	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotReject()) );

	loadHelp();
}

//============================================================================
void ActivityShowHelp::slotReject( void )
{
	reject();
}

//============================================================================
void ActivityShowHelp::slotClipboard( void )
{

}

//============================================================================
void ActivityShowHelp::loadHelp( void )
{
	QString helpResource = ":/AppRes/Resources/RelayHelp.txt";
	ui.LogEdit->clear();
	QFile file(helpResource);
	if( file.open( QIODevice::ReadOnly|QIODevice::Text) )
	{
		QByteArray dump = file.readAll();
		ui.LogEdit->appendPlainText( dump );
		file.close();
	}
	else
	{
		LogMsg( LOG_ERROR, "ActivityShowHelp::loadHelp failed to Open %s\n", helpResource.toUtf8().constData() );
	}
}

//============================================================================
void ActivityShowHelp::loadHelpFile( const char * pFileName, uint64_t u64FileLen )
{
	//FILE * poFile = VxFileUtil::fileOpen( pFileName, "rb" );
	//if( poFile )
	//{
	//	uint32_t u32LenToRead = (uint32_t)u64FileLen;
	//	if( u64FileLen > 30000 )
	//	{
	//		VxFileUtil::fileSeek( poFile, u64FileLen - 30000 );
	//		u32LenToRead = 30000;
	//	}
	//	char * pContent = new char[u32LenToRead + 4];
	//	size_t lenRead = fread( pContent, 1, u32LenToRead, poFile );
	//	if( lenRead == u32LenToRead )
	//	{
	//		pContent[u32LenToRead] = '\n';
	//		pContent[u32LenToRead + 1] = 0;
	//		ui.LogEdit->clear();
	//		ui.LogEdit->appendPlainText( pContent );
	//	}

	//	fclose( poFile );
	//	delete pContent;
	//}
	//else
	//{
	//	QMessageBox::warning(this, tr("Debug Log"), tr("Log File Could Not Be Read.. have you enabled Log To File?" ) );
	//	reject();
	//}
}
