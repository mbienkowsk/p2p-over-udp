#include "LocalResourceManager.h"
#include "PeerResourceMap.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    LocalResourceManager LocalResourceManager("../host_resources");
    std::vector<std::string> resources = LocalResourceManager.listResources();
    for (const auto &resource : resources) {
        spdlog::info("{}", resource);
    }
    spdlog::info("Resource folder: {}", LocalResourceManager.getResourceFolder());
    if (LocalResourceManager.resourceExists("text1.txt")) {
        spdlog::info("Resource exists");
    } else {
        spdlog::info("Resource doesnt exist");
    }
    LocalResourceManager.removeResource("text2.txt");
    std::cout << "Resource manager" << std::endl;
}