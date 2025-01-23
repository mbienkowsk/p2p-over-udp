#include "Listener.h"
#include "Downloader.h"
#include "UdpSender.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstring>
#include <fmt/ranges.h>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

UdpListener::UdpListener(
    int port, std::shared_ptr<LocalResourceManager> localResourceManager,
    std::shared_ptr<PeerResourceMap> peerResourceMap)
    : peerResourceMap(peerResourceMap),
      localResourceManager(localResourceManager), port(port), sockfd(-1) {}

UdpListener::~UdpListener() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void UdpListener::start() {
    tryCreateSocket();
    tryBindSocket();
    setSockOptions();
    spdlog::info("Listening on port {} for incoming UDP traffic...\n", port);
}

void UdpListener::listen(SABool stop) {
    checkSockInit();

    char buffer[MAX_MSG_SIZE];

    while (!*stop) {
        auto receivedPacket = tryRecv(buffer);
        if (receivedPacket.nBytes < 0) {
            // already logged it
            continue;
        }

        // TODO: operate on char arrays in Message::from_bytes instead
        std::vector<std::byte> rawData(receivedPacket.nBytes);
        std::memcpy(rawData.data(), buffer, receivedPacket.nBytes);

        try {
            handleMessage(Message::from_bytes(rawData), receivedPacket.senderIp,
                          receivedPacket.senderPort);
        } catch (const std::exception &ex) {
            std::cerr << "Failed to parse message: " << ex.what() << std::endl;
        }
    }
}
std::thread UdpListener::detached_listen(SABool stop) {
    return std::thread(&UdpListener::listen, this, stop);
}

void UdpListener::handleMessage(std::unique_ptr<Message> message,
                                const std::string &senderIp,
                                const uint16_t &senderPort) {
    if (auto *resourceAnnounce =
            dynamic_cast<ResourceAnnounceMessage *>(message.get())) {
        std::cout << *resourceAnnounce << std::endl;
        peerResourceMap->updateResources(senderIp,
                                         resourceAnnounce->resourceNames);
        // Log
        spdlog::info("Updated resources for {}: {}", senderIp,
                     fmt::join(resourceAnnounce->resourceNames, ", "));

    } else if (auto *resourceRequest =
                   dynamic_cast<ResourceRequestMessage *>(message.get())) {
        std::cout << *resourceRequest << std::endl;
        std::vector<std::byte> resourceData =
            localResourceManager->getResource(resourceRequest->resource_name);

        UdpSender sender(senderIp, senderPort);
        sender.sendMessage(ResourceDataMessage(
            resourceRequest->header, resourceRequest->resource_name,
            // TOOD: Replace this with the actual resource data
            std::vector<std::byte>{std::byte(0)}));
    } else if (auto *resourceData =
                   dynamic_cast<ResourceDataMessage *>(message.get())) {
        std::cout << *resourceData << std::endl;
        // TODO: save the resource to disk
        auto downloader =
            Downloader::getRunningDownload(resourceData->resourceName);
        if (!downloader) {
            spdlog::warn("Received not requested resource. Dropping it...: {}",
                         resourceData->resourceName);
            return;
        }
        downloader->stop();
        std::cout << "listener after stop" << std::endl;
    } else {
        std::cout << "Unknown Message Type" << std::endl;
    }
}

void UdpListener::checkSockInit() const {
    if (sockfd < 0) {
        std::cerr << "Socket is not initialized. Call start() first."
                  << std::endl;
        return;
    }
}

void UdpListener::tryCreateSocket() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}

void UdpListener::tryBindSocket() {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        close(sockfd);
        throw std::runtime_error("Failed to bind socket");
    }
}

UdpListener::ReceivedPacket UdpListener::tryRecv(char *buffer) {
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    ssize_t received = recvfrom(sockfd, buffer, MAX_MSG_SIZE, 0,
                                (struct sockaddr *)&clientAddr, &clientAddrLen);

    // Print the sender's address
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);

    if (received >= 0) {
        spdlog::info("Received {} bytes from {}:{}", received, clientIp,
                     ntohs(clientAddr.sin_port));
    } else {
        spdlog::error("Failed to receive data");
    }
    // return received;
    UdpListener::ReceivedPacket receivedPacket = {};
    receivedPacket.nBytes = received;
    receivedPacket.senderIp = std::string(clientIp);
    receivedPacket.senderPort = ntohs(clientAddr.sin_port);
    return receivedPacket;
}

SMap UdpListener::runningDownloads;

void UdpListener::setSockOptions() {
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &UdpListener::TIMEOUT,
                   sizeof(UdpListener::TIMEOUT)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }
}
