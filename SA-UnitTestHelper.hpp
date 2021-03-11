// Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_HELPER_GUARD
#define SAPPHIRE_UNIT_TEST_HELPER_GUARD

#include <stack>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem> 

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
#pragma region Types & Vars

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
		inline int exit = EXIT_SUCCESS;


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

			/// Output group start.
			GroupStart = 1 << 4,

			/// Output group exit result.
			GroupExit = 1 << 5,


			/// Light verbosity value.
			Light = ParamsName | ParamsFailure | GroupExit,

			/// Default verbosity value.
			Default = Success | ParamsName | ParamsFailure | GroupStart | GroupExit,

			/// Maximum verbosity level (all flags set).
			Max = 0xFF
		};

		/// Current verbosity level.
		inline unsigned int verbosity = Default;

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

			/// Number of test run in this group.
			unsigned int testNum = 0u;

			/**
			*	\brief Update values from predicate.
			*
			*	\param[in] _pred	predicate from current test.
			*/
			void Update(bool _pred)
			{
				++testNum;

				if (!_pred)
					localExit = EXIT_FAILURE;
			}

			/**
			*	\brief Spreads values to parent group.
			*
			*	\param[in] _parent	parent to spread values to.
			*/
			void Spread(Group& _parent)
			{
				if(localExit == EXIT_FAILURE)
					_parent.localExit = EXIT_FAILURE;

				_parent.testNum += testNum;
			}
		};


	#ifndef SA_UTH_DFLT_CSL_LOG
		/**
		*	\brief Wether to log tests in console by default.
		*	Can be defined within cmake options or before including the header.
		*/
		#define SA_UTH_DFLT_CSL_LOG 0
	#endif

		/// Dynamic console log toogle.
		inline bool bCslLog = SA_UTH_DFLT_CSL_LOG;


	#ifndef SA_UTH_DFLT_FILE_LOG
		/**
		*	\brief Wether to log tests in file by default.
		*	Can be defined within cmake options or before including the header.
		*/
		#define SA_UTH_DFLT_FILE_LOG 0
	#endif

		/// Dynamic file log toogle.
		inline bool bFileLog = SA_UTH_DFLT_FILE_LOG;

		/// \cond Internal

		/// Internal implementation namespace.
		namespace Internal
		{
			inline std::fstream logFile;
			inline std::string logFileName;

			/// Number of test run.
			inline unsigned int testNum = 0u;

			inline std::string GroupTabStr() noexcept;
			inline void LogGroupTabs() noexcept;


			/**
			*	Compile time check type T has member ToString using SFINAE.
			* 
			*	// TODO: Update source with new engine.
			*	Source: https://github.com/sapphire-devteam/Sapphire/blob/dev/Engine/Include/Sapphire/Core/Reflection/HasMember.hpp
			*
			*/
			template <typename T>
			class HM_ToString
			{
				/// SFINAE success type.
				using Yes = char;

				/// SFINAE failure type.
				using No = char[2];

				/// template deduction helper.
				template<typename C, C> struct CheckT;

				/// Introduce Name member.
				struct Fallback
				{
					/// Dummy variable with name Name for SFINAE deduction.
					int ToString;
				};

				/// Merge T and Fallback members.
				struct Derived : T, Fallback { };

				template<typename C, C> struct ChT;

				template <typename C>
				static Yes& SFINAE(ChT<int Fallback::*, &C::ToString>* _c);

				template <typename C>
				static No& SFINAE(...);

			public:
				/// true if T as a public member called Name (ie: SFINAE failure), otherwise false.
				static constexpr bool value = sizeof(SFINAE<Derived>(0)) == sizeof(No);
			};
		}

		/// \endcond

#pragma endregion


