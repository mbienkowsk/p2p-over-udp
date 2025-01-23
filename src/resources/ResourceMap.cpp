#include "ResourceMap.h"
#include <map>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>

void ResourceMap::updateResources(const std::string &peerIP,
                                  const std::vector<std::string> &resources) {
    std::lock_guard<std::mutex> lock(mutex_);
    resourceMap_[peerIP] = resources;
    spdlog::info("Updated resources for peer {}: {}", peerIP,
                 join(resources, ", "));
}

std::vector<std::string>
ResourceMap::getPeerResources(const std::string &peerIP) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (resourceMap_.find(peerIP) != resourceMap_.end()) {
        return resourceMap_[peerIP];
    }
    spdlog::warn("Peer {} not found", peerIP);
    return {};
}

std::vector<std::string>
ResourceMap::getResourceHosts(const std::string &searchedResource) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> resourceHosts;
    for (const auto &entry : resourceMap_) {
        const auto &resources = entry.second;

        if (std::find(resources.begin(), resources.end(), searchedResource) !=
            resources.end()) {
            resourceHosts.push_back(entry.first);
        }
    }

    return resourceHosts;
}

void ResourceMap::removePeer(const std::string &peerIP) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (resourceMap_.erase(peerIP) > 0) {
        spdlog::info("Removed peer {}", peerIP);
    } else {
        spdlog::warn("Cannot remove, peer {} not found", peerIP);
    }
}

std::map<std::string, std::vector<std::string>> ResourceMap::getAllResources() {
    std::lock_guard<std::mutex> lock(mutex_);
    return resourceMap_;
}

std::string ResourceMap::join(const std::vector<std::string> &vec,
                              const std::string &delimiter) {
    std::string joined;
    for (const auto &elem : vec) {
        if (!joined.empty())
            joined += delimiter;
        joined += elem;
    }
    return joined;
}

std::ostream &
operator<<(std::ostream &os,
           const std::map<std::string, std::vector<std::string>> &resourceMap) {
    for (const auto &pair : resourceMap) {
        os << pair.first << ": ";
        for (const auto &resource : pair.second) {
            os << resource << " ";
        }
        os << std::endl;
    }
    return os;
}
