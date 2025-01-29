#ifndef LOCALRESOURCEMANAGER_H
#define LOCALRESOURCEMANAGER_H

#include <spdlog/spdlog.h>
#include <string>
#include <vector>

class LocalResourceManager {
  private:
    std::string folder_;
    static const int MAX_FILE_SIZE = 65501;

  public:
    LocalResourceManager(const std::string &folderPath);
    std::vector<std::string> listResources() const;
    bool resourceExists(const std::string &resourceName) const;
    bool saveResource(const std::string &resourceName,
                      const std::vector<std::byte> &content);
    std::vector<std::byte> getResource(const std::string &resourceName);
    bool removeResource(const std::string &resourceName);
    bool setResourceFolder(const std::string &newFolderPath);
    std::string getResourceFolder() const;
};

#endif
