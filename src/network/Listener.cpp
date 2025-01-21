#include "Listener.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

UdpListener::UdpListener(int port) : port(port), sockfd(-1) {}

UdpListener::~UdpListener() {
  if (sockfd >= 0) {
    close(sockfd);
  }
}

bool UdpListener::start() {
  // Create a UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("Failed to create socket");
    return false;
  }

  // Set up the address structure
  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);

  // Bind the socket to the specified port
  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("Failed to bind socket");
    close(sockfd);
    sockfd = -1;
    return false;
  }

  std::cout << "Listening on port " << port << " for incoming UDP traffic..."
            << std::endl;
  return true;
}

void UdpListener::listen() {
  if (sockfd < 0) {
    std::cerr << "Socket is not initialized. Call start() first." << std::endl;
    return;
  }

  char buffer[1024];
  sockaddr_in clientAddr{};
  socklen_t clientAddrLen = sizeof(clientAddr);

  while (true) {
    // Receive data from a client
    ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (received < 0) {
      perror("Failed to receive data");
      continue;
    }

    buffer[received] = '\0'; // Null-terminate the received message

    // Print the sender's address and message
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
    std::cout << "Received message from " << clientIp << ":"
              << ntohs(clientAddr.sin_port) << " - " << buffer << std::endl;
  }
}
