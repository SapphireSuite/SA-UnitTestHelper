// Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.. All Rights Reserved.

#include <UnitTestHelper.hpp>
using namespace Sa;

struct MyClass
{
	float myFloat = 0.0f;

	int operator+(const MyClass& _rhs) { return static_cast<int>(myFloat + _rhs.myFloat); }
	bool operator==(const MyClass& _rhs) const { return myFloat == _rhs.myFloat; }

	std::string ToString() const { return std::to_string(myFloat); }
};

int main()
{
	SA_UTH_INIT();


	// UTH OP
	MyClass m1{ 4.56f };
	MyClass m2{ 8.15f };

	/// lhs, operator, rhs
	SA_UTH_OP(m1, ==, m1);
	SA_UTH_OP(m1, == , m2); // Error.


	// UTH ROP
	int successRes = 12;
	int failureRes = 5;

	/// lhs, operator, rhs, result
	SA_UTH_ROP(successRes, m1, +, m2);
	SA_UTH_ROP(failureRes, m1, +, m2); // Error.


	SA_UTH_EXIT();
}
