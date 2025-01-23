#ifndef CLI_H
#define CLI_H

#include "../resources/LocalResourceManager.h"
#include "../resources/PeerResourceMap.h"

class CLI {
public:
    CLI(LocalResourceManager& resourceManager, PeerResourceMap& resourceMap);
    void run();

private:
    void handleListPeerResources();
    void handleFind(const std::string& filename);
    void handleDownload(const std::string& hostIp, const std::string& filename);
    void handleChangeResourceFolder(const std::string& newFolderPath);

    LocalResourceManager& localResourceManager;
    PeerResourceMap& peerResourceMap;
};

#endif // CLI_H