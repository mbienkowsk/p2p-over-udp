#include "ResourceManager.h"
#include <iostream>
#include <string>
#include <vector>

int main(){
    ResourceManager resourceManager("../host_resources");
    std::vector<std::string> resources = resourceManager.listResources();
    for (const auto& resource : resources) {
        spdlog::info("{}", resource);
    }
    spdlog::info("Resource folder: {}", resourceManager.getResourceFolder());
    if(resourceManager.resourceExists("text1.txt")){
        spdlog::info("Resource exists");
    } else {
        spdlog::info("Resource doesnt exist");
    }
    resourceManager.removeResource("text2.txt");
    std::cout << "Resource manager" << std::endl;
}