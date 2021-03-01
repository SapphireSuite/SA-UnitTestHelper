# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_MacroOp main_macro_op.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_MacroOp PRIVATE SA-UnitTestHelper)
