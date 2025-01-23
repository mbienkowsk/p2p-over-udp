#include "UdpSender.h"

UdpSender::UdpSender(const std::string &ip, const uint16_t &port) {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  std::memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &dest_addr.sin_addr.s_addr) <= 0) {
    close(sockfd);
    throw std::invalid_argument("Invalid IP address");
  }
}

UdpSender::~UdpSender() { close(sockfd); }

void UdpSender::sendMessage(const Message &message) const {
  std::vector<std::byte> serializedMessage = message.serialize();

  ssize_t sentBytes =
      sendto(sockfd, serializedMessage.data(), serializedMessage.size(), 0,
             (struct sockaddr *)&dest_addr, sizeof(dest_addr));

  if (sentBytes < 0) {
    throw std::runtime_error("Failed to send message");
  }

  if (static_cast<size_t>(sentBytes) != serializedMessage.size()) {
    throw std::runtime_error("Partial message sent");
  }
}
