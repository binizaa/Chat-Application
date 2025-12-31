cmake_minimum_required(VERSION 3.14)
project(ChatServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(server/include)

# Find nlohmann_json
find_package(nlohmann_json QUIET)

# Source files
set(SOURCES
    server/src/main.cpp
    server/src/Client.cpp
    server/src/ClientManager.cpp
    server/src/core/NetworkServer.cpp
    server/src/core/KqueueEngine.cpp
)

# Executable
add_executable(chat_server ${SOURCES})

# Link libraries
if(nlohmann_json_FOUND)
    target_link_libraries(chat_server PRIVATE nlohmann_json::nlohmann_json)
else()
    # If not found via find_package, assume it's in the include path (manual install)
    # You might want to add a specific include directory if it's in a custom location
    # target_include_directories(chat_server PRIVATE server/include/external)
    message(STATUS "nlohmann_json not found via find_package, assuming header-only include or manual setup.")
endif()
