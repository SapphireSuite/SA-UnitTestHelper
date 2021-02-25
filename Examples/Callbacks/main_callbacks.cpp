// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

void GroupBeginCB(const std::string& _name)
{
	UTH_LOG("Group of test[" << _name << "] started\n");
}

void GroupEndCB(const UTH::Group& _group)
{
	UTH_LOG("Group of test[" << _group.name << "] exit with code: " << _group.localExit << "\n");
}

void TitleCB(const std::string& _funcDecl, unsigned int _lineNum)
{
	UTH_LOG("Test:\t" << _funcDecl << " at line:" << _lineNum << '\n');
}

void ParamsCB(const std::vector<UTH::Param>& _params)
{
	for (auto it = _params.begin(); it != _params.end(); ++it)
		UTH_LOG(it->name << ": [" << it->value << "]\n");
}

void ResultCB(bool _predicate)
{
	if (_predicate)
		UTH_LOG("Result: Success\n")
	else
		UTH_LOG("Result: Failure\n")
}

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
	UTH::GroupBeginCB = GroupBeginCB;
	UTH::GroupEndCB = GroupEndCB;
	UTH::TitleCB = TitleCB;
	UTH::ParamsCB = ParamsCB;
	UTH::ResultCB = ResultCB;

	UTH_GROUP_TESTS(MainTests());

	return UTH::exit;
}