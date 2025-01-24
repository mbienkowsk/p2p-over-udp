#include "cli/Cli.h"
#include "log/Log.h"
#include "network/BroadcastSender.h"
#include "network/Listener.h"
#include "network/constants.h"
#include "serialization/Utils.h"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <ifaddrs.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#define RESOURCE_FOLDER "./test"

/// Cleanup function that waits for the subthreads to finish and logs
/// the progress
void cleanup(std::thread &listenerThread, std::thread &broadcastThread,
             std::shared_ptr<std::atomic_bool> listenerStop,
             std::shared_ptr<std::atomic_bool> broadcastStop) {
    spdlog::info("Waiting for subthreads to finish.");

    // Stop the listener
    if (listenerThread.joinable()) {
        listenerStop->store(true);
        listenerThread.join();
    }
    spdlog::info("Listener thread finished");

    // Stop the broadcast sender
    if (broadcastThread.joinable()) {
        broadcastStop->store(true);
        broadcastThread.join();
    }
    spdlog::info("Broadcast thread finished");
    spdlog::info("Both subthreads finished, exiting the program");
}

std::string getAbsolutePath(const std::string &relative_path) {
    try {
        std::filesystem::path absolute_path =
            std::filesystem::canonical(relative_path);
        return absolute_path.string();
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Couldn't access '" << relative_path << "' folder"
                  << std::endl;
        exit(1);
    }
}

/*
  Returns resource folder path from the command line arguments
*/
std::string handleArgs(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Resource folder path>"
                  << std::endl;
        exit(1);
    }

    std::string resourceFolder = getAbsolutePath(argv[1]);

    // If the second argument exits set logfile to it
    if (argc > 2) {
        setup_logger(true, argv[2]);
    } else {
        setup_logger(false, "");
    }

    return resourceFolder;
}

// TODO: this is helpful for debugging, but should be factored out to some
// helper instead of hogging main
void listIPAddresses() {
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;

    if (getifaddrs(&ifAddrStruct) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {
            // IPv4 address
            char addrBuffer[INET_ADDRSTRLEN];
            void *addrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            inet_ntop(AF_INET, addrPtr, addrBuffer, INET_ADDRSTRLEN);
            spdlog::warn("Interface: {} Address: {}", ifa->ifa_name,
                         addrBuffer);
        }
        // If you also want IPv6, check for AF_INET6
    }

    if (ifAddrStruct) {
        freeifaddrs(ifAddrStruct);
    }
}

// TODO: same as listIPAddresses, move this somewhere
std::string getEth0Address() {
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;

    if (getifaddrs(&ifAddrStruct) == -1) {
        // Failed to get interface addresses
        return "";
    }

    std::string eth0Address;

    // Walk through list of interfaces
    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue; // No address here
        }

        // Look specifically for eth0 + IPv4
        if (std::strcmp(ifa->ifa_name, "eth0") == 0 &&
            ifa->ifa_addr->sa_family == AF_INET) {

            char addrBuffer[INET_ADDRSTRLEN];
            void *addrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            // Convert the binary IP to a string
            if (inet_ntop(AF_INET, addrPtr, addrBuffer, INET_ADDRSTRLEN) !=
                nullptr) {
                eth0Address = addrBuffer;
                break; // Found it, break out
            }
        }
    }

    // Cleanup
    freeifaddrs(ifAddrStruct);
    if (eth0Address.empty()) {
        spdlog::warn("Failed to find eth0 address");
        throw std::runtime_error("Failed to find eth0 address");
    }

    return eth0Address; // Empty string if not found
}

int main(int argc, char *argv[]) {
    std::string resourceFolder = handleArgs(argc, argv);
    spdlog::info("Sharing resources from folder: {}", resourceFolder);

    auto address = getEth0Address();
    spdlog::info("Eth0 address: {}", address);

    // Initialize all components
    auto broadcastSender = BroadcastSender(PORT, BROADCAST_ADDR);
    auto localResourceManager =
        std::make_shared<LocalResourceManager>(resourceFolder);
    auto peerResourceMap = std::make_shared<PeerResourceMap>();
    auto listener =
        UdpListener(PORT, localResourceManager, peerResourceMap, address);

    // Initialize stop conditions for subthreads
    auto broadcastStop = std::make_shared<std::atomic_bool>(false);
    auto listenerStop = std::make_shared<std::atomic_bool>(false);

    // Launch the threads
    auto broadcastThread =
        broadcastSender.make_worker(broadcastStop, localResourceManager);

    listener.start();
    auto listenerThread = listener.detached_listen(listenerStop);

    // Run the cli in interactive mode
    auto cli = CLI(localResourceManager, peerResourceMap);
    cli.run();

    cleanup(listenerThread, broadcastThread, listenerStop, broadcastStop);
    return 0;
}
