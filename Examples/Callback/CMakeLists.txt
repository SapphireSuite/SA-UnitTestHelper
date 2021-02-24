# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.


# === Output ===

# Add executable target built from sources.
add_executable(UnitTestCallback main_callback.cpp)


# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTestCallback PRIVATE SA-UnitTestHelper)


# === Testing ===

# Create CTest that run UnitTestSuccess exe.
add_test(NAME CUnitTestCallback COMMAND UnitTestCallback --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestCallback>)
