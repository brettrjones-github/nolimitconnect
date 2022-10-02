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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <stdio.h>

namespace
{
	VxNetIdent			g_oUserIdent;
}

////============================================================================
GuiParams& GetGuiParams()
{
    static GuiParams g_GuiParams;
    return g_GuiParams;
}

//============================================================================
AppGlobals::AppGlobals( AppCommon& appCommon )
: m_MyApp( appCommon )
{
}

//============================================================================
VxNetIdent* AppGlobals::getUserIdent( void )
{
	return &g_oUserIdent;
}

//============================================================================
bool AppGlobals::saveUserIdentToDatabase( void )
{
	if( false == m_MyApp.getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "AppGlobals::saveUserIdentToDatabase: ERROR updating database\n" );
		return false;
	}

	return true;
}

//============================================================================
void AppGlobals::launchWebBrowser( const char* pUri )
{
	// NOTE:qt crashes studio sometimes when openUrl so use ShellExecute
#ifdef TARGET_OS_WINDOWS
	ShellExecuteA(0, 0, pUri, 0, 0 , SW_SHOW );
#else
	QString strUrl = pUri;
	QDesktopServices::openUrl( QUrl(strUrl, QUrl::TolerantMode) );
#endif //TARGET_OS_WINDOWS
}

//============================================================================
//! update ident and save to database then send permission change to engine
void  AppGlobals::updatePluginPermission( EPluginType ePluginType, EFriendState ePluginPermission )
{
	g_oUserIdent.setPluginPermission( ePluginType, ePluginPermission );
	if( false == m_MyApp.getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "UpdatePluginPermissions: ERROR updating database\n");
	}

    m_MyApp.getEngine().setPluginPermission( ePluginType, ePluginPermission );
}

//============================================================================
//! update has picture and save to database then send picture change to engine
void UpdateHasPicture( P2PEngine& engine, int bHasPicture )
{
	g_oUserIdent.setHasProfilePicture(bHasPicture);
	if( false == GetAppInstance().getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "UpdateHasPicture: ERROR updating database\n");
	}

	engine.setHasPicture( bHasPicture );
}

//============================================================================
//! show message box to user
void ErrMsgBox( QWidget* parent, int infoLevel, const char* pMsg, ... )
{
	//build message on stack so no out of memory issue
	char szBuffer[4096];
	va_list arg_ptr;
	va_start(arg_ptr, pMsg);
    vsnprintf(szBuffer, 4096, pMsg, arg_ptr);
	szBuffer[4095] = 0;
	va_end(arg_ptr);

	QMessageBox msgBox( parent );
	msgBox.setText( szBuffer );
	msgBox.exec();
}

