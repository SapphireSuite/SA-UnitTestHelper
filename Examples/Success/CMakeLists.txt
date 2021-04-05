# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_Success main_success.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_Success PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestSuccess exe.
add_test(NAME CSA-UTH_Success COMMAND SA-UTH_Success --config $<CONFIGURATION> --exe $<TARGET_FILE:SA-UTH_Success>)
