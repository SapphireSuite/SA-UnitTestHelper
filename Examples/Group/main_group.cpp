// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

/// Methods with grouped test (can be in a separated file).
void GroupTests_Success()
{
	int i = 5;
	UTH_EQUALS_TEST(i, i);
}

void GroupTests_Failure()
{
	int i = 5;
	int j = 9;
	UTH_EQUALS_TEST(i, j);


	// Create subgroup manually with name "TestSubGroup".
	UTH_GROUP_BEGIN(TestSubGroup);

	UTH_EQUALS_TEST(i, i);

	UTH_GROUP_END();
}

int main()
{
	UTH_RUN_TESTS(GroupTests_Success());
	UTH_RUN_TESTS(GroupTests_Failure());

	return UTH::exit;
}