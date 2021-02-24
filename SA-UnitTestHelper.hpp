// Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_HELPER_GUARD
#define SAPPHIRE_UNIT_TEST_HELPER_GUARD

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
		*	\brief Local exit result from UTH_RUN_TESTS.
		*
		*	UTH::localExit is reset at each UTH_RUN_TESTS call.
		*	UTH::localExit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	localExit 0 == success.
		*	localExit 1 == failure.
		*/
		int localExit = EXIT_SUCCESS;

		/// Quick log macro.
		#define UTH_LOG(_str) std::cout << _str << std::endl;

	#ifndef UTH_EXIT_ON_FAILURE
		/**
		*	\brief Wether to exit program on failure or continue next tests.
		*	Can be defined during compilation.
		*/
		#define UTH_EXIT_ON_FAILURE 0
	#endif

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


		/// \cond UTH_Internal

		/// Internal implementation namespace.
		namespace Internal
		{
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

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			template <typename... Args>
			unsigned int __SizeOfArgs(const Args&... _args)
			{
				return sizeof...(_args);
			}

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			unsigned int __SizeOfArgs()
			{
				return 0u;
			}

			/**
			*	\brief Helper function for recursive call.
			*/
			template <typename... Args>
			std::string __ArgsToString(std::string _argsNames, const Args&... _args)
			{
				std::string res = "\n";

				// Handle empty args methods.
				if constexpr (sizeof...(_args) != 0)
					__ArgsToString(res, _argsNames, _args...);

				return res;
			}

			/**
			*	\brief Recursive method to print each args with their names.
			*/
			template <typename T1, typename... Args>
			void __ArgsToString(std::string& _result, std::string& _argsNames, const T1& _first, const Args&... _args)
			{
				if (Sa::UTH::verbosity & Sa::UTH::ParamsName)
				{
					unsigned int index = _argsNames.find_first_of(',');

					_result += _argsNames.substr(0u, index) + ":\n";
					_argsNames = _argsNames.substr(index + 2);
				}
			
				_result += ToString(_first) + "\n\n";

				// Not out of args to parse.
				if constexpr (sizeof...(_args) != 0)
					__ArgsToString(_result, _argsNames, _args...);
			}

			/**
			*	\brief Helper Equals function.
			*/
			template <typename T>
			bool Equals(const T& _lhs, const T& _rhs)
			{
				return _lhs == _rhs;
			}

			/**
			*	\brief Helper Equals function with epsilon.
			*/
			template <typename T>
			bool Equals(const T& _lhs, const T& _rhs, const T& _epsilon)
			{
				return std::abs(_lhs - _rhs) < _epsilon;
			}

			/**
			*	\brief Helper Equals function for tab.
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
			*	\brief Helper Equals function with epsilon for tab.
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


			/**
			*	\brief Internal implementation of unit test result processing.
			*/
			#define __UTH_TEST_RESULT_INTERNAL()\
				if(!bRes)\
				{\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Title);\
					UTH_LOG(titleStr)\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::None);\
				\
					if(Sa::UTH::verbosity & Sa::UTH::ParamsFailure)\
						std::cout << paramStr;\
				\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Failure);\
					UTH_LOG("Failure\n\n")\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::None);\
				\
					if constexpr(UTH_EXIT_ON_FAILURE)\
						exit(EXIT_FAILURE);\
					else\
						Sa::UTH::exit = Sa::UTH::localExit = EXIT_FAILURE;\
				}\
				else if(Sa::UTH::verbosity & Sa::UTH::Success)\
				{\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Title);\
					UTH_LOG(titleStr)\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::None);\
				\
					if(Sa::UTH::verbosity & UTH::ParamsSuccess)\
						std::cout << paramStr;\
				\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::Success);\
					UTH_LOG("Success\n\n")\
					Sa::UTH::Internal::SetConsoleColor(Sa::UTH::Internal::CslColor::None);\
				}

			#define __UTH_TITLE "[SA-UTH] Test:\t"
			#define __UTH_LINE_STR  " -- l:" +  std::to_string(__LINE__)
		}

		/// \endcond


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
			std::string titleStr = __UTH_TITLE "Sa::UTH::Equals(" #_lhs ", " #_rhs;\
			titleStr += Sa::UTH::Internal::__SizeOfArgs(__VA_ARGS__) ? ", " #__VA_ARGS__ : "";\
			titleStr += + ")" __UTH_LINE_STR;\
			std::string paramStr = Sa::UTH::Internal::__ArgsToString(#_lhs ", " #_rhs ", " #__VA_ARGS__, _lhs, _rhs, __VA_ARGS__);\
		\
			bool bRes = Sa::UTH::Internal::Equals(_lhs, _rhs, __VA_ARGS__);\
			__UTH_TEST_RESULT_INTERNAL()\
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
			std::string titleStr = __UTH_TITLE #_caller "." #_func "(" #__VA_ARGS__ ")" __UTH_LINE_STR;\
			std::string paramStr = Sa::UTH::Internal::__ArgsToString(#_caller ", " #__VA_ARGS__, _caller, __VA_ARGS__);\
		\
			bool bRes = _caller._func(__VA_ARGS__);\
			__UTH_TEST_RESULT_INTERNAL()\
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
			std::string titleStr = __UTH_TITLE #_func "(" #__VA_ARGS__ ")" __UTH_LINE_STR;\
			std::string paramStr = Sa::UTH::Internal::__ArgsToString(#__VA_ARGS__, __VA_ARGS__);\
		\
			bool bRes = _func(__VA_ARGS__);\
			__UTH_TEST_RESULT_INTERNAL()\
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
			std::string titleStr = __UTH_TITLE #_lhs " " #_op " " #_rhs __UTH_LINE_STR;\
			std::string paramStr = Sa::UTH::Internal::__ArgsToString(#_lhs ", " #_rhs, _lhs, _rhs);\
		\
			bool bRes = _lhs _op _rhs;\
			__UTH_TEST_RESULT_INTERNAL()\
		}


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
