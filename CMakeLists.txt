# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

cmake_minimum_required(VERSION 3.16)


# Create Cmake project.
project(SA-UnitTestHelper)


# === Input ===

# Add interface library target.
add_library(SA-UnitTestHelper INTERFACE)

# Add SA-UnitTestHelper.hpp include directory.
target_include_directories(SA-UnitTestHelper INTERFACE .)


# === Option ===

# Build examples.
option(SA_UTH_EXAMPLES "Build SA-UnitTestHelper examples" OFF)

if(SA_UTH_EXAMPLES)
	# Setup output directories for project.
	set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR})
	message("Output Directory: ${CMAKE_BINARY_DIR}")

	# Enable testing for this directory and below.
	enable_testing()

	# Add entries.
	add_subdirectory(Examples)
endif()


# Default console log toggle value.
option(SA_UTH_DEFAULT_CSL_LOG "Should Log tests in console by default" ON)

if(SA_UTH_DEFAULT_CSL_LOG)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_DEFAULT_CSL_LOG)
endif()


# Test exit on first failure.
option(SA_UTH_EXIT_ON_FAILURE "Exit on first failure" OFF)

if(SA_UTH_EXIT_ON_FAILURE)
	target_compile_definitions(SA-UnitTestHelper INTERFACE SA_UTH_EXIT_ON_FAILURE)
endif()
