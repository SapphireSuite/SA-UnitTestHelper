// Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.
// Repository: https://github.com/SapphireSuite/UnitTestHelper

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_HELPER_GUARD
#define SAPPHIRE_UNIT_TEST_HELPER_GUARD

#include <stack>
#include <vector>

#include <string>
#include <string.h> // Requiered for strrchr.
#include <iostream>

#include <fstream>
#include <filesystem>

#if _WIN32

#include <Windows.h>

#endif

#if SA_CORE_IMPL

#include <SA-Core/Debug/ToString.hpp>

#endif

/**
*	\file UnitTestHelper.hpp
*
*	\brief Single header file helper for unit testing.
*/

namespace Sa
{
	/// UnitTestHelper global namespace.
	namespace UTH
	{
//{ Hpp

//{ Init / Exit

#ifndef SA_UTH_EXIT_ON_FAILURE
		/**
		*	\brief Wether to exit program on failure or continue next tests.
		*	Can be defined within cmake options or before including the header.
		*/
		#define SA_UTH_EXIT_ON_FAILURE 0
#endif

#ifndef SA_UTH_EXIT_PAUSE
	/**
	*	\brief Whether to pause program on exit.
	*	Always disabled in continuous integration (define SA_CI).
	*/
	#define SA_UTH_EXIT_PAUSE 0
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


		namespace Intl
		{
			/**
			*	\brief Init function to be called at the start of main.
			*	Use SA_UTH_INIT() as helper macro.
			*/
			inline void Init();

			/**
			*	\brief Exit function to be called at the end of main.
			*	Use SA_UTH_EXIT() as helper macro.
			*
			*	\return exit code of all tests run.
			*/
			inline int Exit();
		}


		/**
		*	\brief Helper init program macro.
		*	Should be used at the start of main.
		*/
		#define SA_UTH_INIT() Sa::UTH::Intl::Init();

		/**
		*	\brief Helper exit program macro.
		*	Should be used at the end of main.
		*/
		#define SA_UTH_EXIT() return Sa::UTH::Intl::Exit();

//}


//{ Verbosity

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

			/// Output group counter on exit.
			GroupCount = 1 << 6,


			/// Light verbosity value.
			Light = ParamsName | ParamsFailure | GroupExit,

			/// Default verbosity value.
			Default = Success | ParamsName | ParamsFailure | GroupStart | GroupExit | GroupCount,

			/// Maximum verbosity level (all flags set).
			Max = 0xFF
		};

		/// Current verbosity level.
		inline unsigned int verbosity = Default;

//}


//{ Logger

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
		namespace Intl
		{
			class Logger
			{
				std::string logFileName;

				inline Logger();
				inline ~Logger();

			public:
				static Logger instance;

				std::fstream logFile;
			};

			inline Logger Logger::instance;


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

			inline void SetConsoleColor(CslColor _result);

			/**
			*	\brief Log enabled and should log.
			* 
			*	\return log toggle.
			*/
			inline bool ShouldLog() noexcept;

			/**
			*	\brief Indent in-string \n with group tabs.
			* 
			*	\param[in] _str		String to indent.
			* 
			*	\return	indented string.
			*/
			inline std::string IndentStr(std::string _str);

			/// Getter of file name.
			inline const char* GetFileNameFromPath(const char* _filePath) noexcept;
		}


		/**
		*	\brief UTH log macro
		*
		*	Output with indentation and options bCslLog and bFileLog.
		*/
		#define SA_UTH_LOG(_str)\
		{\
			Sa::UTH::Group::LogTabs();\
			if (Sa::UTH::bCslLog) std::cout << _str << std::endl;\
			if (Sa::UTH::bFileLog) Sa::UTH::Intl::Logger::instance.logFile << _str << std::endl;\
		}

		/// Output only str as input.
		#define __SA_UTH_LOG_IN(_str)\
		{\
			if (Sa::UTH::bCslLog) std::cout << _str;\
			if (Sa::UTH::bFileLog) Sa::UTH::Intl::Logger::instance.logFile << _str;\
		}

		/// Ouput only end of line.
		#define __SA_UTH_LOG_ENDL()\
		{\
			if (Sa::UTH::bCslLog) std::cout << std::endl;\
			if (Sa::UTH::bFileLog) Sa::UTH::Intl::Logger::instance.logFile << std::endl;\
		}

//}


//{ Counter

