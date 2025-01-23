#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include "Message.h"
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

class UdpSender {
private:
  int sockfd;
  struct sockaddr_in dest_addr;

public:
  UdpSender(const std::string &ip, const uint16_t &port);

  ~UdpSender();

  void sendMessage(const Message &message) const;
};

#endif
