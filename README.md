# P2P over UDP

This project is a peer-to-peer file sharing application implemented in C++. It allows multiple clients to share files directly over a local network, without the need for a central server. Each client can select files to share, discover other clients, and download files from them.

## Features

- Peer-to-peer file sharing between multiple clients
- Automatic discovery of other clients on the local network
- Selection of files to share via dedicated resource directories
- Download files from other clients
- Simple command-line interface for running, building, testing, and linting

## Usage

### Prerequisites

- C++ compiler (with C++17 support)
- [spdlog](https://github.com/gabime/spdlog) logging library (`sudo apt install libspdlog-dev`)
- [just](https://github.com/casey/just) command runner

## Usage

For detailed build and usage instructions, Docker usage, and advanced options, see [docs/build.md](build.md).
