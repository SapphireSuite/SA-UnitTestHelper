# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.

cmake_minimum_required(VERSION 3.16)


# Create Cmake project.
project(SA-UnitTestHelper)


# === Output ===

# Setup output directories for project.
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}_${CMAKE_HOST_SYSTEM_PROCESSOR})
message("Output Directory: ${CMAKE_BINARY_DIR}")


# === Input ===

# Add this directory as include directory.
# WARNING: **include_directories** is deprecated and only used for easier access to UnitTestHelper.hpp in examples. Use target_include_directories instead.
include_directories(.)


# === Testing ===

# Enable testing for this directory and below.
enable_testing()


# === Entrypoints ===

add_subdirectory(Examples)