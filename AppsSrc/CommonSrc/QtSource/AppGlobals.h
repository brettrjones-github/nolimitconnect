#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include <PktLib/VxCommon.h>

#include <QString>

class AppCommon;
class MySndMgr;
class QWidget;
class P2PEngine;
class IFromGui;
class VxNetIdent;
class GuiParams;

class AppGlobals
{
public:
	AppGlobals( AppCommon& appCommon );

	void						setRequiresProxy( bool bRequiresProxy );
	bool						getRequiresProxy( void );
	VxNetIdent	*				getUserIdent( void );
	bool						saveUserIdentToDatabase( void );
    //! update identity and save to database then send permission change to engine
    void						updatePluginPermission( enum EPluginType ePluginType, enum EFriendState ePluginPermission );

	void						launchWebBrowser( const char * pUri );
protected:
	AppCommon&					m_MyApp;
};

//! gui params and constants
GuiParams&                      GetGuiParams();

//! update has picture and save to database then send picture change to engine
void							UpdateHasPicture( P2PEngine& engine, int bHasPicture );

//! show message box to user
void							ErrMsgBox( QWidget *, int infoLevel, const char * pMsg, ... );

