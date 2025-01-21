#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include <netinet/in.h>

class UdpListener {
public:
  explicit UdpListener(int port);
  ~UdpListener();

  bool start();
  void listen();

private:
  int port;
  int sockfd;
};

#endif
