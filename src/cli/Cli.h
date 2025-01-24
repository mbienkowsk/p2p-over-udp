#ifndef CLI_H
#define CLI_H

#include "../resources/LocalResourceManager.h"
#include "../resources/PeerResourceMap.h"
#include <memory>

const std::string OOB_CODE = "\"\r\033[K";
const std::string PROMPT = "> ";

class CLI {
  public:
    CLI(std::shared_ptr<LocalResourceManager> resourceManager,
        std::shared_ptr<PeerResourceMap> resourceMap);
    void run();

  private:
    void handleListPeerResources();
    void handleFind(const std::string &filename);
    void handleDownload(const std::string &hostIp, const std::string &filename);
    void handleChangeResourceFolder(const std::string &newFolderPath);

    std::shared_ptr<LocalResourceManager> localResourceManager;
    std::shared_ptr<PeerResourceMap> peerResourceMap;
};
;

#endif // CLI_H
