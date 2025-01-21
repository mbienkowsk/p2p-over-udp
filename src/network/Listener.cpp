#include "Listener.h"
#include "Message.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <unistd.h>
#include <vector>

UdpListener::UdpListener(int port) : port(port), sockfd(-1) {}

UdpListener::~UdpListener() {
  if (sockfd >= 0) {
    close(sockfd);
  }
}

void UdpListener::start() {
  tryCreateSocket();
  tryBindSocket();
  spdlog::info("Listening on port {} for incoming UDP traffic...\n", port);
}

void UdpListener::listen() {
  checkSockInit();

  char buffer[MAX_MSG_SIZE];

  while (true) {
    auto received = tryRecv(buffer);
    if (received < 0) {
      // already logged it
      continue;
    }

    // TODO: operate on char arrays in Message::from_bytes instead
    std::vector<std::byte> rawData(received);
    std::memcpy(rawData.data(), buffer, received);

    try {
      handleMessage(Message::from_bytes(rawData));
    } catch (const std::exception &ex) {
      std::cerr << "Failed to parse message: " << ex.what() << std::endl;
    }
  }
}
void UdpListener::handleMessage(std::unique_ptr<Message> message) {
  if (auto *resourceAnnounce =
          dynamic_cast<ResourceAnnounceMessage *>(message.get())) {
    std::cout << *resourceAnnounce << std::endl;
  } else if (auto *resourceRequest =
                 dynamic_cast<ResourceRequestMessage *>(message.get())) {
    std::cout << *resourceRequest << std::endl;
  } else if (auto *resourceData =
                 dynamic_cast<ResourceDataMessage *>(message.get())) {
    std::cout << *resourceData << std::endl;
  } else {
    std::cout << "Unknown Message Type" << std::endl;
  }
}

void UdpListener::checkSockInit() const {
  if (sockfd < 0) {
    std::cerr << "Socket is not initialized. Call start() first." << std::endl;
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

ssize_t UdpListener::tryRecv(char *buffer) {
  sockaddr_in clientAddr{};
  socklen_t clientAddrLen = sizeof(clientAddr);

  ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                              (struct sockaddr *)&clientAddr, &clientAddrLen);

  // Print the sender's address
  char clientIp[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);

  if (received >= 0) {
    spdlog::info("Message received from {}:{}", clientIp,
                 ntohs(clientAddr.sin_port));
  } else {
    spdlog::error("Failed to receive data");
  }
  return received;
}
