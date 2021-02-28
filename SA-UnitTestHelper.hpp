// Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_HELPER_GUARD
#define SAPPHIRE_UNIT_TEST_HELPER_GUARD

#include <stack>
#include <vector>
#include <string>
#include <iostream>

#if _WIN32

#include <Windows.h>

#endif

/**
*	\file SA-UnitTestHelper.hpp
*
*	\brief Single header file helper for unit testing.
*/

namespace Sa
{
	/// UnitTestHelper global namespace.
	namespace UTH
	{
#pragma region Log

		/// Quick log macro.
		#define SA_UTH_LOG(_str) std::cout << _str << std::endl;

	#ifndef SA_UTH_DEFAULT_CSL_LOG
		/**
		*	\brief Wether to log tests in console by default.
		*	Can be defined within cmake options or before including the header.
		*/
		#define SA_UTH_DEFAULT_CSL_LOG 0
	#endif

		/// Dynamic console log toogle.
		bool bCslLog = SA_UTH_DEFAULT_CSL_LOG;

#pragma endregion


#pragma region Init / Exit

		/**
		*	\brief Init function to be called at the start of main.
		*	Use SA_UTH_INIT() as helper macro.
		*/
		void Init()
		{
			// Init rand.
			srand(time(NULL));
		}

		/**
		*	\brief Helper init program macro.
		*	Should be used at the start of main.
		*/
		#define SA_UTH_INIT() Sa::UTH::Init();


	#ifndef SA_UTH_EXIT_ON_FAILURE
		/**
		*	\brief Wether to exit program on failure or continue next tests.
		*	Can be defined within cmake options or before including the header.
		*/
		#define SA_UTH_EXIT_ON_FAILURE 0
	#endif

		/**
		*	\brief Exit result from unit testing.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	exit 0 == success.
		*	exit 1 == failure.
		*/
		int exit = EXIT_SUCCESS;

		/**
		*	\brief Exit function to be called at the end of main.
		*	Use SA_UTH_EXIT() as helper macro.
		* 
		*	\return exit code of all tests run.
		*/
		int Exit()
		{
			return exit;
		}

		/**
		*	\brief Helper exit program macro.
		*	Should be used at the end of main.
		*/
		#define SA_UTH_EXIT() return Sa::UTH::Exit();

#pragma endregion


#pragma region Verbosity

		/// Verbosity level enum.
		enum Verbosity
		{
			/// No verbosity.
			None,

			/// Output success results.
			Success = 1 << 0,

			/// Output params' name.
			ParamsName = 1 << 1,

			/// Output params' value on failure.
			ParamsFailure = 1 << 2,

			/// Output params' value on success.
			ParamsSuccess = 1 << 3,

			/// Maximum verbosity level (all flags set).
			Max = 0xFF
		};

		/// Current verbosity level.
		unsigned int verbosity = Success | ParamsName | ParamsFailure;

#pragma endregion

		
		/// Pair of param name and value.
		struct Param
		{
			/// Param's name.
			std::string name;

			/// Param's value as a string.
			std::string value;
		};


		/// Infos generated from a group of tests.
		struct Group
		{
			/// Name of the group.
			const std::string name;

			/**
			*	\brief Local exit from tests of the group.
			* 
			*	UTH::localExit will be equal to EXIT_FAILURE (1) if at least one test of the group failed.
			*
			*	localExit 0 == success.
			*	localExit 1 == failure.
			*/
			bool localExit = EXIT_SUCCESS;
		};


#pragma region Callback

		/// Pointer to allow user to get custom data in callbacks.
		void* UserData = nullptr;

		/**
		*	\brief Helper get user data with type.
		*
		*	\tparam T	The type to cast user data.
		*
		*	\return User data casted as T.
		*/
		template<typename T>
		T& GetUserData() { return *reinterpret_cast<T*>(UserData); }

		/// Callback called on groupe begin.
		void (*GroupBeginCB)(const std::string& _name) = nullptr;
		
		/// Callback called on groupe end.
		void (*GroupEndCB)(const Group& _group) = nullptr;
		
		/// Callback called on test's title output.
		void (*TitleCB)(const std::string& _funcDecl, unsigned int _lineNum) = nullptr;
		
		/// Callback called on test's parameters output.
		void (*ParamsCB)(const std::vector<Param>& _params) = nullptr;
		
		/// Callback called on test's result output.
		void (*ResultCB)(bool _pred) = nullptr;

#pragma endregion


#pragma region ToString

