# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# === Input ===

# Add executable target built from sources.
add_executable(UTH_Callbacks main_callbacks.cpp)



# === Dependencies ===

# Add library dependencies.
target_link_libraries(UTH_Callbacks PRIVATE SA-UnitTestHelper)
