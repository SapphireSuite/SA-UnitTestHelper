# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.


# === Output ===
# Add executable target built from sources.
add_executable(UnitTestSucess main_success.cpp)

# === Testing ===
# Create CTest that run UnitTestSuccess exe.
add_test(NAME CUnitTestSuccess COMMAND UnitTestSucess --config $<CONFIGURATION> --exe $<TARGET_FILE:UnitTestSucess>)