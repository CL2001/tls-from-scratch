cmake_minimum_required(VERSION 3.10)

# Name of your project
project(http_from_scratch)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory (optional if you separate headers)
include_directories(include)

# Add the executable
add_executable(server
    src/server.cpp
    src/encryption_hash.cpp
    src/helper.cpp
)

# Add the executable
add_executable(client
    src/client.cpp
    src/encryption_hash.cpp
    src/helper.cpp
)