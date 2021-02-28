# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UnitTestGroups main_groups.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTestGroups PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestGroups exe.
add_test(NAME CUnitTestGroups COMMAND UnitTestGroups --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestGroups>)
