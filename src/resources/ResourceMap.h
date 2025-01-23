#ifndef RESOURCEMAP_H
#define RESOURCEMAP_H

#include <map>
#include <set>
#include <string>
#include <spdlog/spdlog.h>
#include <mutex>

class ResourceMap{
    public:
        void updateResources(const std::string& senderIP, const std::vector<std::string>& resources);
        std::vector<std::string> getPeerResources(const std::string& peerIP);

        std::map<std::string, std::vector<std::string>> getAllResources();

    private:
        std::map<std::string, std::vector<std::string>> resourceMap_;
        std::mutex mutex_;
        
        std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
};
#endif
