#include "../resources/LocalResourceManager.h"
#include "../resources/PeerResourceMap.h"
#include "Cli.h"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    LocalResourceManager LocalResourceManager("../host_resources");
    PeerResourceMap PeerResourceMap;
    std::vector<std::string> resources1 = {"ResourceA", "ResourceB"};
    std::vector<std::string> resources2 = {"ResourceB", "ResourceC"};
    std::vector<std::string> resources3 = {"ResourceA"};

    PeerResourceMap.updateResources("Peer1", resources1);
    PeerResourceMap.updateResources("Peer2", resources2);
    PeerResourceMap.updateResources("Peer3", resources3);

    CLI CLI(LocalResourceManager, PeerResourceMap);

    CLI.run();
};
