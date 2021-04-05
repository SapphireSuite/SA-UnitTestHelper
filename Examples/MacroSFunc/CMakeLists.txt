# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_MacroSFunc main_macro_sfunc.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_MacroSFunc PRIVATE SA-UnitTestHelper)