#pragma region Log

		/**
		*	\brief UTH log macro
		*
		*	Output with indentation and options bCslLog and bFileLog.
		*/
		#define SA_UTH_LOG(_str)\
		{\
			using namespace Sa::UTH;\
			using namespace Sa::UTH::Internal;\
		\
			LogGroupTabs();\
			if (bCslLog) std::cout << _str << std::endl;\
			if (bFileLog) logFile << _str << std::endl;\
		}

		/// Output only str as input.
		#define __SA_UTH_LOG_IN(_str)\
		{\
			using namespace Sa::UTH;\
			using namespace Sa::UTH::Internal;\
		\
			if (bCslLog) std::cout << _str;\
			if (bFileLog) logFile << _str;\
		}

		/// Ouput only end of line.
		#define __SA_UTH_LOG_ENDL()\
		{\
			using namespace Sa::UTH;\
			using namespace Sa::UTH::Internal;\
		\
			if (bCslLog) std::cout << std::endl;\
			if (bFileLog) logFile << std::endl;\
		}


		/// \cond Internal

		namespace Internal
		{
			/// enum for console colors.
			enum class CslColor
			{
				/// Default color.
				None,

				/// UTH Color for init.
				Init,

				/// UTH Color for exit.
				Exit,

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

				/// Color used for test number.
				TestNum,

				/// Color used for param warning.
				ParamWarning,
			};

		#if _WIN32
			inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			inline void SetConsoleColor(CslColor _result)
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
					case CslColor::TestNum:
						SetConsoleTextAttribute(hConsole, 6);
						break;
					case CslColor::GroupBegin:
						SetConsoleTextAttribute(hConsole, 3);
						break;
					case CslColor::GroupEnd:
						SetConsoleTextAttribute(hConsole, 3);
						break;
					case CslColor::Init:
						SetConsoleTextAttribute(hConsole, 13);
						break;
					case CslColor::Exit:
						SetConsoleTextAttribute(hConsole, 13);
						break;
					case CslColor::ParamWarning:
						SetConsoleTextAttribute(hConsole, 6);
						break;
					default:
						SA_UTH_LOG("CslColor not supported yet!");
						break;
				}
			}
		#else
			inline void SetConsoleColor(CslColor _result)
			{
				(void)_result;
			}
		#endif

			/**
			*	\brief Open the output log file.
			*
			*	\param[in] _time	Time used to name the log file.
			*/
			inline void OpenLogFile(time_t _time)
			{
				std::filesystem::create_directories("Logs");

				struct tm timeinfo;
				localtime_s(&timeinfo, &_time);

				/**
				*	log_backup-<month>.<day>.<year>-<hour>h<minute>m<second>s.txt
				*	Ex: 2/27/2021 at 12:07:43
				*	log_backup-2.27.2021-12h07m43s.txt
				*/
				logFileName = std::string("Logs/log_UTH-") +
					std::to_string(timeinfo.tm_mon + 1) + '.' +
					std::to_string(timeinfo.tm_mday) + '.' +
					std::to_string(timeinfo.tm_year + 1900) + '-' +
					std::to_string(timeinfo.tm_hour) + 'h' +
					std::to_string(timeinfo.tm_min) + 'm' +
					std::to_string(timeinfo.tm_sec) + 's' +
					".txt";

				logFile.open(logFileName, std::ios::out | std::ios::app);
			}

			/**
			*	\brief Close the log file.
			* 
			*	Delete file if empty.
			*/
			inline void CloseLogFile()
			{
				logFile.close();

				// Delete if empty file.
				if(std::filesystem::is_empty(logFileName))
					std::filesystem::remove(logFileName);
			}


			/**
			*	\brief Log enabled and should log.
			* 
			*	\return log toggle.
			*/
			inline bool ShouldLog() noexcept
			{
				return bCslLog || bFileLog;
			}

			/**
			*	\brief Indent in-string \n with group tabs.
			* 
			*	\param[in] _str		String to indent.
			* 
			*	\return	indented string.
			*/
			inline std::string IndentStr(std::string _str)
			{
				const std::string indentStr = std::string("\n") + GroupTabStr();

				size_t index = _str.find('\n');

				while (index != std::string::npos)
				{
					_str.replace(index, 1, indentStr);

					index = _str.find('\n', index + indentStr.size());
				}

				return _str;
			}


			/**
			*	\brief GroupBegin output in console.
			*
			*	\param[in] _name	The name of the group that begins.
			*/
			inline void GroupBeginLog(const std::string& _name)
			{
				SetConsoleColor(CslColor::GroupBegin);
				SA_UTH_LOG("[SA-UTH] Group:\t" << _name);
				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief GroupEnd output in console.
			*
			*	\param[in] _group	The group that ends.
			*/
			inline void GroupEndLog(const struct UTH::Group& _group)
			{
				LogGroupTabs();
				SetConsoleColor(CslColor::GroupEnd);

				__SA_UTH_LOG_IN("[SA-UTH] Group:\t" << _group.name << " run: ");

				SetConsoleColor(CslColor::TestNum);
				__SA_UTH_LOG_IN(_group.testNum);

				SetConsoleColor(CslColor::GroupEnd);
				__SA_UTH_LOG_IN(" and exit with code: ");

				if (_group.localExit == EXIT_SUCCESS)
				{
					SetConsoleColor(CslColor::Success);
					__SA_UTH_LOG_IN("EXIT_SUCCESS (" << EXIT_SUCCESS << ')');
				}
				else
				{
					SetConsoleColor(CslColor::Failure);
					__SA_UTH_LOG_IN("EXIT_FAILURE (" << EXIT_FAILURE << ')');
				}

				__SA_UTH_LOG_ENDL();
				SetConsoleColor(CslColor::None);
			}


			/**
			*	\brief Test title output in console.
			*
			*	\param[in] _funcDecl	Declaration of the function as a string.
			*	\param[in] _lineNum		Line number of the function's call.
			*	\param[in] _pred		Result of the test.
			*/
			inline void TitleLog(const std::string& _funcDecl, unsigned int _lineNum, bool _pred)
			{
				SetConsoleColor(CslColor::Title);

				LogGroupTabs();
				__SA_UTH_LOG_IN("[SA-UTH] ");

				// Result.
				if (_pred)
				{
					SetConsoleColor(CslColor::Success);
					__SA_UTH_LOG_IN("Success ");
				}
				else
				{
					SetConsoleColor(CslColor::Failure);
					__SA_UTH_LOG_IN("Failure ");
				}

				SetConsoleColor(CslColor::Title);

				__SA_UTH_LOG_IN(_funcDecl << " -- l:" << _lineNum << std::endl);

				SetConsoleColor(CslColor::None);
			}

			/**
			*	\brief Test parameters output in console.
			*
			*	\param[in] _params	Every param infos extracted from call.
			*/
			inline void ParamsLog(const std::vector<Param>& _params)
			{
				for (auto it = _params.begin(); it != _params.end(); ++it)
				{
					if(verbosity & Verbosity::ParamsName)
						SA_UTH_LOG(it->name << ':');

					// ToString not implemented.
					if (it->value.empty())
					{
						__SA_UTH_LOG_IN("-No debug string-\t");
						SetConsoleColor(CslColor::ParamWarning);
						__SA_UTH_LOG_IN("Implement ToString() in class or UTH::ToString template specialization.");
						SetConsoleColor(CslColor::None);
					}
					else
						SA_UTH_LOG(IndentStr(it->value));
				}
			}
		}

		/// \endcond

