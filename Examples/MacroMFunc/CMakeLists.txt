# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_MacroMFunc main_macro_mfunc.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_MacroMFunc PRIVATE SA-UnitTestHelper)
