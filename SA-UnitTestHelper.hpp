// Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_HELPER_GUARD
#define SAPPHIRE_UNIT_TEST_HELPER_GUARD

#include <string>
#include <vector>
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

	#ifndef SA_UTH_EXIT_ON_FAILURE
		/**
		*	\brief Wether to exit program on failure or continue next tests.
		*	Can be defined during compilation.
		*/
		#define SA_UTH_EXIT_ON_FAILURE 0
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

		///**
		//*	\brief Local exit result from UTH_RUN_TESTS.
		//*
		//*	UTH::localExit is reset at each UTH_RUN_TESTS call.
		//*	UTH::localExit will be equal to EXIT_FAILURE (1) if at least one test failed.
		//*
		//*	localExit 0 == success.
		//*	localExit 1 == failure.
		//*/
		int localExit = EXIT_SUCCESS;

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
		struct ParamStr
		{
			/// Param's name.
			std::string name;

			/// Param's value as a string.
			std::string value;
		};


#pragma region Callback

		namespace Internal
		{
			void DefaultTitleCB(const std::string& _funcDecl, unsigned int _lineNum);
			void DefaultParamCB(const std::vector<ParamStr>& _paramStrs);
			void DefaultResultCB(bool _pred);
		}

		void (*TitleCB)(const std::string& _funcDecl, unsigned int _lineNum) = Internal::DefaultTitleCB;
		void (*ParamCB)(const std::vector<ParamStr>& _paramStrs) = Internal::DefaultParamCB;
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

		/// \brief Helper Equals function.
		template <typename T>
		bool Equals(const T& _lhs, const T& _rhs)
		{
			return _lhs == _rhs;
		}

		/// \brief Helper Equals function with epsilon.
		template <typename T>
		bool Equals(const T& _lhs, const T& _rhs, const T& _epsilon)
		{
			return std::abs(_lhs - _rhs) < _epsilon;
		}

		/// \brief Helper Equals function for tab.
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

		/// \brief Helper Equals function with epsilon for tab.
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
			/// Wether to continue output test with predicate _pred.
			bool ShouldOutputTest(bool _pred)
			{
				return !_pred || (verbosity & Verbosity::Success);
			}


			/// Compute title from function declaration and line num.
			void ComputeTitleStr(const std::string& _funcDecl, unsigned int _lineNum)
			{
				TitleCB(_funcDecl, _lineNum);
			}


			/// Compute params.
			template <typename... Args>
			void ComputeParamStr(bool _pred, std::string _paramNames, const Args&... _args)
			{
				if ((_pred && (verbosity & Success) && (verbosity & ParamsSuccess)) ||	// Should output params on success.
					(verbosity & ParamsFailure))										// Should output params on failure.

				{
					std::vector<ParamStr> paramStrs;
					GenerateParamStr(paramStrs, _paramNames, _args...);

					ParamCB(paramStrs);
				}
			}

			/// \brief Generate ParamStrs from params' names and values.
			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<ParamStr>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args)
			{
				unsigned int index = _paramNames.find_first_of(',');

				_result.push_back(ParamStr{ _paramNames.substr(0u, index), ToString(_first) });

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

#if SA_UTH_EXIT_ON_FAILURE
				if(!_pred)
					exit(EXIT_FAILURE);
#endif
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
			

			void DefaultTitleCB(const std::string& _funcDecl, unsigned int _lineNum)
			{
				SetConsoleColor(CslColor::Title);

				UTH_LOG("[SA-UTH] Test:\t" << _funcDecl << " -- l:" << _lineNum << '\n');
				
				SetConsoleColor(CslColor::None);
			}

			void DefaultParamCB(const std::vector<ParamStr>& _paramStrs)
			{
				for (auto it = _paramStrs.begin(); it != _paramStrs.end(); ++it)
					UTH_LOG(it->name << ":\n" << it->value << '\n');
			}

			void DefaultResultCB(bool _predicate)
			{
				if (_predicate)
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
			bool bRes = UTH::Equals(_lhs, _rhs, __VA_ARGS__);\
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
			bool bRes = _func(__VA_ARGS__);\
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
			bool bRes = _caller._func(__VA_ARGS__);\
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
			bool bRes = _lhs _op _rhs;\
		\
			if(ShouldOutputTest(bRes))\
			{\
				ComputeTitleStr(#_lhs " " #_op " " #_rhs, __LINE__);\
				ComputeParamStr(bRes, #_lhs ", " #_rhs, _lhs, _rhs);\
				ComputeResult(bRes);\
			}\
		}

#pragma endregion




















		/**
		*	\brief Run tests grouped in a single function.
		*
		*	\param[in] _func	The function which own the group of tests.
		*/
		#define UTH_RUN_TESTS(_func)\
		{\
			UTH_LOG("=== Start " #_func " ===");\
			Sa::UTH::localExit = EXIT_SUCCESS;\
			_func;\
		\
			std::cout << "=== End " #_func " exit with code: ";\
			if(Sa::UTH::localExit == EXIT_SUCCESS)\
			{\
				Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Success);\
				std::cout << "EXIT_SUCCESS (" << EXIT_SUCCESS << ')';\
			}\
			else\
			{\
				Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Failure);\
				std::cout << "EXIT_FAILURE (" << EXIT_FAILURE << ')';\
			}\
			Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::None);\
			UTH_LOG(" ===\n\n");\
		}
	}
}

#endif // GUARD
