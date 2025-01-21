#include "spdlog/spdlog.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

#include "network/BroadcastSender.h"
#include "resources/resource.h"
#include "serialization/Utils.h"
#include "network/UdpSender.h"

#define PORT 12345
#define BROADCAST_ADDR "172.21.255.255"

int main()
{
  auto b = BroadcastSender(PORT, BROADCAST_ADDR);
  auto broadcast_thread = b.make_worker([]()
                                        {
    // TODO: make real function for this
    return ResourceAnnounceMessage({"resource1", "resource2"}); });

  broadcast_thread.join();
  spdlog::info("Exiting main");
}
