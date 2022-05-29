#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "IDefs.h"

class IGoTvEvents
{
public:
    virtual void                fromGuiKeyPressEvent( int moduleNum, int key, int mod ) = 0;
    virtual void                fromGuiKeyReleaseEvent( int moduleNum, int key, int mod ) = 0;

    virtual void                fromGuiMousePressEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton ) = 0;
    virtual void                fromGuiMouseReleaseEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton ) = 0;
    virtual void                fromGuiMouseMoveEvent( int moduleNum, int mouseXPos, int mouseYPos ) = 0;

	virtual void                fromGuiResizeBegin( int moduleNum, int winWidth, int winHeight ) = 0;
	virtual void                fromGuiResizeEvent( int moduleNum, int winWidth, int winHeight ) = 0;
	virtual void                fromGuiResizeEnd( int moduleNum, int winWidth, int winHeight ) = 0;

    virtual void                fromGuiCloseEvent( int moduleNum ) = 0;
    virtual void                fromGuiVisibleEvent( int moduleNum, bool isVisible ) = 0;

};
