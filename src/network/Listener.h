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
  static const int MAX_MSG_SIZE = 65505;
  int port;
  int sockfd;
  void handleMessage(std::unique_ptr<Message> message);
  void checkSockInit() const;
  void tryCreateSocket();
  void tryBindSocket();
  ssize_t tryRecv(char *buffer);
};

#endif