#pragma endregion


#pragma region Init / Exit

		/**
		*	\brief Init function to be called at the start of main.
		*	Use SA_UTH_INIT() as helper macro.
		*/
		inline void Init()
		{
			using namespace Internal;

			time_t currTime = time(NULL);


			OpenLogFile(currTime);


			SetConsoleColor(CslColor::Init);

			// Init rand.
			srand(static_cast<unsigned int>(currTime));
			SA_UTH_LOG("[SA-UTH] Init Rand seed: " << currTime);
			
			SetConsoleColor(CslColor::None);
		}

		/**
		*	\brief Helper init program macro.
		*	Should be used at the start of main.
		*/
		#define SA_UTH_INIT() Sa::UTH::Init();


		/**
		*	\brief Exit function to be called at the end of main.
		*	Use SA_UTH_EXIT() as helper macro.
		* 
		*	\return exit code of all tests run.
		*/
		inline int Exit()
		{
			using namespace Internal;

			// Reset to default.
			bCslLog = SA_UTH_DFLT_CSL_LOG;
			bFileLog = SA_UTH_DFLT_FILE_LOG;

			SetConsoleColor(CslColor::Exit);
			__SA_UTH_LOG_IN("[SA-UTH] Run: ");

			SetConsoleColor(CslColor::TestNum);
			__SA_UTH_LOG_IN(testNum);

			SetConsoleColor(CslColor::Exit);
			__SA_UTH_LOG_IN(" and exit with code: ");

			if (exit == EXIT_SUCCESS)
			{
				SetConsoleColor(CslColor::Success);
				__SA_UTH_LOG_IN("EXIT_SUCCESS (" << EXIT_SUCCESS << ')');
			}
			else
			{
				SetConsoleColor(CslColor::Failure);
				__SA_UTH_LOG_IN("EXIT_FAILURE (" << EXIT_FAILURE << ')');
			}

			__SA_UTH_LOG_ENDL();
			SetConsoleColor(CslColor::None);
			

			CloseLogFile();


			return exit;
		}

		/**
		*	\brief Helper exit program macro.
		*	Should be used at the end of main.
		*/
		#define SA_UTH_EXIT() return Sa::UTH::Exit();

