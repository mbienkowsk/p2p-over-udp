#ifndef CLI_H
#define CLI_H

#include <string>

void runCLI();

void handleListResources();

void handleFind(const std::string &filename);

void handleDownload(const std::string &hostIp, const std::string &filename);

#endif // CLI_H
