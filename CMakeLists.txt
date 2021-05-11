# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.

cmake_minimum_required(VERSION 3.16)



# === Projects ===

# Create CMake project.
project(SA-UnitTestHelper)

message("Main directory: ${CMAKE_SOURCE_DIR}")



# === Input ===

# Add interface library target.
add_library(SA-UnitTestHelper INTERFACE)

# Add SA-UnitTestHelper.hpp include directory.
target_include_directories(SA-UnitTestHelper INTERFACE .)



# === Compile features ===

# Standard
target_compile_features(SA-UnitTestHelper INTERFACE cxx_std_17)

# UTH module implementation preprocessor.
target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_IMPL)



# === Option ===

# Default console log toggle value.
option(SA_UTH_DFLT_CSL_LOG "Should Log tests in console by default" ON)
if(SA_UTH_DFLT_CSL_LOG)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_DFLT_CSL_LOG)
endif()


# Default file log toggle value.
option(SA_UTH_DFLT_FILE_LOG "Should Log tests in a file by default" ON)

if(SA_UTH_DFLT_FILE_LOG)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_DFLT_FILE_LOG)
endif()


# Test exit on first failure.
option(SA_UTH_EXIT_ON_FAILURE "Exit on first failure" OFF)

if(SA_UTH_EXIT_ON_FAILURE)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_EXIT_ON_FAILURE)
endif()


# Test exit on first failure.
option(SA_UTH_EXIT_PAUSE "Should pause program on exit" OFF)

if(SA_UTH_EXIT_PAUSE)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_EXIT_PAUSE)
endif()


# Add SA-UnitTestHelper's examples to build tree.
option(SA_UTH_BUILD_EXAMPLES "Should build SA-Engine tests" OFF)


# === Tests ===

# Enable testing for this directory and below.
enable_testing()



# === Entrypoints ===

if(SA_UTH_BUILD_EXAMPLES OR SA_CI)
	add_subdirectory(Examples)
endif()
