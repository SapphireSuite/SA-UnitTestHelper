// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	int i = 5;
	int j = 9;

	UTH_EQUALS_TEST(i, j);
	UTH_EQUALS_TEST(i, i);
}

int main()
{
	UTH_RUN_TESTS(MainTests());
	UTH_RUN_TESTS(MainTests());

	return UTH::exit;
}