#pragma endregion


#pragma region Callback

		/// Pointer to allow user to get custom data in callbacks.
		inline void* UserData = nullptr;

		/**
		*	\brief Helper get user data with type.
		*
		*	\tparam T	The type to cast user data.
		*
		*	\return User data casted as T.
		*/
		template<typename T>
		inline T& GetUserData() { return *reinterpret_cast<T*>(UserData); }

		/// Callback called on groupe begin.
		inline void (*GroupBeginCB)(const std::string& _name) = nullptr;

		/// Callback called on groupe end.
		inline void (*GroupEndCB)(const Group& _group) = nullptr;

		/// Callback called on test's title processing.
		inline void (*TitleCB)(const std::string& _funcDecl, unsigned int _lineNum, bool _pred) = nullptr;

		/// Callback called on test's parameters processing.
		inline void (*ParamsCB)(const std::vector<Param>& _params) = nullptr;

		/// Callback called on test's result processing.
		inline void (*ResultCB)(bool _pred) = nullptr;

#pragma endregion


#pragma region Group

		/// \cond Internal

		namespace Internal
		{
			inline std::stack<Group> groups;

			inline std::string GroupTabStr() noexcept
			{
				return std::string(groups.size(), '\t');
			}

			inline void LogGroupTabs() noexcept
			{
				if(groups.size())
					__SA_UTH_LOG_IN(GroupTabStr());
			}

			/// Start a new group of tests.
			inline void GroupBegin(const std::string& _name)
			{
				// Log before push for log indentation.
				if ((verbosity & Verbosity::GroupStart) && ShouldLog())
					GroupBeginLog(_name);

				groups.push(Group{ _name });

				if (GroupBeginCB)
					GroupBeginCB(_name);
			}

			/// End a group of tests.
			inline Group GroupEnd()
			{
				Group group = groups.top();
				groups.pop();

				// Spread values to parent.
				if (!groups.empty())
					group.Spread(groups.top());

				if ((verbosity & Verbosity::GroupExit) && ShouldLog())
					GroupEndLog(group);

				if (GroupEndCB)
					GroupEndCB(group);

				return group;
			}
		}

		/// \endcond Internal

#pragma endregion


