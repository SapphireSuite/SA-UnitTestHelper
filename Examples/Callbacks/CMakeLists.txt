# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.


# === Output ===

# Add executable target built from sources.
add_executable(UnitTestCallbacks main_callbacks.cpp)


# === Dependencies ===

# Add library dependencies.
target_link_libraries(UnitTestCallbacks PRIVATE SA-UnitTestHelper)


# === Testing ===

# Create CTest that run UnitTestCallbacks exe.
add_test(NAME CUnitTestCallbacks COMMAND UnitTestCallbacks --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestCallbacks>)
