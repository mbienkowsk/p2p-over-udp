#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <spdlog/spdlog.h>

class ResourceManager {
    private:
        std::string folder_;
    public:
        ResourceManager(const std::string& folderPath);
        std::vector<std::string> listResources() const;
        bool resourceExists(const std::string& resourceName) const;
        void saveResource(const std::string& resourceName, const std::string& content);
        void removeResource(const std::string& resourceName);
        void setResourceFolder(const std::string& newFolderPath);
        std::string getResourceFolder() const;
};


#endif