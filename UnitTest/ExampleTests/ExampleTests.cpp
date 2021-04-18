#include <gtest/gtest.h>

//============================================================================
class ExampleTest :public ::testing::Test
{ 
protected:   
	ExampleTest()
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
TEST_F(ExampleTest, EqualsTrue) 
{  
	EXPECT_TRUE( true );
}

