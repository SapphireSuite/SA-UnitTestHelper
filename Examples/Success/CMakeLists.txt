# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_Sucess main_success.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_Sucess PRIVATE SA-UnitTestHelper)



# === Testing ===

# Create CTest that run UnitTestSuccess exe.
add_test(NAME CUTH_Sucess COMMAND UTH_Sucess --config $<CONFIGURATION> --exe $<TARGET_FILE:UTH_Sucess>)
