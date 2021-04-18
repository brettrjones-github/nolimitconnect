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

#include <gtest/gtest.h>

#include <PktLib/PktAnnounce.h>

//============================================================================
class PktTest :public ::testing::Test
{ 
protected:   
	PktTest()
	{
	}

	virtual void SetUp() 
	{      
	}  

	virtual void TearDown() 
	{      
	}  

};

//============================================================================
TEST_F(PktTest, PktAnnounceSizeIsCorrect) 
{  
	EXPECT_EQ( 384, sizeof( VxNetIdentBase ) );
	EXPECT_EQ( 40, sizeof( VxPktHdr ) );
	EXPECT_EQ( 432, sizeof( VxNetIdent ) );
	EXPECT_EQ( 32, sizeof( PktAnnActionData ) );

	EXPECT_EQ( 480, sizeof( PktAnnBase ) );
	EXPECT_EQ( 512, sizeof( PktAnnounce ) );
	EXPECT_EQ( 0, sizeof( PktAnnounce ) & 0x0f );
}