#pragma region ToString

		/**
		*	\brief Stringizer used to print elem during unit testing.
		*
		*	Default implementation tries to call _elem.ToString() if any ToString() function is defined in T.
		*	Define template specialization for complex class custom implementation.
		*	Allows template partial specialization (ex: Stringizer<MyClass<T>>).
		*
		*	\tparam T			Type of element.
		*/
		template <typename T>
		class Stringizer
		{
		public:
			/**
			*	Default implementation of stringizer
			* 
			*	\param[in] _elem	Element to convert to string.
			* 
			*	\return	std::string from converted element.
			*/
			static std::string Value(const T& _elem) noexcept
			{
				if constexpr (std::is_arithmetic<T>::value)
					return std::to_string(_elem);
				else if constexpr (std::is_pointer<T>::value)
					return std::string("Addr: ") + std::to_string(reinterpret_cast<unsigned __int64>(_elem));
				else if constexpr (Internal::HM_ToString<T>::value)
					return _elem.ToString();
				else
					return std::string();
			}
		};

		/**
		*	\brief ToString implementation used to print elem during unit testing.
		*
		*	Default implementation: helper to call Stringizer using template deduction.
		*	Define template specialization for simple class custom implementation.
		* 
		*	\tparam T			Type of element.
		*	\param[in] _elem	Element to convert to string.
		*
		*	\return	std::string from converted element using stringizer.
		*/
		template <typename T>
		std::string ToString(const T& _elem)
		{
			return Stringizer<T>::Value(_elem);
		}

		/**
		*	\brief ToString implementation used to print tab of elems during unit testing.
		*
		*	Default implementation is for loop ToString(_elem).
		*	Define template specialization for custom implementation.
		*
		*	\tparam T			Type of element.
		*	\tparam size		Size of elem tab.
		*	\param[in] _elems	Element to convert to string.
		*
		*	\return	std::string from converted elements  using stringizer.
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

		/**
		*	\brief Rand specialization for bool.
		* 
		*	\param[in] _min		Min bound (false) (included).
		*	\param[in] _max		Max bound (true) (included).
		*
		*	\return Random bool.
		*/
		template<>
		inline bool Rand(bool _min, bool _max) { (void)_min; (void)_max; return Rand<int>(0, 2) == 1; }

#pragma endregion


