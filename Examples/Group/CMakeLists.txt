# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.


# === Output ===

# Add executable target built from sources.
add_executable(UnitTestGroup main_group.cpp)


# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTestGroup PRIVATE SA-UnitTestHelper)


# === Testing ===

# Create CTest that run UnitTestSuccess exe.
add_test(NAME CUnitTestGroup COMMAND UnitTestGroup --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestGroup>)
