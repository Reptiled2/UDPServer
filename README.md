# UDP Server

A high-performance, asynchronous UDP server implementation in C++ using Boost.Asio for handling multiple client connections with session management and secure token-based authentication.

## 🚀 Features

- **Asynchronous I/O**: Built with Boost.Asio for non-blocking, high-performance network operations
- **Session Management**: Automatic peer tracking with unique session tokens
- **Packet-Based Communication**: Structured message protocol with different packet types
- **Connection Handling**: Automatic peer registration and connection management
- **Buffer Management**: Custom buffer system for efficient data serialization
- **Cross-Platform**: Compatible with Windows, Linux, and macOS

## 📋 Requirements

- **C++ Compiler**: C++20 or higher (MSVC, GCC, or Clang)
- **vcpkg**: For dependency management
- **Boost Libraries**:
  - boost-asio
  - boost-system

## 🔧 Installation

### Using vcpkg (Recommended)

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Reptiled2/UDPServer.git
   cd UDPServer
   ```

2. **Install dependencies**:
   ```bash
   vcpkg install
   ```

3. **Build the project**:
   ```bash
   # Using Visual Studio
   Open UDPServer.sln in Visual Studio and build

   # Or using MSBuild
   msbuild UDPServer.sln /p:Configuration=Release
   ```

### Manual Setup

1. Install Boost libraries:
   ```bash
   # Windows (vcpkg)
   vcpkg install boost-asio boost-system

   # Linux (apt)
   sudo apt-get install libboost-system-dev libboost-dev

   # macOS (homebrew)
   brew install boost
   ```

2. Build and link against Boost libraries in your build system.

## 🎯 Usage

### Basic Server Setup

```cpp
#include "udp_server.h"

int main() {
    const int PORT = 8080;
    
    try {
        UDPServer server(PORT);
        server.start();  // Blocks and runs the server
    }
    catch (const std::exception& e) {
    std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
  
    return 0;
}
```

### Client Connection

Clients can connect by sending a packet with `PacketTypes::CONNECT` to the server:

```cpp
// Client-side example (pseudo-code)
Packet connectPacket;
connectPacket.type = PacketTypes::CONNECT;
// Send to server...
```

## 📡 Protocol

### Packet Types

- `CONNECT (1)`: Client connection request
- `DISCONNECT (2)`: Client disconnection
- `MESSAGE (3)`: General message communication
- `KEEPALIVE (4)`: Connection keepalive
- `ALREADY_CONNECTED (5)`: Connection already exists response

### Packet Structure

```cpp
struct Packet {
    std::vector<int> peerId;    // Target peer IDs
    PacketTypes type;           // Message type
    BufferVector buffer;   // Message payload
};
```

### Connection Flow

1. **Client sends CONNECT packet**
2. **Server validates connection** (checks for existing connections)
3. **Server generates unique session token**
4. **Server creates Peer object** with unique ID and endpoint
5. **Server responds with CONNECT packet** containing peer ID and session token
6. **Connection established** - client can now send messages

## 🔒 Security Features

- **Session Tokens**: 32-character random tokens for each connection
- **Duplicate Connection Prevention**: Prevents multiple connections from same endpoint
- **Buffer Size Limits**: 2048-byte maximum buffer size to prevent overflow attacks

## 🛠️ Configuration

### Server Configuration

The server binds to `127.0.0.1` (localhost) by default.

### Buffer Limits

Default buffer size is 2048 bytes.

## 🐛 Debugging

### Enable Debug Output

The server includes console output for debugging:

- Connection attempts
- Message reception
- Error conditions

### Common Issues

1. **Port Already in Use**: Ensure no other application is using the server port
2. **Firewall Blocking**: Check firewall settings for UDP traffic
3. **Network Interface**: Verify the server is binding to the correct network interface

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details.

## 📞 Support

For questions, issues, or contributions:

- **GitHub Issues**: [Create an issue](https://github.com/Reptiled2/UDPServer/issues)

## 🏆 Acknowledgments

- Built with [Boost.Asio](https://www.boost.org/doc/libs/release/libs/asio/) for networking
- Inspired by modern C++ networking library [Enet](https://github.com/lsalzman/enet)