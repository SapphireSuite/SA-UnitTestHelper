// Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.. All Rights Reserved.

#include <UnitTestHelper.hpp>
using namespace Sa;

struct Vec2
{
	float x = 0.0f;
	float y = 0.0f;

	bool IsEqual(const Vec2& _other) const
	{
		return x == _other.x && y == _other.y;
	}
	static bool Equals(const Vec2& _lhs, const Vec2& _rhs)
	{
		return _lhs.x == _rhs.x && _lhs.y == _rhs.y;
	}

	Vec2 Add(const Vec2& _other) const
	{
		return Vec2{ x + _other.x, y + _other.y };
	}

	Vec2 operator+(const Vec2& _other) const
	{
		return Add(_other);
	}

	bool operator==(const Vec2& _rhs) const noexcept
	{
		return IsEqual(_rhs);
	}
};

// Custom definition of UTH::ToString because Vec2 does not define a ToString() method.
template <>
std::string UTH::ToString(const Vec2& _elem)
{
	std::string res;

	res = "X: " + std::to_string(_elem.x) + "\tY: " + std::to_string(_elem.y);

	return res;
}

bool GlobalValidate(bool _pred)
{
	return _pred;
}

int GlobalAdd(int _i, int _j)
{
	return _i + _j;
}


/// Methods with all the tests (can be in a separated file).
void MainTests()
{
	// No output on success.
	UTH::verbosity = UTH::None;

	SA_UTH_SF(GlobalValidate, true);


	// Print tests on success.
	UTH::verbosity = UTH::Success;

	SA_UTH_RSF(12, GlobalAdd, 8, 4);


	// Output param's value.
	UTH::verbosity |= UTH::ParamsSuccess;


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
	SA_UTH_OP(v1, ==, v2);

	SA_UTH_RMF(v1v2, v1, Add, v2);
	SA_UTH_ROP(v1v2, v1, +, v2);
}

int main()
{
	SA_UTH_INIT();


	int aa1 = 1;
	float f1 = 1.0f;
	
	std::cout << "HELLO 0" << std::endl;
	
	SA_UTH_EQ(aa1, static_cast<int>(f1));
	
	std::cout << "HELLO 1" << std::endl;

	SA_UTH_GP(MainTests());


	SA_UTH_EXIT();
}