		/**
		*	\brief ToString implementation used to print elem during unit testing.
		*
		*	Default implementation is _elem.ToString(). Define template specialization for custom implementation.
		*
		*	\tparam T			Type of element.
		*	\param[in] _elem	Element to convert to string.
		*
		*	\return	std::string from converted element.
		*/
		template <typename T>
		std::string ToString(const T& _elem)
		{
			if constexpr (std::is_arithmetic<T>::value)
				return std::to_string(_elem);
			else
				return _elem.ToString();
		}

		/**
		*	\brief ToString implementation used to print tab of elems during unit testing.
		*
		*	Default implementation is for loop _elem.ToString(). Define template specialization for custom implementation.
		*
		*	\tparam T			Type of element.
		*	\tparam size		Size of elem tab.
		*	\param[in] _elems	Element to convert to string.
		*
		*	\return	std::string from converted element.
		*/
		template <typename T, unsigned int size>
		std::string ToString(const T(&_elems)[size])
		{
			std::string res = "{ ";

			for (unsigned int i = 0; i < size; ++i)
				res += ToString(_elems[i]) + ", ";

			res[res.size() - 2] = ' ';
			res[res.size() - 1] = '}';

			return res;
		}

#pragma endregion


#pragma region Equals

		/**
		*	\brief Helper Equals function.
		*
		*	\tparam T		Type of operands.
		*
		*	\param[in] _lhs		Left hand side operand to compare.
		*	\param[in] _rhs		Right hand side operand to compare.
		* 
		*	\return	True on equality, otherwise false.
		*/
		template <typename T>
		bool Equals(const T& _lhs, const T& _rhs)
		{
			return _lhs == _rhs;
		}

		/**
		*	\brief Helper Equals function using epsilon.
		*
		*	\tparam T		Type of operands.
		*
		*	\param[in] _lhs		Left hand side operand to compare.
		*	\param[in] _rhs		Right hand side operand to compare.
		*	\param[in] _epsilon	Epsilon value for threshold compare.
		* 
		*	\return	True on equality, otherwise false.
		*/
		template <typename T>
		bool Equals(const T& _lhs, const T& _rhs, const T& _epsilon)
		{
			return std::abs(_lhs - _rhs) < _epsilon;
		}

		/**
		*	\brief Helper Equals function for tab.
		*
		*	\tparam T		Type of operands.
		*
		*	\param[in] _lhs		Left hand side operand to compare.
		*	\param[in] _rhs		Right hand side operand to compare.
		*	\param[in] _size	Size of tabs to compare compare.
		* 
		*	\return	True on equality, otherwise false.
		*/
		template <typename T>
		bool Equals(const T* _lhs, const T* _rhs, unsigned int _size)
		{
			for (unsigned int i = 0; i < _size; ++i)
			{
				if (!Equals(_lhs[i], _rhs[i]))
					return false;
			}

			return true;
		}

		/**
		*	\brief Helper Equals function for tab using epsilon.
		*
		*	\tparam T		Type of operands.
		*
		*	\param[in] _lhs		Left hand side operand to compare.
		*	\param[in] _rhs		Right hand side operand to compare.
		*	\param[in] _size	Size of tabs to compare compare.
		*	\param[in] _epsilon	Epsilon value for threshold compare.
		* 
		*	\return	True on equality, otherwise false.
		*/
		template <typename T>
		bool Equals(const T* _lhs, const T* _rhs, unsigned int _size, const T& _epsilon)
		{
			for (unsigned int i = 0; i < _size; ++i)
			{
				if (!Equals(_lhs[i], _rhs[i], _epsilon))
					return false;
			}

			return true;
		}

#pragma endregion

#pragma region Random

		/**
		*	\brief Rand between [min, max[ (max excluded).
		* 
		*	\tparam T			Type of the rand.
		*	\param[in] _min		Min bound for rand (included).
		*	\param[in] _max		Max bound for rand (excluded).
		* 
		*	\return Random T between [min, max[ (max excluded).
		*/
		template <typename T>
		T Rand(T _min = T(0), T _max = T(1))
		{
			return _min + static_cast<T>(rand()) / static_cast<T>(RAND_MAX/(_max - _min));
		}

#pragma endregion


#pragma region Internal

		/// \cond Internal

		/// Internal implementation namespace.
		namespace Internal
		{
#pragma region Log

			/// enum for console colors.
			enum class CslColor
			{
				/// Default color.
				None,

				/// Color used for test's titles.
				Title,

				/// Color used on test success.
				Success,

				/// Color used on test failure.
				Failure,

				/// Color used for group begin.
				GroupBegin,

				/// Color used for group begin.
				GroupEnd,
			};

