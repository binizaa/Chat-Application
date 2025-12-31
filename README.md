# Chat Server (C++ / Kqueue)

This is a high-performance, non-blocking chat server written in modern C++ and designed for macOS/BSD systems using `kqueue` for efficient event notification.

## Features

*   **High Performance**: Uses multiplexed I/O with `kqueue` to handle multiple connections simultaneously without blocking the main thread.
*   **Modular Architecture**:
    *   `NetworkServer`: Handles low-level socket creation and configuration.
    *   `KqueueEngine`: Abstracts the complexity of `kevent` system calls.
    *   `ClientManager`: Manages client logic and message broadcasting.
*   **Modern Memory Management**: Extensive use of `std::unique_ptr` for memory safety and clear resource ownership.

## Prerequisites

This project uses the [nlohmann/json](https://github.com/nlohmann/json) library for message handling.

### Installing Dependencies

**Option A: Manual Download (Recommended)**
Download the `json.hpp` file from the official repository and place it in the following path:
`server/include/external/nlohmann/json.hpp` (You may need to create these directories).

**Option B: Package Managers**
- **macOS:** `brew install nlohmann-json`
- **Ubuntu/Debian:** `sudo apt-get install nlohmann-json3-dev`

## Project Structure

```
server/
├── include/
│   ├── Client.h
│   ├── ClientManager.h
│   └── core/
│       ├── KqueueEngine.h
│       └── NetworkServer.h
└── src/
    ├── Client.cpp
    ├── ClientManager.cpp
    ├── main.cpp
    └── core/
        ├── KqueueEngine.cpp
        └── NetworkServer.cpp
```

## Build Instructions

You can build the project using **CMake** (recommended) or directly with `clang++`.

### Option 1: Using CMake

1.  Create a build directory:
    ```bash
    mkdir build && cd build
    ```
2.  Generate build files:
    ```bash
    cmake ..
    ```
3.  Compile:
    ```bash
    make
    ```

### Option 2: Using clang++ directly

Run the following command from the project root:

```bash
clang++ -std=c++14 \
    server/src/main.cpp \
    server/src/Client.cpp \
    server/src/ClientManager.cpp \
    server/src/core/NetworkServer.cpp \
    server/src/core/KqueueEngine.cpp \
    -I server/include \
    -o chat_server
```

## Running the Server

Once compiled, start the server:

```bash
# If built with CMake (inside build/ directory)
./chat_server

# If built with clang++ (from root)
./chat_server
```

You should see a message indicating the server is running on port `8080`.

## Usage

To connect to the server and test the chat, you can use `netcat` (`nc`) from multiple terminal windows:

```bash
nc localhost 8080
```

Type messages in one window and see them broadcasted to other connected clients.
