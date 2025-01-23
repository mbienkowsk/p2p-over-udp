#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include "Message.h"
#include <memory>
#include <netinet/in.h>
#include <unistd.h>

class UdpListener {
public:
  explicit UdpListener(int port);
  ~UdpListener();

  void start();
  void listen();

private:
  struct ReceivedPacket {
    ssize_t nBytes;
    std::string senderIp;
    uint16_t senderPort;
  };

  static const int MAX_MSG_SIZE = 65505;
  int port;
  int sockfd;
  void handleMessage(std::unique_ptr<Message> message,
                     const std::string &senderIp, const uint16_t &senderPort);
  void checkSockInit() const;
  void tryCreateSocket();
  void tryBindSocket();
  UdpListener::ReceivedPacket tryRecv(char *buffer);
};

#endif