		#if _WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			void SetConsoleColor(CslColor _result)
			{
				switch (_result)
				{
					case CslColor::None:
						SetConsoleTextAttribute(hConsole, 15);
						break;
					case CslColor::Title:
						SetConsoleTextAttribute(hConsole, 14);
						break;
					case CslColor::Success:
						SetConsoleTextAttribute(hConsole, 10);
						break;
					case CslColor::Failure:
						SetConsoleTextAttribute(hConsole, 12);
						break;
					case CslColor::GroupBegin:
						SetConsoleTextAttribute(hConsole, 3);
						break;
					case CslColor::GroupEnd:
						SetConsoleTextAttribute(hConsole, 3);
						break;
					default:
						SA_UTH_LOG("CslColor not supported yet!");
						break;
				}
			}
		#else
			void SetConsoleColor(CslColor _result)
			{
				(void)_result;
			}
		#endif

			/**
			*	\brief GroupBegin output in console.
			*
			*	\param[in] _name	The name of the group that begins.
			*/
			void GroupBeginCslLog(const std::string& _name)
			{
				SetConsoleColor(CslColor::GroupBegin);
				SA_UTH_LOG("=== Start " << _name << " ===\n");
				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief GroupEnd output in console.
			*
			*	\param[in] _group	The group that ends.
			*/
			void GroupEndCslLog(const Group& _group)
			{
				SetConsoleColor(CslColor::GroupEnd);
				std::cout << "=== End " << _group.name << " exit with code: ";

				if (_group.localExit == EXIT_SUCCESS)
				{
					SetConsoleColor(CslColor::Success);
					std::cout << "EXIT_SUCCESS (" << EXIT_SUCCESS << ')';
				}
				else
				{
					SetConsoleColor(CslColor::Failure);
					std::cout << "EXIT_FAILURE (" << EXIT_FAILURE << ')';
				}

				SetConsoleColor(CslColor::GroupEnd);
				SA_UTH_LOG(" ===\n\n");
				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief Test title output in console.
			*
			*	\param[in] _funcDecl	Declaration of the function as a string.
			*	\param[in] _lineNum		Line number of the function's call.
			*/
			void TitleCslLog(const std::string& _funcDecl, unsigned int _lineNum)
			{
				SetConsoleColor(CslColor::Title);

				SA_UTH_LOG("[SA-UTH] Test:\t" << _funcDecl << " -- l:" << _lineNum << '\n');

				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief Test parameters output in console.
			*
			*	\param[in] _paramStrs	Every param infos extracted from call.
			*/
			void ParamsCslLog(const std::vector<Param>& _params)
			{
				for (auto it = _params.begin(); it != _params.end(); ++it)
					SA_UTH_LOG(it->name << ":\n" << it->value << '\n');
			}

			/**
			*	\brief Test result output in console.
			*
			*	\param[_pred]	Result of the test.
			*/
			void ResultCslLog(bool _pred)
			{
				if (_pred)
				{
					SetConsoleColor(CslColor::Success);

					SA_UTH_LOG("Success\n\n");

					SetConsoleColor(CslColor::None);
				}
				else
				{
					SetConsoleColor(CslColor::Failure);

					SA_UTH_LOG("Failure\n\n");

					SetConsoleColor(CslColor::None);
				}
			}

#pragma endregion


#pragma region Group

			std::stack<Group> groups;

			/// Start a new group of tests.
			void GroupBegin(const std::string& _name)
			{
				groups.push(Group{ _name });

				if (bCslLog)
					GroupBeginCslLog(_name);

				if(GroupBeginCB)
					GroupBeginCB(_name);
			}

			/// End a group of tests.
			Group GroupEnd()
			{
				Group group = groups.top();
				groups.pop();

				if (bCslLog)
					GroupEndCslLog(group);

				if(GroupEndCB)
					GroupEndCB(group);

				return group;
			}

			/// Update current group from test result.
			void GroupUpdate(bool _pred)
			{
				if (!groups.empty())
				{
					if (!_pred)
						groups.top().localExit = EXIT_FAILURE;
				}
			}

#pragma endregion


#pragma region ComputeStr

			/// Compute title from function declaration and line num.
			void ComputeTitleStr(const std::string& _funcDecl, unsigned int _lineNum)
			{
				if (bCslLog)
					TitleCslLog(_funcDecl, _lineNum);

				if(TitleCB)
					TitleCB(_funcDecl, _lineNum);
			}


			/// Compute params.
			template <typename... Args>
			void ComputeParamStr(bool _pred, std::string _paramNames, const Args&... _args)
			{
				// No need to compute params.
				if (!bCslLog && !ParamsCB)
					return;

				if (_pred && (verbosity & ParamsSuccess) ||		// Should output params on success.
					!_pred && (verbosity & ParamsFailure))		// Should output params on failure.
				{
					std::vector<Param> params;
					GenerateParamStr(params, _paramNames, _args...);

					if (bCslLog)
						ParamsCslLog(params);

					if(ParamsCB)
						ParamsCB(params);
				}
			}

			/// \brief Generate Params from params' names and values.
			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<Param>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args)
			{
				unsigned int index = _paramNames.find_first_of(',');

				_result.push_back(Param{ _paramNames.substr(0u, index), ToString(_first) });

				if constexpr (sizeof...(_args) != 0)
					GenerateParamStr(_result, _paramNames.substr(index + 2), _args...);
			}


			/// Compute the result using _pred predicate.
			void ComputeResult(bool _pred)
			{
				if(_pred)
					Sa::UTH::exit = EXIT_SUCCESS;
				else
					Sa::UTH::exit = EXIT_FAILURE;

				if (bCslLog)
					ResultCslLog(_pred);

				if(ResultCB)
					ResultCB(_pred);

#if SA_UTH_EXIT_ON_FAILURE
				if(!_pred)
					::exit(EXIT_FAILURE);
#endif
			}

#pragma endregion


#pragma region Misc

			/// Wether to continue computing test with predicate _pred.
			bool ShouldComputeTest(bool _pred)
			{
				return !_pred || (verbosity & Verbosity::Success);
			}

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			template <typename... Args>
			unsigned int SizeOfArgs(const Args&... _args)
			{
				return sizeof...(_args);
			}

#pragma endregion
		}
		
		/// \endcond

#pragma endregion

#pragma region Macro

		/**
		*	\brief Run a \e <b> Unit Test </b> using internal Equals implementation.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _lhs		Left hand side operand to test.
		*	\param[in] _rhs		Right hand side operand to test.
		*
		*	Additionnal params: 
		*	uint32 size:		Size lenght to compare _lhs and _rhs are T*
		*	T epsilon			Epsilon value to allow threshold.
		*/
		#define SA_UTH_EQUALS(_lhs, _rhs, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = UTH::Equals(_lhs, _rhs, __VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				std::string titleStr = std::string("Sa::UTH::Equals(" #_lhs ", " #_rhs) + (SizeOfArgs(__VA_ARGS__) ? ", " #__VA_ARGS__ ")" : ")");\
			\
				ComputeTitleStr(titleStr, __LINE__);\
				ComputeParamStr(bRes, #_lhs ", " #_rhs ", " #__VA_ARGS__, _lhs, _rhs, __VA_ARGS__);\
				ComputeResult(bRes);\
			}\
		}


