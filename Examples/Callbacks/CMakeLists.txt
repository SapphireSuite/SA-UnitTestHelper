# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(SA-UTH_Callbacks main_callbacks.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(SA-UTH_Callbacks PRIVATE SA-UnitTestHelper)
