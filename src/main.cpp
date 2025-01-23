#include "network/Listener.h"
#include "network/ThreadSafeHashMap.h"
#include "serialization/Utils.h"
#include "spdlog/spdlog.h"
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

#include "cli/Cli.h"
#include "log/Log.h"
#include "network/BroadcastSender.h"
#include "serialization/Utils.h"
#include <memory>
#include <unistd.h>

#define PORT 12345
#define BROADCAST_ADDR "172.21.255.255"
#define RESOURCE_FOLDER "../host_resources"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <UDP Listen Port>" << std::endl;
        return 1;
    }

    const int LISTENER_PORT = std::stoi(argv[1]);

    // If the second argument exits set logfile to it
    if (argc > 2) {
        setup_logger(true, argv[2]);
    } else {
        setup_logger(false);
    }

    setup_logger(true);

    auto b = BroadcastSender(PORT, BROADCAST_ADDR);

    auto localResourceManager =
        std::make_shared<LocalResourceManager>(RESOURCE_FOLDER);

    auto peerResourceMap = std::make_shared<PeerResourceMap>();

    auto listener =
        UdpListener(LISTENER_PORT, localResourceManager, peerResourceMap);

    auto broadcastStop = std::make_shared<std::atomic_bool>(false);
    auto broadcastThread =
        b.make_worker(broadcastStop, [broadcastStop, localResourceManager]() {
            return ResourceAnnounceMessage(
                localResourceManager->listResources());
        });

    CLI cli(localResourceManager, peerResourceMap);

    listener.start();
    auto listenerStop = std::make_shared<std::atomic_bool>(false);
    auto listenerThread = listener.detached_listen(listenerStop);

    cli.run();

    spdlog::info("Waiting for subthreads to finish.");

    if (listenerThread.joinable()) {
        listenerStop->store(true);
        listenerThread.join();
    }

    if (broadcastThread.joinable()) {
        broadcastStop->store(true);
        broadcastThread.join();
    }

    spdlog::info("Exiting main");
    return 0;
}
