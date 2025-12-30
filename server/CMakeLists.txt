cmake_minimum_required(VERSION 3.12)
project(MyProjectName CXX)

# 1. Gather all .cpp files recursively from the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 2. Define the executable using the list of sources found
add_executable(server_app ${SOURCES})

# 3. Include your headers directory
target_include_directories(server_app PRIVATE include)