		struct Counter
		{
			/// Counter of success.
			unsigned int success = 0;

			/// Counter of failure.
			unsigned int failure = 0;

			/// Total count.
			inline unsigned int Total() const;

			/// Update counter from predicate.
			inline void Update(bool _pred);

			inline Counter& operator+=(const Counter& _rhs) noexcept;

			inline void Log() const;

			inline bool IsEmpty() const;
		};

//}


//{ Title

		struct Title
		{
			const std::string& funcDecl;
			const std::string& fileName;
			unsigned int lineNum = 0u;
			bool pred = false;

			/// Log test's title in console.
			inline void Log() const;
		};

//}


//{ Group

		/// Infos generated from a group of tests.
		class Group
		{
			static std::stack<Group> sGroups;

		public:

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

			/// Counter of test run in this group.
			Counter count{};

			/// Global Group counter.
			static inline Counter globalCount;

			/**
			*	\brief Spreads values to parent group.
			*
			*	\param[in] _parent	parent to spread values to.
			*/
			inline void Spread(Group& _parent);

			/**
			*	\brief Update values from predicate.
			*
			*	\param[in] _pred	predicate from current test.
			*/
			static inline void Update(bool _pred);

			/// Start a new group of tests.
			static inline void Begin(const std::string& _name);

			/// End a group of tests.
			static inline Group End();


			/**
			*	\brief GroupBegin output in console.
			*
			*	\param[in] _name	The name of the group that begins.
			*/
			static inline void BeginLog(const std::string& _name);

			/**
			*	\brief GroupEnd output in console.
			*
			*	\param[in] _group	The group that ends.
			*/
			static inline void EndLog(const UTH::Group& _group);


			static inline std::string TabStr() noexcept;

			static inline void LogTabs() noexcept;
		};

		inline std::stack<Group> Group::sGroups;

//}


//{ Param

		/// Pair of param name and value.
		struct Param
		{
			/// Param's name.
			std::string name;

			/// Param's value as a string.
			std::string value;

			/**
			*	\brief Test parameters output in console.
			*
			*	\param[in] _params	Every param infos extracted from call.
			*/
			static inline void Log(const std::vector<Param>& _params);
		};

//}


//{ Callback

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
		inline void (*TitleCB)(const Title& _infos) = nullptr;

		/// Callback called on test's parameters processing.
		inline void (*ParamsCB)(const std::vector<Param>& _params) = nullptr;

		/// Callback called on test's result processing.
		inline void (*ResultCB)(bool _pred) = nullptr;

//}


//{ ToString

