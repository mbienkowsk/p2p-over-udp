#include "Listener.h"

int main() {
  int port = 8000; // Hardcoded port number

  UdpListener listener(port);
  if (!listener.start()) {
    return 1;
  }

  listener.listen();

  return 0;
}
