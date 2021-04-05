# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_Failure main_failure.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_Failure PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestFailure exe.
#add_test(NAME CUTH_Failure COMMAND UTH_Failure --config $<CONFIGURATION> --exe $<TARGET_FILE:UTH_Failure>)