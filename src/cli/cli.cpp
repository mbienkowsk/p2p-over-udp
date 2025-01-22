#include "cli.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>


void runCLI() {
    std::cout << "Welcome to the P2P File Sharing CLI!\n";
    std::cout << "Available commands:\n"
              << "  list-resources\n"
              << "  find <filename>\n"
              << "  download <host-ip> <filename>\n"
              << "  exit\n\n";

    while (true) {
        std::cout << "> ";

        // Read a line of input from the user
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) {
            continue;
        }

        // Split the line into tokens
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            continue;
        }

        // The command is the first token
        const auto &cmd = tokens[0];

        // Dispatch to command handlers
        if (cmd == "exit") {
            std::cout << "Exiting CLI.\n";
            break;
        } 
        else if (cmd == "list-resources") {
            handleListResources();
        }
        else if (cmd == "find") {
            if (tokens.size() < 2) {
                std::cerr << "Usage: find <filename>\n";
            } else {
                handleFind(tokens[1]);
            }
        }
        else if (cmd == "download") {
            if (tokens.size() < 3) {
                std::cerr << "Usage: download <host-ip> <filename>\n";
            } else {
                handleDownload(tokens[1], tokens[2]);
            }
        }
        else {
            std::cerr << "Unknown command: " << cmd << "\n";
        }
    }
}

void handleListResources() {
    std::cout << "[Stub] Listing local resources...\n";
}

void handleFind(const std::string &filename) {
    UdpSender sender(BROADCAST_ADDR, DISCOVERY_PORT);

    ResourceRequestMessage request(filename);

    sender.sendMessage(requestMsg);

    std::cout << "Broadcasted request for file: '" << filename << "'\n";

    
}

void handleDownload(const std::string &hostIp, const std::string &filename) {
    std::cout << "[Stub] Downloading '" << filename 
              << "' from host: " << hostIp << "\n";
}
