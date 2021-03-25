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

	Vec2 Add(const Vec2& _other) const
	{
		// ERROR.
		return Vec2{ x + _other.x, y + _other.x };
	}

	Vec2 operator+(const Vec2& _other) const
	{
		return Add(_other);
	}

	bool operator==(const Vec2& _rhs) const noexcept
	{
		return IsEqual(_rhs);
	}

	// In-class ToString definition
	std::string ToString() const noexcept
	{
		std::string res;

		res = "X: " + std::to_string(x) + "\tY: " + std::to_string(y);

		return res;
	}
};

bool GlobalValidate(bool _pred)
{
	return _pred;
}

int GlobalAdd(int _i, int _j)
{
	(void)_j;

	// ERROR.
	return _i + _i;
}


/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	// No output on success.
	UTH::verbosity = UTH::None;

	SA_UTH_SF(GlobalValidate, true);
	SA_UTH_SF(GlobalValidate, false); // ERROR.


	// Print tests on success.
	UTH::verbosity = UTH::Success;

	SA_UTH_RSF(16, GlobalAdd, 8, 8);
	SA_UTH_RSF(12, GlobalAdd, 8, 4); // ERROR


	// Output param's value.
	UTH::verbosity |= UTH::ParamsFailure;


	// Single method test.
	int i = 4;
	int j = 6;
	int expected_res = 10;
	SA_UTH_RSF(expected_res, GlobalAdd, i, j);


	// Output param's name.
	UTH::verbosity |= UTH::ParamsName;

	SA_UTH_RSF(expected_res, GlobalAdd, i, j);


	// Reset to Default.
	UTH::verbosity = UTH::Default;



	// Vec2 Tests.
	Vec2 v1{ 1.0f, 2.0f };
	Vec2 v2{ 1.0f, 2.0f };
	Vec2 v1v2{ 2.0f, 4.0f };

	SA_UTH_EQ(v1, v2);
	SA_UTH_MF(v1, IsEqual, v2);
	SA_UTH_SF(Vec2::Equals, v1, v2);
	SA_UTH_OP(v1, == , v2);

	SA_UTH_RMF(v1v2, v1, Add, v2);
	SA_UTH_ROP(v1v2, v1, +, v2);
}

int main()
{
	SA_UTH_INIT();


	SA_UTH_GP(MainTests());


	SA_UTH_EXIT();
}
