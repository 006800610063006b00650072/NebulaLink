# **NebulaLink**

**NebulaLink** is a cross-platform P2P networking solution that empowers secure communication, file sharing, and command execution across devices behind NAT, using modern technologies like ICE for NAT traversal and TLS for encryption.

---

## **Features**

- **P2P Communication**: Direct connections between devices, even behind NAT.
- **Cross-Platform Compatibility**: Runs seamlessly on Windows and Unix-like systems.
- **File Sharing**: Easily transfer files between peers.
- **Secure Shell Access**: Execute remote commands using PowerShell on Windows and Bash on Unix.
- **Encryption**: End-to-end TLS encryption ensures security.
- **Dynamic NAT Traversal**: Uses ICE (Interactive Connectivity Establishment) to bypass NAT.

---

## **Getting Started**

### **Prerequisites**

- C++17 or higher
- Boost Libraries
- OpenSSL
- libnice (for ICE)

### **Installation**

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/NebulaLink.git
   cd NebulaLink
Install dependencies:

Ubuntu/Debian:
bash
Copy code
sudo apt update
sudo apt install build-essential libboost-all-dev libssl-dev libglib2.0-dev libnice-dev
Windows:
Install Boost and OpenSSL using a package manager like vcpkg.
MacOS:
bash
Copy code
brew install boost openssl libnice
Build the project:

bash
Copy code
mkdir build
cd build
cmake ..
make
Run the application:

bash
Copy code
./NebulaLink
Usage
Start the application on multiple devices.
Exchange peer GUIDs to establish a P2P connection.
Use the secure shell interface for file transfer or remote command execution.
Examples
Secure Shell
Run the application on one device and connect to it from another:

bash
Copy code
Welcome to the secure shell.
Type commands to execute them remotely. Type 'exit' to close.
> ls -al
> exit
Goodbye!
File Sharing
Upload or download files using the integrated file transfer functionality.

Contributing
Contributions are welcome! Please fork the repository, create a new branch for your feature, and submit a pull request.

License
This project is licensed under the MIT License. See LICENSE for more details.

Acknowledgements
Boost C++ Libraries
OpenSSL
libnice
PowerShell and Bash
