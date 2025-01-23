#include "ResourceManager.h"

ResourceManager::ResourceManager(const std::string& folderPath)
    : folder_(folderPath) {
    // Check if the folder exists
    if (!std::filesystem::exists(folder_) || !std::filesystem::is_directory(folder_)) {
        throw std::invalid_argument("incorrect path");
    }
}

// Removing resource
void ResourceManager::removeResource(const std::string& resourceName) {
    std::string filePath = folder_ + "/" + resourceName;

    // Check if the resource exists
    if (!std::filesystem::exists(filePath)) {
        spdlog::warn("Cant delete {}, resource not found", resourceName);
        return;
    }

    // Delete file
    if (std::filesystem::remove(filePath)) {
        spdlog::info("Deleted {}", resourceName);
    } else {
        spdlog::error("Couldnt delete {}", resourceName);
    }
}

// Listing all resources
std::vector<std::string> ResourceManager::listResources() const {
    std::vector<std::string> resourceNames;

    // check all resources in folder
    for (const auto& entry : std::filesystem::directory_iterator(folder_)) {
        if (entry.is_regular_file()) {
            resourceNames.push_back(entry.path().filename().string());
        }
    }

    return resourceNames;
}

// check if resource exists
bool ResourceManager::resourceExists(const std::string& resourceName) const {
    std::string filePath = folder_ + "/" + resourceName;
    return std::filesystem::exists(filePath);
}

// Set a new resource folder
void ResourceManager::setResourceFolder(const std::string& newFolderPath) {
    if (!std::filesystem::exists(newFolderPath) || !std::filesystem::is_directory(newFolderPath)) {
        spdlog::error("Incorrect path", newFolderPath);
        return;
    }

    folder_ = newFolderPath;
    spdlog::info("Changed resource folder to {}", newFolderPath);
}

std::string ResourceManager::getResourceFolder() const {
    return folder_;
}