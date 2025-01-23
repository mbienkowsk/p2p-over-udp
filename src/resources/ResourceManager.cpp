#include "ResourceManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <vector>

ResourceManager::ResourceManager(const std::string &folderPath)
    : folder_(folderPath) {
    // Check if the folder exists
    if (!std::filesystem::exists(folder_) ||
        !std::filesystem::is_directory(folder_)) {
        throw std::invalid_argument("incorrect path");
    }
}

// Listing all resources
std::vector<std::string> ResourceManager::listResources() const {
    std::vector<std::string> resourceNames;

    // check all resources in folder
    for (const auto &entry : std::filesystem::directory_iterator(folder_)) {
        if (entry.is_regular_file()) {
            resourceNames.push_back(entry.path().filename().string());
        }
    }

    return resourceNames;
}

// check if resource exists
bool ResourceManager::resourceExists(const std::string &resourceName) const {
    std::string filePath = folder_ + "/" + resourceName;
    return std::filesystem::exists(filePath);
}

void ResourceManager::saveResource(const std::string &resourceName,
                                   const std::vector<std::byte> &content) {
    std::string filePath = folder_ + "/" + resourceName;

    // Check if it already exists
    if (std::filesystem::exists(filePath)) {
        spdlog::warn("Resource {} already saved", resourceName);
        return;
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Couldnt write resource: " << filePath << '\n';
        return;
    }

    // Write the data to the file
    file.write(reinterpret_cast<const char *>(content.data()), content.size());

    if (!file) {
        std::cerr << "Couldnt save resource: " << resourceName << '\n';
    }
}

std::vector<std::byte>
ResourceManager::getResource(const std::string &resourceName) {
    std::string filePath = folder_ + "/" + resourceName;

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Cannot open resource" << resourceName << '\n';
        return {};
    }

    // Get the size of the file
    std::streamsize fileSize = file.tellg();
    if (fileSize <= 0) {
        std::cerr << "Empty resource: " << resourceName << '\n';
        return {};
    }

    // Resize the vector to hold the file content
    std::vector<std::byte> resourceContent(static_cast<size_t>(fileSize));

    // Seek back to the beginning of the file
    file.seekg(0, std::ios::beg);

    // Read the file into the vector
    if (!file.read(reinterpret_cast<char *>(resourceContent.data()),
                   fileSize)) {
        std::cerr << "Cannot read resource: " << resourceName << '\n';
        return {};
    }

    return resourceContent;
}

// Removing resource
void ResourceManager::removeResource(const std::string &resourceName) {
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

// Set a new resource folder
void ResourceManager::setResourceFolder(const std::string &newFolderPath) {
    if (!std::filesystem::exists(newFolderPath) ||
        !std::filesystem::is_directory(newFolderPath)) {
        spdlog::error("Incorrect path", newFolderPath);
        return;
    }

    folder_ = newFolderPath;
    spdlog::info("Changed resource folder to {}", newFolderPath);
}

std::string ResourceManager::getResourceFolder() const { return folder_; }
