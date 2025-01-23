#include "network/BroadcastSender.h"
#include "network/Listener.h"
#include "serialization/Utils.h"
#include "spdlog/spdlog.h"
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

#include "cli/Cli.h"
#include "log/Log.h"
#include "network/constants.h"
#include "serialization/Utils.h"
#include <memory>
#include <unistd.h>

#define RESOURCE_FOLDER "../host_resources"

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

int handleArgs(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <UDP Listen Port>" << std::endl;
        exit(1);
    }

    const int port = std::stoi(argv[1]);

    // If the second argument exits set logfile to it
    if (argc > 2) {
        setup_logger(true, argv[2]);
    } else {
        setup_logger(false, "");
    }

    return port;
}

int main(int argc, char *argv[]) {
    handleArgs(argc, argv);

    // Initialize all components
    auto broadcastSender = BroadcastSender(PORT, BROADCAST_ADDR);
    auto localResourceManager =
        std::make_shared<LocalResourceManager>(RESOURCE_FOLDER);
    auto peerResourceMap = std::make_shared<PeerResourceMap>();
    auto listener = UdpListener(PORT, localResourceManager, peerResourceMap);

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
