# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_MacroEquals main_macro_equals.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_MacroEquals PRIVATE SA-UnitTestHelper)
