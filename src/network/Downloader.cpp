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

    auto stopFlag = this->stopFlag;
    auto sender = std::move(this->sender);
    auto msg = std::move(this->msg);

    std::thread worker([stopFlag, sender = std::move(sender),
                        msg = std::move(msg)]() {
        size_t retry = 0;
        while (!*stopFlag) {
            if (retry >= 5) {
                spdlog::error(
                    "Couldn't download: '{}', after {} retries. Aborting.",
                    msg->resource_name, retry);
                *stopFlag = true;
                break;
            }
            if (retry > 0) {
                spdlog::info("Couldn't download: '{}', done {} retries.",
                             msg->resource_name, retry);
            }
            try {
                sender->sendMessage(*msg);
            } catch (const std::exception &ex) {
                *stopFlag = true;
                spdlog::error("An error occured when downloading resource '{}'",
                              msg->resource_name);
                break;
            }
            retry++;
            if (*stopFlag) {
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
    *stopFlag = true;
    spdlog::trace("stop called");
}
