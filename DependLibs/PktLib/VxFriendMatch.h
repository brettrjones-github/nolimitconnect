#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/VxDefs.h>
#include <GuiInterface/IDefs.h>

#include <string>

#pragma pack(push)
#pragma pack(1)
class PktBlobEntry;

class FriendMatch
{
public:
	FriendMatch() = default;
    FriendMatch( const FriendMatch& rhs );
    FriendMatch&                operator =( const FriendMatch& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    //! if was anonymouse upgrade to guest friendship
    void                        upgradeToGuestFriendship( void );

	bool						isIgnored();
	bool						isAnonymous();
	bool						isGuest();
	bool						isFriend();
	bool						isAdministrator();
	//! set my permissions to him as ignored
	void						makeIgnored();
	//! set my permissions to him as Anonymous
	void						makeAnonymous();
	//! set my permissions to him as Guest
	void						makeGuest();
	//! set my permissions to him as Friend
	void						makeFriend();
	//! set my permissions to him as Administrator
	void						makeAdministrator();
	//! wants to be friend
	bool						wantsToBeFriend();
	//! wants to be Administrator
	bool						wantsToBeAdministrator();


	//! set permission level I have given to friend
    void						setMyFriendshipToHim( enum EFriendState eFriendState );
	//! get permission level I have given to friend
	EFriendState				getMyFriendshipToHim( void );

	//! set permission level he has given to me
	void						setHisFriendshipToMe( EFriendState eFriendState );

	//! get permission level he has given to me
	EFriendState				getHisFriendshipToMe( void );

	//! reverse the permissions
	void						reversePermissions( void );
	//! return string with friend state He has given Me
	void						describeHisFriendshipToMe( std::string & strRetPermission );
	//! return string with friend state He has given Me
	const char*				describeHisFriendshipToMe( void );
	//! return string with friend state I have given Him
	void						describeMyFriendshipToHim( std::string & strRetPermission );
	//! return string with friend state I have given Him
	const char*				describeMyFriendshipToHim( void );

	//=== vars ===//
    uint8_t						m_u8FriendMatch{ 0x11 };
};

#pragma pack(pop)