		namespace Intl
		{
			/**
			*	Compile time check type T has member ToString using SFINAE.
			* 
			*	Source: https://github.com/SapphireSuite/Core/blob/main/Include/Core/Misc/HasMember.hpp
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
			if constexpr (std::is_arithmetic<T>::value)
				return std::to_string(_elem);
			else if constexpr (std::is_enum<T>::value)
				return std::to_string(static_cast<unsigned int>(_elem));
			else if constexpr (std::is_pointer<T>::value)
				return "0x" + std::to_string(reinterpret_cast<unsigned long long>(_elem));
			else if constexpr (Intl::HM_ToString<T>::value)
				return _elem.ToString();
			else if constexpr (std::is_same<T, std::string>::value)
				return _elem;
			else
			{
#if SA_CORE_IMPL
				return Sa::ToString(_elem);
#else
				return std::string();
#endif
			}
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
				res += ToString(_elems[i]) + "; ";

			res[res.size() - 2] = ' ';
			res[res.size() - 1] = '}';

			return res;
		}

//}


//{ Equals

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

//}


//{ Random

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
			return _min + static_cast<T>(rand()) / static_cast<T>(RAND_MAX / (_max - _min));
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

//}


//{ Compute

		/// \cond Internal

		namespace Intl
		{
			/// Total number of test run.
			inline Counter globalCount;

			/// Update UTH module from predicate.
			inline void Update(bool _pred);
			

			/// Compute title from function declaration and line num.
			inline void ComputeTitle(const Title& _infos);


			/// Compute params.
			template <typename... Args>
			void ComputeParam(bool _pred, std::string _paramNames, const Args&... _args);

			/// \brief Generate Params from params' names and values.
			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<Param>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args);


			/// Compute the result using _pred predicate.
			inline void ComputeResult(bool _pred);

			/// Wether to continue computing test with predicate _pred.
			inline bool ShouldComputeTest(bool _pred);

			/// \brief Helper function for size of VA_ARGS (handle empty args).
			template <typename... Args>
			unsigned int SizeOfArgs(const Args&...);
		}

		/// \endcond

//}


//}


//{ Cpp

		/// \cond

//{ Init / Exit

		namespace Intl
		{
			void Init()
			{
				using namespace Intl;

				SetConsoleColor(CslColor::Init);

				// Init rand.
				time_t currTime = time(NULL);
				srand(static_cast<unsigned int>(currTime));
				SA_UTH_LOG("[SA-UTH] Init Rand seed: " << currTime);

				SetConsoleColor(CslColor::None);
			}

			int Exit()
			{
				using namespace Intl;

				// Reset to default.
				bCslLog = SA_UTH_DFLT_CSL_LOG;
				bFileLog = SA_UTH_DFLT_FILE_LOG;

				SetConsoleColor(CslColor::Exit);
				__SA_UTH_LOG_IN("[SA-UTH] Run: ");


				Intl::globalCount.Log();

				// Output Group counter.
				if ((verbosity & Verbosity::GroupCount) && !Group::globalCount.IsEmpty())
				{
					SetConsoleColor(CslColor::Exit);
					__SA_UTH_LOG_IN(" in ");

					Group::globalCount.Log();

					SetConsoleColor(CslColor::GroupEnd);
					__SA_UTH_LOG_IN(" groups");
				}


				// Output exit.
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

			#if SA_UTH_EXIT_PAUSE && !defined(SA_CI)
				SA_UTH_LOG("[SA-UTH] Press Enter to continue...");
				std::cin.get();
			#endif

				return exit;
			}
		}

//}


//{ Logger

		namespace Intl
		{
			Logger::Logger()
			{
				time_t currTime = time(NULL);

				// Open Log file.
				{
					std::filesystem::create_directories("Logs");

					struct tm timeinfo;

				# if _WIN32
					localtime_s(&timeinfo, &currTime);
				#else
					localtime_r(&currTime, &timeinfo);
				#endif

					/**
					*	log_UTH-<month>.<day>.<year>-<hour>h<minute>m<second>s.txt
					*	Ex: 2/27/2021 at 12:07:43
					*	log_UTH-2.27.2021-12h07m43s.txt
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
			}

			Logger::~Logger()
			{
				// Close log file.
				logFile.close();
			}


#if _WIN32
			void SetConsoleColor(CslColor _result)
			{
				static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
			void SetConsoleColor(CslColor _result)
			{
				switch (_result)
				{
					case CslColor::None:
						std::cout << "\033[0;0m";
						break;
					case CslColor::Title:
						std::cout << "\033[0;33m";
						break;
					case CslColor::Success:
						std::cout << "\033[0;32m";
						break;
					case CslColor::Failure:
						std::cout << "\033[0;31m";
						break;
					case CslColor::TestNum:
						std::cout << "\033[1;33m";
						break;
					case CslColor::GroupBegin:
						std::cout << "\033[1;34m";
						break;
					case CslColor::GroupEnd:
						std::cout << "\033[1;34m";
						break;
					case CslColor::Init:
						std::cout << "\033[0;35m";
						break;
					case CslColor::Exit:
						std::cout << "\033[0;35m";
						break;
					case CslColor::ParamWarning:
						std::cout << "\033[1;33m";
						break;
					default:
						SA_UTH_LOG("CslColor not supported yet!");
						break;
				}
			}
#endif

			bool ShouldLog() noexcept
			{
				return bCslLog || bFileLog;
			}

			std::string IndentStr(std::string _str)
			{
				const std::string indentStr = std::string("\n") + Group::TabStr();

				size_t index = _str.find('\n');

				while (index != std::string::npos)
				{
					_str.replace(index, 1, indentStr);

					index = _str.find('\n', index + indentStr.size());
				}

				return _str;
			}

			/// Getter of file name.
			const char* GetFileNameFromPath(const char* _filePath) noexcept
			{
				// Remove characters until last backslash.
				const char* fileName = strrchr(_filePath, '\\');

				if (!fileName) // No backslash found.
					fileName = _filePath;
				else
					fileName += 1; // Remove last '\\' found.


				// Remove characters until last slash.
				if (const char* filePathNoSlash = strrchr(fileName, '/'))
				{
					// Remove last '/' found.
					fileName = filePathNoSlash + 1;
				}

				return fileName;
			}
		}

//}


//{ Counter

		unsigned int Counter::Total() const
		{
			return success + failure;
		}

		void Counter::Update(bool _pred)
		{
			if (_pred)
				++success;
			else
				++failure;
		}

		Counter& Counter::operator+=(const Counter& _rhs) noexcept
		{
			success += _rhs.success;
			failure += _rhs.failure;

			return *this;
		}

		void Counter::Log() const
		{
			using namespace Intl;

			SetConsoleColor(CslColor::TestNum);
			__SA_UTH_LOG_IN(Total());

			if (failure)
			{
				__SA_UTH_LOG_IN(" (");

				SetConsoleColor(CslColor::Success);
				__SA_UTH_LOG_IN(success);

				SetConsoleColor(CslColor::TestNum);
				__SA_UTH_LOG_IN('/');

				SetConsoleColor(CslColor::Failure);
				__SA_UTH_LOG_IN(failure);

				SetConsoleColor(CslColor::TestNum);
				__SA_UTH_LOG_IN(')');
			}
		}

		bool Counter::IsEmpty() const
		{
			return success != 0 && failure != 0;
		}

//}


//{ Title

		void Title::Log() const
		{
			using namespace Intl;

			SetConsoleColor(CslColor::Title);

			Group::LogTabs();
			__SA_UTH_LOG_IN("[SA-UTH] ");

			// Result.
			if (pred)
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

			__SA_UTH_LOG_IN(funcDecl << " -- " << fileName << ":" << lineNum << std::endl);

			SetConsoleColor(CslColor::None);
		}

//}


//{ Group

		void Group::Update(bool _pred)
		{
			// Update top group.
			if (!Group::sGroups.empty())
			{
				Group& gp = Group::sGroups.top();


				gp.count.Update(_pred);

				if (!_pred)
					gp.localExit = EXIT_FAILURE;
			}
		}

		void Group::Spread(Group& _parent)
		{
			if (localExit == EXIT_FAILURE)
				_parent.localExit = EXIT_FAILURE;

			_parent.count += count;
		}

		void Group::Begin(const std::string& _name)
		{
			// Log before push for log indentation.
			if ((verbosity & Verbosity::GroupStart) && Intl::ShouldLog())
				BeginLog(_name);

			sGroups.push(Group{ _name });

			if (GroupBeginCB)
				GroupBeginCB(_name);
		}

		Group Group::End()
		{
			Group group = sGroups.top();
			sGroups.pop();

			// Spread values to parent.
			if (!sGroups.empty())
				group.Spread(sGroups.top());

			if ((verbosity & Verbosity::GroupExit) && Intl::ShouldLog())
				EndLog(group);

			if (GroupEndCB)
				GroupEndCB(group);

			globalCount.Update(group.localExit == EXIT_SUCCESS);

			return group;
		}


		void Group::BeginLog(const std::string& _name)
		{
			using namespace Intl;

			SetConsoleColor(CslColor::GroupBegin);
			SA_UTH_LOG("[SA-UTH] Group:\t" << _name);
			SetConsoleColor(CslColor::None);
		}

		void Group::EndLog(const UTH::Group& _group)
		{
			using namespace Intl;

			LogTabs();
			SetConsoleColor(CslColor::GroupEnd);

			__SA_UTH_LOG_IN("[SA-UTH] Group:\t" << _group.name << " run: ");

			_group.count.Log();

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


		std::string Group::TabStr() noexcept
		{
			return std::string(sGroups.size(), '\t');
		}

		void Group::LogTabs() noexcept
		{
			if (sGroups.size())
				__SA_UTH_LOG_IN(TabStr());
		}

//}


//{ Param

		void Param::Log(const std::vector<Param>& _params)
		{
			using namespace Intl;

			for (auto it = _params.begin(); it != _params.end(); ++it)
			{
				if (verbosity & Verbosity::ParamsName)
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

//}


//{ Compute

		namespace Intl
		{
			void Update(bool _pred)
			{
				globalCount.Update(_pred);

				Group::Update(_pred);
			}
			
			void ComputeTitle(const Title& _infos)
			{
				if(ShouldLog())
					_infos.Log();

				if (TitleCB)
					TitleCB(_infos);
			}


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

					Param::Log(params);

					if (ParamsCB)
						ParamsCB(params);
				}
			}

			template <typename FirstT, typename... Args>
			void GenerateParamStr(std::vector<Param>& _result, std::string _paramNames, const FirstT& _first, const Args&... _args)
			{
				size_t index = _paramNames.find_first_of(',');

				_result.push_back(Param{ _paramNames.substr(0u, index), Sa::UTH::ToString(_first) });

				if constexpr (sizeof...(_args) != 0)
					GenerateParamStr(_result, _paramNames.substr(index + 2), _args...);
			}


			void ComputeResult(bool _pred)
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

			inline bool ShouldComputeTest(bool _pred)
			{
				return !_pred || (verbosity & Verbosity::Success);
			}

			template <typename... Args>
			unsigned int SizeOfArgs(const Args&...)
			{
				return sizeof...(Args);
			}
		}

//}

		/// \endcond

//}


//{ Macro

		/// \cond Internal

		/// Helper macro for file name.
		#define __SA_UTH_FILE_NAME Sa::UTH::Intl::GetFileNameFromPath(__FILE__)

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
		#define SA_UTH_EQ(_lhs, _rhs, ...)\
		{\
			auto&& sLhs = _lhs;\
			auto&& sRhs = _rhs;\
			bool bRes = Sa::UTH::Equals(sLhs, sRhs, ##__VA_ARGS__);\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				std::string titleStr = std::string("Sa::UTH::Equals(" #_lhs ", " #_rhs) + \
				(Sa::UTH::Intl::SizeOfArgs(__VA_ARGS__) ? ", " #__VA_ARGS__ ")" : ")");\
			\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ titleStr, __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #_lhs ", " #_rhs ", " #__VA_ARGS__, sLhs, sRhs, ##__VA_ARGS__);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			bool bRes = _func(__VA_ARGS__);\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_func "(" #__VA_ARGS__ ")", __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #__VA_ARGS__, __VA_ARGS__);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			auto result = _func(__VA_ARGS__);\
			bool bRes = result == _res;\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_func "(" #__VA_ARGS__ ") == " #_res, __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #__VA_ARGS__ ", " #_func "(), " #_res, __VA_ARGS__, result, _res);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			bool bRes = (_caller)._func(__VA_ARGS__);\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_caller "." #_func "(" #__VA_ARGS__ ")", __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #_caller ", " #__VA_ARGS__, _caller, ##__VA_ARGS__);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			auto result = (_caller)._func(__VA_ARGS__);\
			bool bRes = result == _res;\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_caller "." #_func "(" #__VA_ARGS__ ") == " #_res, __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #_caller ", " #__VA_ARGS__ ", " #_caller "." #_func "(), " #_res, _caller, __VA_ARGS__, result, _res);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			auto&& sLhs = _lhs;\
			auto&& sRhs = _rhs;\
			bool bRes = sLhs _op sRhs;\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_lhs " " #_op " " #_rhs, __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #_lhs ", " #_rhs, sLhs, sRhs);\
				Sa::UTH::Intl::ComputeResult(bRes);\
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
			auto&& sLhs = _lhs;\
			auto&& sRhs = _rhs;\
			auto result = sLhs _op sRhs;\
			bool bRes = result == _res;\
			Sa::UTH::Intl::Update(bRes);\
		\
			if(Sa::UTH::Intl::ShouldComputeTest(bRes))\
			{\
				Sa::UTH::Intl::ComputeTitle(Sa::UTH::Title{ #_lhs " " #_op " " #_rhs " == " #_res, __SA_UTH_FILE_NAME, __LINE__, bRes });\
				Sa::UTH::Intl::ComputeParam(bRes, #_lhs ", " #_rhs ", " #_lhs " " #_op " " #_rhs ", " #_res, sLhs, sRhs, result, _res);\
				Sa::UTH::Intl::ComputeResult(bRes);\
			}\
		}


		/**
		*	\brief Begin a group of test with name.
		* 
		*	\param[in] _name	Name of the group.
		*/
		#define SA_UTH_GPB(_name) Sa::UTH::Group::Begin(#_name);

		/**
		*	\brief End current group.
		*/
		#define SA_UTH_GPE() Sa::UTH::Group::End();


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

//}
	}
}

#endif // GUARD
