// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

struct MyClass
{
	float myFloat = 0.0f;

	std::string ToString() const { return std::to_string(myFloat); }

	bool operator==(const MyClass& _rhs) const { return myFloat == _rhs.myFloat; }
};

int main()
{
	SA_UTH_INIT();


	// Single elem
	float i = 4.6f;
	float j = 1.25f;

	/// elem1, elem2, epsilon.
	SA_UTH_EQ(i, i);
	SA_UTH_EQ(i, j, FLT_EPSILON); // Error


	// Tab elems.
	int size = 3;
	float ftab1[] = { 1.45f, 8.36f, 1.247f };
	float ftab2[] = { 1.45f, 8.36f, 945.9f };

	/// elem1, elem2, size, epsilon.
	SA_UTH_EQ(ftab1, ftab2, 2, FLT_EPSILON);
	SA_UTH_EQ(ftab1, ftab2, size); // Error


	// Custom elem
	MyClass m1{ 4.56f };
	MyClass m2{ 8.15f };
	SA_UTH_EQ(m1, m1);
	SA_UTH_EQ(m1, m2); // Error


	SA_UTH_EXIT();
}
