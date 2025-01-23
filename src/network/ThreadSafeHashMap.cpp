#include "ThreadSafeHashMap.h"
#include <spdlog/spdlog.h>

void ThreadSafeHashMap::insert(const std::string &key, SABool value) {
    std::lock_guard<std::mutex> lock(mtx);
    // Insert or update the shared_ptr directly
    map[key] = value;
}

std::optional<bool> ThreadSafeHashMap::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = map.find(key);
    if (it != map.end()) {
        // Retrieve the value using shared_ptr and atomic_bool
        return it->second->load();
    }
    return std::nullopt;
}

void ThreadSafeHashMap::erase(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);
    map.erase(key);
}

void ThreadSafeHashMap::makeTrue(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = map.find(key);
    if (it != map.end()) {
        // Set the value to true using shared_ptr and atomic_bool
        it->second->store(true);
    } else {
        spdlog::warn("Key {} not found", key);
    }
};
