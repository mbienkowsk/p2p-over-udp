#include "network/Listener.h"
#include "network/ThreadSafeHashMap.h"
#include "serialization/Utils.h"
#include "spdlog/spdlog.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

#include "cli/Cli.h"
#include "network/BroadcastSender.h"
#include "network/Downloader.h"
#include "network/UdpSender.h"
#include "resources/resource.h"
#include "serialization/Utils.h"
#include "log/Log.h"
#include <memory>
#include <unistd.h>

#define LISTENER_PORT 8000
#define PORT 12345
#define BROADCAST_ADDR "172.21.255.255"

int main() {
  auto b = BroadcastSender(PORT, BROADCAST_ADDR);
  auto broadcast_thread = b.make_worker([]() {
    // TODO: make real function for this
    return ResourceAnnounceMessage({"resource1", "resource2"});
  });

  UdpSender sender("0.0.0.0", 8000);
  sender.sendMessage(ResourceDataMessage(Header(MessageType::RESOURCE_DATA),
                                         "resource1",
                                         std::vector<std::byte>{std::byte(0)}));

  sender.sendMessage(ResourceDataMessage(Header(MessageType::RESOURCE_DATA),
                                         "resource2",
                                         std::vector<std::byte>{std::byte(0)}));

  broadcast_thread.join();

    spdlog::info("Exiting main");
    return 0;
}
