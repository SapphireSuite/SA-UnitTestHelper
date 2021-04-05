# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_Failure main_failure.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_Failure PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestFailure exe.
#add_test(NAME CSA-UTH_Failure COMMAND SA-UTH_Failure --config $<CONFIGURATION> --exe $<TARGET_FILE:SA-UTH_Failure>)