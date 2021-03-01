// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

struct MyClass
{
	float myFloat = 0.0f;

	static bool Equals(const MyClass& _lhs, const MyClass& _rhs) { return _lhs.myFloat == _rhs.myFloat; }

	std::string ToString() const { return std::to_string(myFloat); }
};

bool GlobalFunc(bool _pred)
{
	return _pred;
}

int GlobalAdd(int _i, int _j)
{
	return _i + _j;
}

int main()
{
	SA_UTH_INIT();

	// UTH SF
	bool failurePred = false;

	/// func_name, args1, args2...
	SA_UTH_SF(GlobalFunc, true);
	SA_UTH_SF(GlobalFunc, failurePred); // Error.


	// UTH RSF
	int i = 4;
	int j = 1;

	/// func_name, result, args1, args2...
	SA_UTH_RSF(GlobalAdd, 5, i, j);
	SA_UTH_RSF(GlobalAdd, 8, i, j); // Error.


	// Custom elem.
	MyClass m1{ 4.56f };
	MyClass m2{ 8.15f };

	SA_UTH_RSF(MyClass::Equals, false, m1, m2);
	SA_UTH_RSF(MyClass::Equals, true, m1, m2); // Error.


	SA_UTH_EXIT();
}
