// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

void GroupBeginCB(const std::string& _name)
{
	// Get UserData.
	int& myUserData = UTH::GetUserData<int>();

	LOG("Group of test[" << _name << "] started\n");
}

void GroupEndCB(const UTH::Group& _group)
{
	LOG("Group of test[" << _group.name << "] exit with code: " << _group.localExit << "\n");
}

void TitleCB(const std::string& _funcDecl, unsigned int _lineNum, bool _pred)
{
	LOG("Test: " << _pred << "\t" << _funcDecl << " at line:" << _lineNum << '\n');
}

void ParamsCB(const std::vector<UTH::Param>& _params)
{
	for (auto it = _params.begin(); it != _params.end(); ++it)
		LOG(it->name << ": [" << it->value << "]\n");
}

void ResultCB(bool _predicate)
{
	if (_predicate)
		LOG("Result: Success\n")
	else
		LOG("Result: Failure\n")
}

/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	int i = 5;
	int j = 9;

	SA_UTH_EQ(i, j);
	SA_UTH_EQ(i, i);
}

int main()
{
	SA_UTH_INIT();


	// Disable default console log.
	UTH::bCslLog = false;

	// Set UserData.
	int myUserData = 9;
	UTH::UserData = &myUserData;

	// Set Callbacks.
	UTH::GroupBeginCB = GroupBeginCB;
	UTH::GroupEndCB = GroupEndCB;
	UTH::TitleCB = TitleCB;
	UTH::ParamsCB = ParamsCB;
	UTH::ResultCB = ResultCB;

	SA_UTH_GP(MainTests());


	SA_UTH_EXIT();
}