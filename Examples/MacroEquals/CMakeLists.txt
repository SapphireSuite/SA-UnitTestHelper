# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_MacroEquals main_macro_equals.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_MacroEquals PRIVATE SA-UnitTestHelper)
