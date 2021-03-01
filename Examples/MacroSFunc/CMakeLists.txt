# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_MacroSFunc main_macro_sfunc.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_MacroSFunc PRIVATE SA-UnitTestHelper)
