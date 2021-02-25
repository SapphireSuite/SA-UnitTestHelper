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
		/// Quick log macro.
		#define UTH_LOG(_str) std::cout << _str << std::endl;

	#ifndef UTH_EXIT_ON_FAILURE
		/**
		*	\brief Wether to exit program on failure or continue next tests.
		*	Can be defined during compilation.
		*/
		#define UTH_EXIT_ON_FAILURE 0
	#endif


#pragma region Exit

		/**
		*	\brief Exit result from unit testing.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	exit 0 == success.
		*	exit 1 == failure.
		*/
		int exit = EXIT_SUCCESS;

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

#ifndef DOXYGEN_SKIP

		namespace Internal
		{
			void DefaultGroupBeginCB(const std::string& _name);
			void DefaultGroupEndCB(const Group& _group);
			void DefaultTitleCB(const std::string& _funcDecl, unsigned int _lineNum);
			void DefaultParamsCB(const std::vector<Param>& _params);
			void DefaultResultCB(bool _pred);
		}

#endif // DOXYGEN_SKIP

		/// Pointer to allow user to get custom data in callbacks.
		void* UserData = nullptr;

		/// Callback called on groupe begin.
		void (*GroupBeginCB)(const std::string& _name) = Internal::DefaultGroupBeginCB;
		
		/// Callback called on groupe end.
		void (*GroupEndCB)(const Group& _group) = Internal::DefaultGroupEndCB;
		
		/// Callback called on test's title output.
		void (*TitleCB)(const std::string& _funcDecl, unsigned int _lineNum) = Internal::DefaultTitleCB;
		
		/// Callback called on test's parameters output.
		void (*ParamsCB)(const std::vector<Param>& _params) = Internal::DefaultParamsCB;
		
		/// Callback called on test's result output.
		void (*ResultCB)(bool _pred) = Internal::DefaultResultCB;

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


#pragma region Internal

		/// \cond Internal

		/// Internal implementation namespace.
		namespace Internal
		{
#pragma region Group

			std::stack<Group> groups;

			/// Start a new group of tests.
			void GroupBegin(const std::string& _name)
			{
				groups.push(Group{ _name });

				GroupBeginCB(_name);
			}

			/// End a group of tests.
			Group GroupEnd()
			{
				Group group = groups.top();
				groups.pop();

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
				TitleCB(_funcDecl, _lineNum);
			}


			/// Compute params.
			template <typename... Args>
			void ComputeParamStr(bool _pred, std::string _paramNames, const Args&... _args)
			{
				if (_pred && (verbosity & ParamsSuccess) ||		// Should output params on success.
					!_pred && (verbosity & ParamsFailure))		// Should output params on failure.
				{
					std::vector<Param> params;
					GenerateParamStr(params, _paramNames, _args...);

					ParamsCB(params);
				}
			}

			/// \brief Generate Params from params' names and values.
			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<Param>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args)
			{
				unsigned int index = _paramNames.find_first_of(',');

				_result.push_back(Param{ _paramNames.substr(0u, index), ToString(_first) });

				if constexpr (sizeof...(_args))
					GenerateParamStr(_result, _paramNames.substr(index + 2), _args...);
			}


			/// Compute the result using _pred predicate.
			void ComputeResult(bool _pred)
			{
				if(_pred)
					Sa::UTH::exit = EXIT_SUCCESS;
				else
					Sa::UTH::exit = EXIT_FAILURE;

				ResultCB(_pred);

#if UTH_EXIT_ON_FAILURE
				if(!_pred)
					::exit(EXIT_FAILURE);
#endif
			}

#pragma endregion


#pragma region Misc

			/// Wether to continue output test with predicate _pred.
			bool ShouldOutputTest(bool _pred)
			{
				return !_pred || (verbosity & Verbosity::Success);
			}

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			template <typename... Args>
			unsigned int SizeOfArgs(const Args&... _args)
			{
				return sizeof...(_args);
			}


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
				Failure
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
					default:
						UTH_LOG("CslColor not supported yet!");
						break;
				}
			}
		#else
			void SetConsoleColor(CslColor _result)
			{
				(void)_result;
			}
		#endif

