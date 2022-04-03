#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones 
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

#include "PktTypes.h"

#pragma pack(push)
#pragma pack(1)

class PktImAliveReq : public VxPktHdr
{
public:
	PktImAliveReq();

	//void						setTimestamp( uint64_t timeStamp );
	//uint64_t							getTimestamp( void );

private:
	uint64_t							m_Res;								
};

class PktImAliveReply : public VxPktHdr
{
public:
	PktImAliveReply();

	//void						setTimestamp( uint64_t timeStamp );
	//uint64_t							getTimestamp( void );

private:
	uint64_t							m_Res;								
};

#pragma pack(pop)
