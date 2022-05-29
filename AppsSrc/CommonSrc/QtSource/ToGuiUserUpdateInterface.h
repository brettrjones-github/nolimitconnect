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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

class VxGUID;
class VxNetIdent;

class ToGuiUserUpdateInterface
{
public:
    virtual void				toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) = 0;
    virtual void				toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId ) = 0;

    virtual void				toGuiContactAdded( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactRemoved( VxGUID& onlineId ) = 0; 

	virtual void				toGuiContactOnline( VxNetIdent * netIdent ) = 0; 
	virtual void				toGuiContactOffline( VxNetIdent * netIdent ) = 0; 

    virtual void				toGuiContactNameChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactDescChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent ) = 0; 

    virtual void				toGuiUpdateMyIdent( VxNetIdent * netIdent ) = 0; 
    virtual void				toGuiSaveMyIdent( VxNetIdent* netIdent ) = 0;
};


