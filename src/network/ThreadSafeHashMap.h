
#ifndef THREAD_SAFE_HASH_MAP_H
#define THREAD_SAFE_HASH_MAP_H

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

using SABool = std::shared_ptr<std::atomic_bool>;

class ThreadSafeHashMap {
  private:
    // Map of strings to atomic booleans
    std::unordered_map<std::string, SABool> map;
    std::mutex mtx;

  public:
    // Default constructor
    ThreadSafeHashMap() = default;

    // Destructor
    ~ThreadSafeHashMap() = default;

    // Insert a key with a boolean value
    void insert(const std::string &key, SABool value);

    // Get the value associated with a key
    std::optional<bool> get(const std::string &key);

    // Erase a key-value pair
    void erase(const std::string &key);

    /// Make the value associated with a key true
    void makeTrue(const std::string &key);
};

#endif // THREAD_SAFE_HASH_MAP_H
