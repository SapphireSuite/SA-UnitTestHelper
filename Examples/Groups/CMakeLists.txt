# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_Groups main_groups.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_Groups PRIVATE SA-UnitTestHelper)
