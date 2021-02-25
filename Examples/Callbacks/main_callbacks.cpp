// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

void GroupBeginCB(const std::string& _name)
{
	SA_UTH_LOG("Group of test[" << _name << "] started\n");
}

void GroupEndCB(const UTH::Group& _group)
{
	SA_UTH_LOG("Group of test[" << _group.name << "] exit with code: " << _group.localExit << "\n");
}

void TitleCB(const std::string& _funcDecl, unsigned int _lineNum)
{
	SA_UTH_LOG("Test:\t" << _funcDecl << " at line:" << _lineNum << '\n');
}

void ParamsCB(const std::vector<UTH::Param>& _params)
{
	for (auto it = _params.begin(); it != _params.end(); ++it)
		SA_UTH_LOG(it->name << ": [" << it->value << "]\n");
}

void ResultCB(bool _predicate)
{
	if (_predicate)
		SA_UTH_LOG("Result: Success\n")
	else
		SA_UTH_LOG("Result: Failure\n")
}

/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	int i = 5;
	int j = 9;

	SA_UTH_EQUALS(i, j);
	SA_UTH_EQUALS(i, i);
}

int main()
{
	UTH::GroupBeginCB = GroupBeginCB;
	UTH::GroupEndCB = GroupEndCB;
	UTH::TitleCB = TitleCB;
	UTH::ParamsCB = ParamsCB;
	UTH::ResultCB = ResultCB;

	SA_UTH_GROUP(MainTests());

	return UTH::exit;
}