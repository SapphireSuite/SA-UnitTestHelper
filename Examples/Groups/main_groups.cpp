// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

/// Methods with grouped test (can be in a separated file).
void GroupTests_Success()
{
	int i = 5;
	SA_UTH_EQ(i, i);
}

void GroupTests_Failure()
{
	int i = 5;
	int j = 9;
	SA_UTH_EQ(i, j);


	// Create subgroup manually with name "TestSubGroup".
	SA_UTH_GPB(TestSubGroup);

	SA_UTH_EQ(i, i);

	SA_UTH_GPE();
}

int main()
{
	SA_UTH_INIT();


	SA_UTH_GP(GroupTests_Success());
	SA_UTH_GP(GroupTests_Failure());


	SA_UTH_EXIT();
}