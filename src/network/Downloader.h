#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "UdpSender.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <thread>

class Downloader : public std::enable_shared_from_this<Downloader> {
public:
  static std::unordered_map<std::string, std::shared_ptr<Downloader>>
      runningDownloads;
  static std::mutex mutex;

  static std::shared_ptr<Downloader>
  getRunningDownload(const std::string &resourceName) {
    std::lock_guard<std::mutex> lock(mutex);
    return runningDownloads[resourceName];
  }

  static std::shared_ptr<Downloader> create(const UdpSender &sender,
                                            const ResourceRequestMessage &msg) {
    return std::shared_ptr<Downloader>(new Downloader(sender, msg));
  }

  ~Downloader() {
    spdlog::trace("~Downloader()");
    stop();
  }

  std::optional<std::string> errorReason;

  bool start();
  void stop();

private:
  Downloader(const UdpSender &sender, const ResourceRequestMessage &msg)
      : sender(sender), msg(msg), stopFlag(false){};

  const UdpSender &sender;
  const ResourceRequestMessage &msg;
  std::atomic_bool stopFlag;
};

#endif // DOWNLOADER_H
