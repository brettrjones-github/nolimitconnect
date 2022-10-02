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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletBase.h"
#include <GuiInterface/IToGui.h>

class GuiHostSession;

class AppletHostBase : public AppletBase
{
	Q_OBJECT
public:
    AppletHostBase( const char* objName, AppCommon& app, QWidget* parent );
	virtual ~AppletHostBase() = default;

    void                        setHostType( EHostType hostType )           { m_HostType = hostType; }
    EHostType                   getHostType(  void )                        { return m_HostType; }

    void                        setSearchType( ESearchType searchType )     { m_SearchType = searchType; }
    ESearchType                 getSearchType(  void )                      { return m_SearchType; }

    virtual void                onJointButtonClicked( GuiHostSession* hostSession );

protected:
    EHostType                   m_HostType{ eHostTypeUnknown };
    ESearchType                 m_SearchType{ eSearchNone };
    VxGUID                      m_JoinSessionId;
    std::string                 m_JoinHostUrl;
    EHostType                   m_JoinHostType{ eHostTypeUnknown };
};


