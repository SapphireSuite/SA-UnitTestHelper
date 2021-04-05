# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.
# This script must be run from the root folder.

config=$1

# Create generator folder.
mkdir -p Build/Ninja/$config

# Generate project.
cmake -B Build/Ninja/$config -DCMAKE_BUILD_TYPE=$config -G Ninja

# Build project.
cmake --build Build/Ninja/$config --config $config --verbose

# ctest must be triggered in built folder.
cd Build/Ninja/$config

# Execute ctests.
ctest -C $config -V

# Move back to root folder.
cd ../../../