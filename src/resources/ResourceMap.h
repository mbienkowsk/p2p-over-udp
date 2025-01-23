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
        std::vector<std::string> getResourceHosts(const std::string& resource);
        std::map<std::string, std::vector<std::string>> getAllResources();


    private:
        std::map<std::string, std::vector<std::string>> resourceMap_;
        std::mutex mutex_;
        
        std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
};

std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::vector<std::string>> &resourceMap);

#endif
