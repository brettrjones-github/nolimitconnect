#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "config_gotvapps.h"
class ToGuiFileXferInterface;

class ToGuiFileXferClient
{
public:
	ToGuiFileXferClient(	ToGuiFileXferInterface *	callback, 
							void *						userData );
	ToGuiFileXferClient( const ToGuiFileXferClient &rhs );

	ToGuiFileXferClient&	operator =( const ToGuiFileXferClient &rhs );

	//=== vars ===//
	ToGuiFileXferInterface *	m_Callback; 
	void *						m_UserData;
};

