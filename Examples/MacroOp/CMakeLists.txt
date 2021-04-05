# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_MacroOp main_macro_op.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_MacroOp PRIVATE SA-UnitTestHelper)
