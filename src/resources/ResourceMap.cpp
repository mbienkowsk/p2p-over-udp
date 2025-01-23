#include "ResourceMap.h"
#include <map>
#include <set>
#include <string>
#include <spdlog/spdlog.h>
#include <mutex>

void ResourceMap::updateResources(const std::string& senderIP, const std::vector<std::string>& resources){
    std::lock_guard<std::mutex> lock(mutex_);
    resourceMap_[senderIP] = resources;
    spdlog::info("Updated resources for peer {}: {}", senderIP, join(resources, ", "));

}

std::vector<std::string> ResourceMap::getPeerResources(const std::string& peerIP){
    std::lock_guard<std::mutex> lock(mutex_);
    if (resourceMap_.find(peerIP) != resourceMap_.end()){
        return resourceMap_[peerIP];
    }
    return {};
}

std::map<std::string, std::vector<std::string>> ResourceMap::getAllResources() {
    std::lock_guard<std::mutex> lock(mutex_);
    return resourceMap_;
}

std::string ResourceMap::join(const std::vector<std::string>& vec, const std::string& delimiter) {
    std::string result;
    for (const auto& elem : vec){
        if (!result.empty()) result += delimiter;
        result += elem;
    }
    return result;
}
