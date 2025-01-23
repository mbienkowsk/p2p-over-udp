#ifndef CLI_H
#define CLI_H

#include "../resources/ResourceManager.h"

class CLI {
public:
    CLI(ResourceManager& resourceManager);
    void run();

private:
    void handleListResources();
    void handleFind(const std::string& filename);
    void handleDownload(const std::string& hostIp, const std::string& filename);

    ResourceManager& resourceManager;
};

#endif // CLI_H