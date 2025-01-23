#include "Listener.h"

int main() {
    int port = 8000;

    UdpListener listener(port);

    listener.start();
    listener.listen();

    return 0;
}
