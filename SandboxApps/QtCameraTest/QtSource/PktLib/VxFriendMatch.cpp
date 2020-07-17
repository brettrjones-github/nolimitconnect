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
// http://www.nolimitconnect.com
//============================================================================

#include "VxFriendMatch.h"

//============================================================================
//! describe friend state
const char * DescribeFriendState( EFriendState eFriendState )
{
	switch( eFriendState )
	{
	case eFriendStateAnonymous:	// anonymous user
		return "Anonymous ";
	case eFriendStateGuest:		// guest user
		return "Guest ";
	case eFriendStateFriend:	// friend user
		return "Friend ";
	case eFriendStateAdmin:		// administrator
		return "Administrator ";
	default:
		return "Ignore ";
	}
}

//============================================================================
FriendMatch::FriendMatch() 
: m_u8FriendMatch(0x11) // assume anonymous
{
} 

//! return true if is ignored
bool			FriendMatch::isIgnored()										{ return eFriendStateIgnore == getMyFriendshipToHim(); }
//! return true if is Anonymous
bool			FriendMatch::isAnonymous()										{ return eFriendStateAnonymous == getMyFriendshipToHim(); }
//! return true if is Guest
bool			FriendMatch::isGuest()											{ return eFriendStateGuest == getMyFriendshipToHim(); }
//! return true if is Friend
bool			FriendMatch::isFriend()											{ return eFriendStateFriend == getMyFriendshipToHim(); }
//! return true if is Administrator
bool			FriendMatch::isAdministrator()									{ return eFriendStateAdmin == getMyFriendshipToHim(); }
//! set my permissions to him as ignored
void			FriendMatch::makeIgnored()										{ setMyFriendshipToHim(eFriendStateIgnore); }
//! set my permissions to him as Anonymous
void			FriendMatch::makeAnonymous()									{ setMyFriendshipToHim(eFriendStateAnonymous); }
//! set my permissions to him as Guest
void			FriendMatch::makeGuest()										{ setMyFriendshipToHim(eFriendStateGuest); }
//! set my permissions to him as Friend
void			FriendMatch::makeFriend()										{ setMyFriendshipToHim(eFriendStateFriend); }
//! set my permissions to him as Administrator
void			FriendMatch::makeAdministrator()								{ setMyFriendshipToHim(eFriendStateAdmin); }
//! wants to be friend
bool			FriendMatch::wantsToBeFriend()									{ return eFriendStateFriend == getHisFriendshipToMe(); }
//! wants to be friend
bool			FriendMatch::wantsToBeAdministrator()							{ return eFriendStateAdmin == getHisFriendshipToMe(); }

//! set permission level I have given to friend
void			FriendMatch::setMyFriendshipToHim( EFriendState eFriendState )	
{ 
	m_u8FriendMatch &= 0xf0; 
	m_u8FriendMatch |= eFriendState; 
}

//! get permission level I have given to friend
EFriendState	FriendMatch::getMyFriendshipToHim( void )						{ return (EFriendState)(m_u8FriendMatch & 0x0f); }

//! set permission level he has given to me
void			FriendMatch::setHisFriendshipToMe( EFriendState eFriendState )	
{ 
	m_u8FriendMatch &= 0x0f; 
	m_u8FriendMatch |= (eFriendState << 4); 
}

//! get permission level he has given to me
EFriendState	FriendMatch::getHisFriendshipToMe( void )						{ return (EFriendState)((m_u8FriendMatch >> 4) & 0x0f); }

//! reverse the permissions
void			FriendMatch::reversePermissions( void )
{
	uint8_t u8TmpPermission = m_u8FriendMatch << 4;
	m_u8FriendMatch = u8TmpPermission | (( m_u8FriendMatch >> 4 ) & 0x0f);
}
//! return string with friend state He has given Me
void			FriendMatch::describeHisFriendshipToMe( std::string & strRetPermission ) { strRetPermission = DescribeFriendState( getHisFriendshipToMe() ); }
//! return string with friend state He has given Me
const char *	FriendMatch::describeHisFriendshipToMe( void )							{ return DescribeFriendState( getHisFriendshipToMe() ); }
//! return string with friend state I have given Him
void			FriendMatch::describeMyFriendshipToHim( std::string & strRetPermission ) { strRetPermission = DescribeFriendState( getMyFriendshipToHim() ); }
//! return string with friend state I have given Him
const char *	FriendMatch::describeMyFriendshipToHim( void )							{ return DescribeFriendState( getMyFriendshipToHim() ); }
