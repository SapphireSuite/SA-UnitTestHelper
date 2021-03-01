# Copyright (c) 2021 Sapphire Development Team. All Rights Reserved.



# === Outputs ===

# Setup output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/UTH_Examples)		# .exe
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib/UTH_Examples)		# .dll / .so
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib/UTH_Examples)		# .lib / .a



# === EntryPoints ===

add_subdirectory(MacroEquals)
add_subdirectory(MacroSFunc)
add_subdirectory(MacroMFunc)
add_subdirectory(MacroOp)
add_subdirectory(Groups)
add_subdirectory(Callbacks)
add_subdirectory(Success)
add_subdirectory(Failure)
