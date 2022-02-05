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

#include <PktLib/GroupieId.h>

class GuiHostJoin;

class GuiHostJoinCallback
{
public:
    virtual void				callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) {};
    virtual void				callbackGuiHostJoinWasGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) {};
    virtual void				callbackGuiHostJoinIsGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) {};
    virtual void				callbackGuiHostUnJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) {};
    virtual void				callbackGuiHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) {};
    virtual void				callbackGuiHostJoinLeaveHost( GroupieId& groupieId ) {};
    virtual void				callbackGuiHostUnJoin( GroupieId& groupieId ) {};
    virtual void				callbackGuiHostJoinRemoved( GroupieId& groupieId ) {};
};

