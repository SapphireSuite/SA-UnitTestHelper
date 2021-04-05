# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_MacroMFunc main_macro_mfunc.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_MacroMFunc PRIVATE SA-UnitTestHelper)
