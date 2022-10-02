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

#include "AppletJoinBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
AppletJoinBase::AppletJoinBase( const char*name, AppCommon& app, QWidget* parent )
: AppletBase( name, app, parent )
{
    m_NetworkHostUrl = m_MyApp.getFromGuiInterface().fromGuiQueryDefaultUrl( eHostTypeNetwork );
    VxPtopUrl netHostUrl( m_NetworkHostUrl );
    m_NetHostPtopUrl = netHostUrl;
    if( m_NetHostPtopUrl.isValid() )
    {
        m_NetworkHostOnlineId = m_NetHostPtopUrl.getOnlineId();
    }
}

//============================================================================
AppletJoinBase::~AppletJoinBase()
{
}

//============================================================================
void AppletJoinBase::queryHostedList( void )
{
	if( isNetworkHostUrlValid() )
	{
		VxGUID nullGuid;
		m_MyApp.getFromGuiInterface().fromGuiQueryHostListFromNetworkHost( m_NetHostPtopUrl, getHostType(), nullGuid );
	}
	else
	{
		okMessageBox( QObject::tr( "Network Host Url Is Invalid" ), QObject::tr( "You need to set a valid network host url in network settings" ) );
	}
}