#pragma region Compute

		/// \cond Internal

		namespace Internal
		{
			/// Update UTH module from predicate.
			inline void Update(bool _pred)
			{
				++testNum;

				if (!groups.empty())
					groups.top().Update(_pred);
			}
			

			/// Compute title from function declaration and line num.
			inline void ComputeTitle(const std::string& _funcDecl, unsigned int _lineNum, bool _pred)
			{
				if(ShouldLog())
					TitleLog(_funcDecl, _lineNum, _pred);

				if (TitleCB)
					TitleCB(_funcDecl, _lineNum, _pred);
			}


			/// Compute params.
			template <typename... Args>
			void ComputeParam(bool _pred, std::string _paramNames, const Args&... _args)
			{
				// No need to compute params.
				if (!ShouldLog() && !ParamsCB)
					return;

				if ((_pred && (verbosity & ParamsSuccess)) ||		// Should output params on success.
					(!_pred && (verbosity & ParamsFailure)))		// Should output params on failure.
				{
					std::vector<Param> params;
					GenerateParamStr(params, _paramNames, _args...);

					ParamsLog(params);

					if (ParamsCB)
						ParamsCB(params);
				}
			}

			/// \brief Generate Params from params' names and values.
			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<Param>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args)
			{
				size_t index = _paramNames.find_first_of(',');

				_result.push_back(Param{ _paramNames.substr(0u, index), ToString(_first) });

				if constexpr (sizeof...(_args) != 0)
					GenerateParamStr(_result, _paramNames.substr(index + 2), _args...);
			}


			/// Compute the result using _pred predicate.
			inline void ComputeResult(bool _pred)
			{
				if (!_pred)
					Sa::UTH::exit = EXIT_FAILURE;

				if (ResultCB)
					ResultCB(_pred);

#if SA_UTH_EXIT_ON_FAILURE
				if (!_pred)
					::exit(EXIT_FAILURE);
#endif
			}

			/// Wether to continue computing test with predicate _pred.
			inline bool ShouldComputeTest(bool _pred)
			{
				return !_pred || (verbosity & Verbosity::Success);
			}

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			template <typename... Args>
			unsigned int SizeOfArgs(const Args&...)
			{
				return sizeof...(Args);
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
		#define SA_UTH_EQ(_lhs, _rhs, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = UTH::Equals(_lhs, _rhs, __VA_ARGS__);\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				std::string titleStr = std::string("Sa::UTH::Equals(" #_lhs ", " #_rhs) + (SizeOfArgs(__VA_ARGS__) ? ", " #__VA_ARGS__ ")" : ")");\
			\
				ComputeTitle(titleStr, __LINE__, bRes);\
				ComputeParam(bRes, #_lhs ", " #_rhs ", " #__VA_ARGS__, _lhs, _rhs, __VA_ARGS__);\
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
		#define SA_UTH_SF(_func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = _func(__VA_ARGS__);\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_func "(" #__VA_ARGS__ ")", __LINE__, bRes);\
				ComputeParam(bRes, #__VA_ARGS__, __VA_ARGS__);\
				ComputeResult(bRes);\
			}\
		}

		/**
		*	\brief Run a \e <b> Unit Test </b> using a static function with return value.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _res		Value to compare with _func result.
		*	\param[in] _func	Function to test with ... args.
		*/
		#define SA_UTH_RSF(_res, _func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			auto result = _func(__VA_ARGS__);\
			bool bRes = result == _res;\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_func "(" #__VA_ARGS__ ") == " #_res, __LINE__, bRes);\
				ComputeParam(bRes, #__VA_ARGS__ ", " #_func "(), " #_res, __VA_ARGS__, result, _res);\
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
		#define SA_UTH_MF(_caller, _func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			bool bRes = (_caller)._func(__VA_ARGS__);\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_caller "." #_func "(" #__VA_ARGS__ ")", __LINE__, bRes);\
				ComputeParam(bRes, #_caller ", " #__VA_ARGS__, _caller, __VA_ARGS__);\
				ComputeResult(bRes);\
			}\
		}

		/**
		*	\brief Run a \e <b> Unit Test </b> using a member function with return value.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _res		Value to compare with _func result.
		*	\param[in] _caller	caller of the functin _func.
		*	\param[in] _func	Function to test with ... args.
		*/
		#define SA_UTH_RMF(_res, _caller, _func, ...)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			auto result = (_caller)._func(__VA_ARGS__);\
			bool bRes = result == _res;\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_caller "." #_func "(" #__VA_ARGS__ ") == " #_res, __LINE__, bRes);\
				ComputeParam(bRes, #_caller ", " #__VA_ARGS__ ", " #_caller "." #_func "(), " #_res, _caller, __VA_ARGS__, result, _res);\
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
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_lhs " " #_op " " #_rhs, __LINE__, bRes);\
				ComputeParam(bRes, #_lhs ", " #_rhs, _lhs, _rhs);\
				ComputeResult(bRes);\
			}\
		}

		/**
		*	\brief Run a \e <b> Unit Test </b> using an operator with return value.
		*
		*	UTH::exit will be equal to EXIT_FAILURE (1) if at least one test failed.
		*
		*	\param[in] _res		Value to compare with operator result.
		*	\param[in] _lhs		Left hand side operand to test.
		*	\param[in] _op		Operator of the test between _lhs and _rhs.
		*	\param[in] _rhs		Right hand side operand to test.
		*/
		#define SA_UTH_ROP(_res, _lhs, _op, _rhs)\
		{\
			using namespace Sa::UTH::Internal;\
		\
			auto result = _lhs _op _rhs;\
			bool bRes = result == _res;\
			Sa::UTH::Internal::Update(bRes);\
		\
			if(ShouldComputeTest(bRes))\
			{\
				ComputeTitle(#_lhs " " #_op " " #_rhs " == " #_res, __LINE__, bRes);\
				ComputeParam(bRes, #_lhs ", " #_rhs ", " #_lhs " " #_op " " #_rhs ", " #_res, _lhs, _rhs, result, _res);\
				ComputeResult(bRes);\
			}\
		}


		/**
		*	\brief Begin a group of test with name.
		* 
		*	\param[in] _name	Name of the group.
		*/
		#define SA_UTH_GPB(_name) Sa::UTH::Internal::GroupBegin(#_name);

		/**
		*	\brief End current group.
		*/
		#define SA_UTH_GPE() Sa::UTH::Internal::GroupEnd();


		/**
		*	\brief Run a group of tests from a single function.
		*
		*	\param[in] _func	The function that own the group of tests.
		*/
		#define SA_UTH_GP(_func)\
		{\
			SA_UTH_GPB(_func)\
			_func;\
			SA_UTH_GPE()\
		}

#pragma endregion
	}
}

#endif // GUARD
