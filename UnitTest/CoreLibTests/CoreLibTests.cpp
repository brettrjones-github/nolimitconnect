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

#include <CoreLib/VxGUID.h>

//============================================================================
class CoreLibTest :public ::testing::Test
{ 
protected:   
    CoreLibTest()
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
TEST_F(CoreLibTest, GuidToStringTest) 
{  
    uint64_t hiPart = 0x789101112;
    uint64_t loPart = 0x123456789;
    std::string testStrId( "!00000007891011120000000123456789!" );

    VxGUID testGuid1( hiPart, loPart );
    std::string testStrExport;
    testGuid1.toOnlineIdString(testStrExport);

    EXPECT_TRUE( testStrExport == testStrId );

    VxGUID testGuid2;
    testGuid2.fromOnlineIdString( testStrId.c_str() );

	EXPECT_TRUE( testGuid1 == testGuid2 );
}

