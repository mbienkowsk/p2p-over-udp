#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <spdlog/spdlog.h>
#include <string>
#include <vector>

class ResourceManager {
  private:
    std::string folder_;

  public:
    ResourceManager(const std::string &folderPath);
    std::vector<std::string> listResources() const;
    bool resourceExists(const std::string &resourceName) const;
    void saveResource(const std::string &resourceName,
                      const std::vector<std::byte> &content);
    std::vector<std::byte> getResource(const std::string &resourceName);
    void removeResource(const std::string &resourceName);
    void setResourceFolder(const std::string &newFolderPath);
    std::string getResourceFolder() const;
};

#endif