		/**
		*	\brief Run a \e <b> Unit Test </b> using a static function.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _func	Function to test with ... args.
		*/
		#define SA_UTH_SFUNC(_func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _func(__VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitleStr(#_func "(" #__VA_ARGS__ ")", __LINE__);\
				ComputeParamStr(bRes, #__VA_ARGS__, __VA_ARGS__);\
				ComputeResult(bRes);\
			}\
		}


		/**
		*	\brief Run a \e <b> Unit Test </b> using a member function.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _caller	caller of the functin _func.
		*	\param[in] _func	Function to test with ... args.
		*/
		#define SA_UTH_MFUNC(_caller, _func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _caller._func(__VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitleStr(#_caller "." #_func "(" #__VA_ARGS__ ")", __LINE__);\
				ComputeParamStr(bRes, #_caller ", " #__VA_ARGS__, _caller, __VA_ARGS__);\
				ComputeResult(bRes);\
			}\
		}


		/**
		*	\brief Run a \e <b> Unit Test </b> using an operator.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _lhs		Left hand side operand to test.
		*	\param[in] _op		Operator of the test between _lhs and _rhs.
		*	\param[in] _rhs		Right hand side operand to test.
		*/
		#define SA_UTH_OP(_lhs, _op, _rhs)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _lhs _op _rhs;\
			GroupUpdate(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitleStr(#_lhs " " #_op " " #_rhs, __LINE__);\
				ComputeParamStr(bRes, #_lhs ", " #_rhs, _lhs, _rhs);\
				ComputeResult(bRes);\
			}\
		}


		/// Begin a group of tests.
		#define SA_UTH_GROUP_BEGIN(_name) Sa::UTH::Internal::GroupBegin(#_name);

		/// End a group of tests.
		#define SA_UTH_GROUP_END() Sa::UTH::Internal::GroupEnd();


		/**
		*	\brief Run a group of tests from a single function.
		*
		*	\param[in] _func	The function which own the group of tests.
		*/
		#define SA_UTH_GROUP(_func)\
		{\
			SA_UTH_GROUP_BEGIN(_func)\
			_func;\
			SA_UTH_GROUP_END()\
		}

#pragma endregion
	}
}

#endif // GUARD
