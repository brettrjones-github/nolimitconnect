
#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	getchar();
}