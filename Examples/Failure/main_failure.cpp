// Copyright 2021 mrouffet. All Rights Reserved.

#include <SA-UnitTestHelper.hpp>
using namespace Sa;

struct Vec2
{
	float x = 0.0f;
	float y = 0.0f;

	bool IsEqual(const Vec2& _other) const
	{
		// ERROR.
		return x == _other.y && y == _other.x;
	}

	static bool Equals(const Vec2& _lhs, const Vec2& _rhs)
	{
		// ERROR.
		return _lhs.x == _rhs.y && _lhs.y == _rhs.x;
	}

	bool operator==(const Vec2& _rhs) const noexcept
	{
		return IsEqual(_rhs);
	}
};

// Custom definition of UTH::ToString because Ex_Vec2 does not define a ToString() method.
template <>
std::string UTH::ToString<Vec2>(const Vec2& _elem)
{
	std::string res;

	res = "X: " + std::to_string(_elem.x) + "\tY: " + std::to_string(_elem.y);

	return res;
}

bool TestMethod(int _i, float _j)
{
	return false;
}


/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	// No verbosity.
	UTH::verbosity = UTH::None;


	UTH_SFUNC_TEST(TestMethod, 8, 3.45f);


	// Output param's value.
	UTH::verbosity = UTH::ParamsFailure;


	// Single method test.
	int i = 4;
	float j = 12.11f;
	UTH_SFUNC_TEST(TestMethod, i, j);


	// Output params' value and name.
	UTH::verbosity |= UTH::ParamsName;


	// Vec2 Tests.
	Vec2 v1{ 1.0f, 2.0f };
	Vec2 v2{ 1.0f, 2.0f };

	UTH_MFUNC_TEST(v1, IsEqual, v2);
	UTH_SFUNC_TEST(Vec2::Equals, v1, v2);
	UTH_OP_TEST(v1, ==, v2);


	// Equals tests.
	// Single elem.
	float f1 = 45.3654f;
	float f2 = 3.4f;
	UTH_EQUALS_TEST(f1, f2);
	UTH_EQUALS_TEST(f1, f2, FLT_EPSILON);

	UTH_EQUALS_TEST(v1, v2);


	// Tab.
	float ftab1[] = { 45.3654f, 983.64f, 1.254f, 4.25f };
	float ftab2[] = { 4.15f, 983.64f, 1.254f, 4.25f };

	UTH_EQUALS_TEST(ftab1, ftab2, 4u);
	UTH_EQUALS_TEST(ftab1, ftab2, 4u, FLT_EPSILON);

	UTH_EQUALS_TEST(ftab1, ftab2, 3u);
}

int main()
{
	UTH_GROUP_TESTS(MainTests());

	return UTH::exit;
}