#include "Cli.h"
#include "../network/Downloader.h"
#include "../network/Message.h"
#include "../network/UdpSender.h"
#include "../network/constants.h"
#include "../resources/LocalResourceManager.h"
#include "../resources/PeerResourceMap.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

CLI::CLI(std::shared_ptr<LocalResourceManager> resourceManager,
         std::shared_ptr<PeerResourceMap> resourceMap)
    : localResourceManager(resourceManager), peerResourceMap(resourceMap) {};

void CLI::run() {
    const std::string HELP_STRING =
        "Available commands:\n"
        "  list-resources\n"
        "  find <filename>\n"
        "  download <host-ip> <filename>\n"
        "  change-resource-folder <new-folder-path>\n"
        "  help\n"
        "  exit\n\n";

    std::cout << "Welcome to the P2P File Sharing CLI!\n";
    std::cout << HELP_STRING;

    while (true) {
        std::cout << PROMPT;

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
        } else if (cmd == "list-resources") {
            handleListPeerResources();
        } else if (cmd == "find") {
            if (tokens.size() < 2) {
                std::cerr << "Usage: find <filename>\n";
            } else {
                handleFind(tokens[1]);
            }
        } else if (cmd == "download") {
            if (tokens.size() < 3) {
                std::cerr << "Usage: download <host-ip> <filename>\n";
            } else {
                handleDownload(tokens[1], tokens[2]);
            }
        } else if (cmd == "change-resource-folder") {
            if (tokens.size() < 2) {
                std::cerr
                    << "Usage: change-resource-folder <new-folder-path>\n";
            } else {
                handleChangeResourceFolder(tokens[1]);
            }
        } else if (cmd == "help") {
            std::cout << HELP_STRING;
        } else {
            std::cerr << "Unknown command: " << cmd << "\n";
        }
    }
}

void CLI::handleListPeerResources() {
    std::cout << "\nListing resources available for download:\n\n";
    std::cout << peerResourceMap->getAllResources() << "\n";
}

void CLI::handleFind(const std::string &filename) {
    std::vector<std::string> resourceHosts =
        peerResourceMap->getResourceHosts(filename);

    std::cout << std::endl;

    if (resourceHosts.empty()) {
        std::cout << "No hosts have the file '" << filename << "'\n";
    } else {
        int hostIndex = 1;
        std::cout << "The file '" << filename
                  << "' is available from the following hosts:\n";
        for (const auto &host : resourceHosts) {
            std::cout << hostIndex << ") " << host << "\n";
            ++hostIndex;
        }
    }
}

void CLI::handleDownload(const std::string &hostIp,
                         const std::string &filename) {
    auto downloader = Downloader::create(
        std::make_unique<UdpSender>(hostIp, PORT),
        std::make_unique<ResourceRequestMessage>(
            Header(MessageType::RESOURCE_REQUEST), filename),
        [peerResourceMap = this->peerResourceMap, filename, hostIp]() {
            std::cout << "Download failed: " << filename << std::endl;
            peerResourceMap->removeResourceFromPeer(hostIp, filename);
        },
        [peerResourceMap = this->peerResourceMap, filename, hostIp]() {
            std::cout << OOB_CODE << "Download completed: " << filename
                      << std::endl
                      << OOB_CODE << PROMPT << std::flush;
        });
    auto res = downloader->start();
    if (res)
        std::cout << "Downloading '" << filename << "' from " << hostIp << "\n";
}

void CLI::handleChangeResourceFolder(const std::string &newFolderPath) {
    if (localResourceManager->setResourceFolder(newFolderPath)) {
        std::cout << "Resource folder changed to: " << newFolderPath << "\n";
    } else {
        std::cerr << "Failed to change resource folder to: " << newFolderPath
                  << "\n"
                  << "Please check if the folder exists.\n"
                  << OOB_CODE << PROMPT << std::flush;
    }
}
