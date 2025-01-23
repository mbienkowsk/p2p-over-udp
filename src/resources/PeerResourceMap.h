#ifndef PEERRESOURCEMAP_H
#define PEERRESOURCEMAP_H

#include <map>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>

class PeerResourceMap {
  public:
    void updateResources(const std::string &peerIP,
                         const std::vector<std::string> &resources);
    std::vector<std::string> getPeerResources(const std::string &peerIP);
    std::vector<std::string> getResourceHosts(const std::string &resource);
    std::map<std::string, std::vector<std::string>> getAllResources();

  private:
    std::map<std::string, std::vector<std::string>> resourceMap_;
    std::mutex mutex_;

    void removePeer(const std::string &peerIP);
    std::string join(const std::vector<std::string> &vec,
                     const std::string &delimiter);
};

std::ostream &
operator<<(std::ostream &os,
           const std::map<std::string, std::vector<std::string>> &PeerResourceMap);

#endif
