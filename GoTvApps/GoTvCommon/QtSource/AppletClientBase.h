#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#include <GoTvInterface/IToGui.h>


class AppletClientBase : public AppletBase
{
	Q_OBJECT
public:
    AppletClientBase( const char * objName, AppCommon& app, QWidget * parent );
	virtual ~AppletClientBase() = default;

    void                        setHostType( EHostType hostType )           { m_HostType = hostType; }
    EHostType                   getHostType(  void )                        { return m_HostType; }

    void                        setSearchType( ESearchType searchType )     { m_SearchType = searchType; }
    ESearchType                 getSearchType(  void )                      { return m_SearchType; }

protected:
    EHostType                   m_HostType{ eHostTypeUnknown };
    ESearchType                 m_SearchType{ eSearchNone };
};


