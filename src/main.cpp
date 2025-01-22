#include "spdlog/spdlog.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

#include "cli/cli.h"
#include "network/BroadcastSender.h"
#include "resources/resource.h"
#include "serialization/Utils.h"
#include "log/log.h"
#include "network/UdpSender.h"

#define PORT 12345
#define BROADCAST_ADDR "172.21.255.255"
#define LISTEN_PORT 8000

int main() {

    bool write_to_file = true;

    setup_logger(write_to_file);

    auto b = BroadcastSender(PORT, BROADCAST_ADDR);
    auto broadcast_thread = b.make_worker([]() {
        // TODO: make real function for this
        return ResourceAnnounceMessage({"resource1", "resource2"});
    });
    
    broadcast_thread.detach();  // run in background

    runCLI();

    spdlog::info("Exiting main");
    return 0;
}
