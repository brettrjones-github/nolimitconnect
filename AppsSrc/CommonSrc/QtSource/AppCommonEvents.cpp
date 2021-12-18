//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "AppCommon.h"	

#include <GuiInterface/IGoTv.h>

//============================================================================
void AppCommon::fromGuiKeyPressEvent( int moduleNum, int key, int mod )
{
    m_GoTv.fromGuiKeyPressEvent( moduleNum, key, mod );
}

//============================================================================
void AppCommon::fromGuiKeyReleaseEvent( int moduleNum, int key, int mod )
{
    m_GoTv.fromGuiKeyReleaseEvent( moduleNum, key, mod );
}

//============================================================================
void AppCommon::fromGuiMousePressEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
    m_GoTv.fromGuiMousePressEvent( moduleNum, mouseXPos, mouseYPos, mouseButton );
}

//============================================================================
void AppCommon::fromGuiMouseReleaseEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
    m_GoTv.fromGuiMouseReleaseEvent( moduleNum, mouseXPos, mouseYPos, mouseButton );
}

//============================================================================
void AppCommon::fromGuiMouseMoveEvent( int moduleNum, int mouseXPos, int mouseYPos )
{
    m_GoTv.fromGuiMouseMoveEvent( moduleNum, mouseXPos, mouseYPos );
}

//============================================================================
void AppCommon::fromGuiResizeBegin( int moduleNum, int winWidth, int winHeight )
{
	m_GoTv.fromGuiResizeBegin( moduleNum, winWidth, winHeight );
}

//============================================================================
void AppCommon::fromGuiResizeEvent( int moduleNum, int winWidth, int winHeight )
{
    m_GoTv.fromGuiResizeEvent( moduleNum, winWidth, winHeight );
}

//============================================================================
void AppCommon::fromGuiResizeEnd( int moduleNum, int winWidth, int winHeight )
{
	m_GoTv.fromGuiResizeEnd( moduleNum, winWidth, winHeight );
}

//============================================================================
void AppCommon::fromGuiCloseEvent( int moduleNum )
{
    m_GoTv.fromGuiCloseEvent( moduleNum );
}

//============================================================================
void AppCommon::fromGuiVisibleEvent( int moduleNum, bool isVisible )
{
    m_GoTv.fromGuiVisibleEvent( moduleNum, isVisible );
}
