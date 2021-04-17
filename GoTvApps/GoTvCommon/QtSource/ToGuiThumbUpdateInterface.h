#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include <GuiInterface/IDefs.h>

class ThumbInfo;
class VxGUID;

class ToGuiThumbUpdateInterface
{
public:
    virtual void				toGuiThumbAdded( ThumbInfo * thumb ) = 0; 
    virtual void				toGuiThumbUpdated( ThumbInfo * thumb ) = 0; 
    virtual void				toGuiThumbRemoved( VxGUID& thumbId ) = 0; 
};


