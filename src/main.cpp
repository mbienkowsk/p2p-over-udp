#include "network/Listener.h"
#include "network/ThreadSafeHashMap.h"
#include "serialization/Utils.h"
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
#include <memory>
#include <unistd.h>

#define LISTENER_PORT 8000
#define PORT 12345
#define BROADCAST_ADDR "172.21.255.255"
#define LISTEN_PORT 8000

using SMap = std::shared_ptr<ThreadSafeHashMap>;

std::thread create_worker(SABool stop_condition, int id) {
  return std::thread([stop_condition, id] {
    auto i = 0;
    while (!*stop_condition) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      spdlog::info("Loop {}, worker {}", i++, id);
    }
    spdlog::info("Exiting subthread\n");
  });
}

int main() {

  UdpListener::runningDownloads = std::make_shared<ThreadSafeHashMap>();

  auto stop_1 = std::make_shared<std::atomic_bool>(false);
  auto stop_2 = std::make_shared<std::atomic_bool>(false);

  auto worker_1 = create_worker(stop_1, 1);
  auto worker_2 = create_worker(stop_2, 2);

  auto ds = UdpListener::runningDownloads;

  ds->insert("1", stop_1);
  ds->insert("2", stop_2);

  sleep(2);
  ds->makeTrue("1");
  sleep(2);
  ds->makeTrue("2");

  if (worker_1.joinable()) {
    worker_1.join();
  }
  if (worker_2.joinable()) {
    worker_2.join();
  }

  auto listener = UdpListener(LISTENER_PORT);

  spdlog::info("Exiting main");
  return 0;
}
