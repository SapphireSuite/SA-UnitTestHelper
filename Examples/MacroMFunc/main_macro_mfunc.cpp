// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

struct MyClass
{
	float myFloat = 0.0f;

	bool IsZero() { return myFloat == 0.0f; }
	bool Equals(const MyClass& _rhs) { return myFloat == _rhs.myFloat; }

	int Add(const MyClass& _rhs) { return static_cast<int>(myFloat + _rhs.myFloat); }

	std::string ToString() const { return std::to_string(myFloat); }
};

int main()
{
	SA_UTH_INIT();


	// UTH MF
	MyClass m0;
	MyClass m1{ 4.56f };
	MyClass m2{ 8.15f };

	/// caller, func_name
	SA_UTH_MF(m0, IsZero);
	SA_UTH_MF(m1, IsZero); // Error.

	/// caller, func_name, args1, args2...
	SA_UTH_MF(m0, Equals, m0);
	SA_UTH_MF(m1, Equals, m0); // Error.


	// UTH RMF
	int successRes = 12;
	int failureRes = 5;

	/// caller, func_name, result, args1, args2...
	SA_UTH_RMF(successRes, m1, Add, m2);
	SA_UTH_RMF(failureRes, m1, Add, m2); // Error.


	SA_UTH_EXIT();
}
