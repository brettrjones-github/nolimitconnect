//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "GuiOfferHostMgr.h"
#include "AppCommon.h"

//============================================================================
void GuiOfferHostMgr::onAppCommonCreated( void )
{
    m_MyApp.getEngine().getOfferHostMgr().addOfferHostMgrClient( this, true );
}

//============================================================================
bool GuiOfferHostMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}
