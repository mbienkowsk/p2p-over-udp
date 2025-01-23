#include "Downloader.h"
#include <chrono>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>

std::unordered_map<std::string, std::shared_ptr<Downloader>>
    Downloader::runningDownloads;
std::mutex Downloader::mutex;

bool Downloader::start() {
    std::lock_guard<std::mutex> lock(Downloader::mutex);

    if (Downloader::runningDownloads.find(msg->resource_name) !=
        Downloader::runningDownloads.end()) {
        spdlog::info("Resource {} is already being downloaded",
                     msg->resource_name);
        this->errorReason = "Resource is already being downloaded";
        return false;
    }

    Downloader::runningDownloads[msg->resource_name] = shared_from_this();

    std::thread worker([this]() {
        size_t retry = 0;
        while (!this->stopFlag) {
            if (retry >= 5) {
                spdlog::error(
                    "Couldn't download: '{}', after {} retries. Aborting.",
                    this->msg->resource_name, retry);
                // TODO: failing to download resource due to unavailable host
                // segfaults on exit
                this->stopFlag = true;
                this->errorReason = "Failed to send message after 5 retries";
                return;
            }
            if (retry > 0) {
                spdlog::info("Couldn't download: '{}', done {} retries.",
                             this->msg->resource_name, retry);
            }
            try {
                sender->sendMessage(*msg);
            } catch (const std::exception &ex) {
                this->errorReason = ex.what();
                this->stopFlag = true;
                spdlog::error(
                    "An error occured when downloading resource '{}': {}",
                    this->msg->resource_name, this->errorReason.value());
                return;
            }
            retry++;
            if (this->stopFlag) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        spdlog::trace("Exiting thread");
        Downloader::runningDownloads.erase(msg->resource_name);
    });
    worker.detach();

    return true;
}

void Downloader::stop() {
    stopFlag = true;
    spdlog::trace("stop called");
}
