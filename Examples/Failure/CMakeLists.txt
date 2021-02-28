# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UnitTestFailure main_failure.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTestFailure PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestFailure exe.
add_test(NAME CUnitTestFailure COMMAND UnitTestFailure --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestFailure>)