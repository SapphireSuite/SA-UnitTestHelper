# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.


# === Output ===
# Add executable target built from sources.
add_executable(UnitTestFailure main_failure.cpp)

# === Testing ===
# Create CTest that run UnitTestFailure exe.
add_test(NAME CUnitTestFailure COMMAND UnitTestFailure --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestFailure>)