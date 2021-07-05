#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones
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

#include "AppletBase.h"
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>


class AppletPlayerBase : public AppletBase
{
	Q_OBJECT
public:
	AppletPlayerBase( const char * ObjName, AppCommon& app, QWidget * parent );
	virtual ~AppletPlayerBase(){};


protected:
	void						initAppletPlayerBase( void );

	//=== vars ===//
	bool						m_ActivityCallbacksEnabled;
	bool						m_IsPlaying;
	bool						m_SliderIsPressed;

};