#pragma endregion

			
#pragma region Callbacks

			/**
			*	\brief Default implementation of GroupBegin callback.
			*	Log in console.
			*
			*	\param[in] _name	The name of the group that begins.
			*/
			void DefaultGroupBeginCB(const std::string& _name)
			{
				UTH_LOG("=== Start " << _name << " ===\n");
			}

			/**
			*	\brief Default implementation of GroupEnd callback.
			*	Log in console.
			*
			*	\param[in] _group	The group that ends.
			*/
			void DefaultGroupEndCB(const Group& _group)
			{
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

				SetConsoleColor(CslColor::None);
				UTH_LOG(" ===\n\n");
			}

			/**
			*	\brief Default implementation of <b>title output</b> callback.
			*	Log in console.
			*
			*	\param[in] _funcDecl	Declaration of the function as a string.
			*	\param[in] _lineNum		Line number of the function's call.
			*/
			void DefaultTitleCB(const std::string& _funcDecl, unsigned int _lineNum)
			{
				SetConsoleColor(CslColor::Title);

				UTH_LOG("[SA-UTH] Test:\t" << _funcDecl << " -- l:" << _lineNum << '\n');
				
				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief Default implementation of <b>parameters output</b> callback.
			*	Log in console.
			*
			*	\param[in] _paramStrs	Every param infos extracted from call.
			*/
			void DefaultParamsCB(const std::vector<Param>& _params)
			{
				for (auto it = _params.begin(); it != _params.end(); ++it)
					UTH_LOG(it->name << ":\n" << it->value << '\n');
			}

			/**
			*	\brief Default implementation of <b>result output</b> callback.
			*	Log in console.
			*
			*	\param[_pred]	Result of the test.
			*/
			void DefaultResultCB(bool _pred)
			{
				if (_pred)
				{
					SetConsoleColor(CslColor::Success);

					UTH_LOG("Success\n\n");

					SetConsoleColor(CslColor::None);
				}
				else
				{
					SetConsoleColor(CslColor::Failure);
					
					UTH_LOG("Failure\n\n");
					
					SetConsoleColor(CslColor::None);
				}
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
		#define UTH_EQUALS_TEST(_lhs, _rhs, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = UTH::Equals(_lhs, _rhs, __VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldOutputTest(bRes))\
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
		#define UTH_SFUNC_TEST(_func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _func(__VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldOutputTest(bRes))\
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
		#define UTH_MFUNC_TEST(_caller, _func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _caller._func(__VA_ARGS__);\
			GroupUpdate(bRes);\
		\
			if(ShouldOutputTest(bRes))\
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
		#define UTH_OP_TEST(_lhs, _op, _rhs)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _lhs _op _rhs;\
			GroupUpdate(bRes);\
		\
			if(ShouldOutputTest(bRes))\
			{\
				ComputeTitleStr(#_lhs " " #_op " " #_rhs, __LINE__);\
				ComputeParamStr(bRes, #_lhs ", " #_rhs, _lhs, _rhs);\
				ComputeResult(bRes);\
			}\
		}


		/// Begin a group of tests.
		#define UTH_GROUP_BEGIN(_name) Sa::UTH::Internal::GroupBegin(#_name);

		/// End a group of tests.
		#define UTH_GROUP_END() Sa::UTH::Internal::GroupEnd();


		/**
		*	\brief Run a group of tests from a single function.
		*
		*	\param[in] _func	The function which own the group of tests.
		*/
		#define UTH_GROUP_TESTS(_func)\
		{\
			UTH_GROUP_BEGIN(_func)\
			_func;\
			UTH_GROUP_END()\
		}

#pragma endregion
	}
}

#endif // GUARD
