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

    static std::shared_ptr<Downloader> create(
        std::unique_ptr<UdpSender> sender,
        std::unique_ptr<ResourceRequestMessage> msg,
        std::function<void()> onFailure = []() {},
        std::function<void()> onSuccess = []() {}) {
        return std::shared_ptr<Downloader>(new Downloader(
            std::move(sender), std::move(msg), onFailure, onSuccess));
    }

    ~Downloader() {
        spdlog::trace("~Downloader()");
        stop();
    }

    std::optional<std::string> errorReason;

    // via the cli, now it's silent
    bool start();
    void stop();

  private:
    Downloader(
        std::unique_ptr<UdpSender> sender,
        std::unique_ptr<ResourceRequestMessage> msg,
        std::function<void()> onFailure = []() {},
        std::function<void()> onSuccess = []() {})
        : sender(std::move(sender)), msg(std::move(msg)), onFailure(onFailure),
          onSuccess(onSuccess){};

    std::shared_ptr<std::atomic_bool> stopFlag =
        std::make_shared<std::atomic_bool>(false);
    std::unique_ptr<UdpSender> sender;
    std::unique_ptr<ResourceRequestMessage> msg;
    std::function<void()> onFailure;
    std::function<void()> onSuccess;
};

#endif // DOWNLOADER_H
