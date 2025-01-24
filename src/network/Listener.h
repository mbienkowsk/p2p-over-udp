#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include "../resources/LocalResourceManager.h"
#include "../resources/PeerResourceMap.h"
#include "Message.h"
#include "ThreadSafeHashMap.h"
#include <memory>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

using SMap = std::shared_ptr<ThreadSafeHashMap>;

class UdpListener {
  public:
    UdpListener(int port,
                std::shared_ptr<LocalResourceManager> localResourceManager,
                std::shared_ptr<PeerResourceMap> peerResourceMap,
                const std::string &myAddress);

    ~UdpListener();

    void start();
    void listen(SABool stop);
    void setSockOptions();
    std::thread detached_listen(SABool stop);

    static SMap runningDownloads;

  private:
    constexpr static const timeval TIMEOUT = {1, 0};

    struct ReceivedPacket {
        ssize_t nBytes;
        std::string senderIp;
        uint16_t senderPort;
    };

    static const int MAX_MSG_SIZE = 65507;

    std::shared_ptr<PeerResourceMap> peerResourceMap;
    std::shared_ptr<LocalResourceManager> localResourceManager;

    int port;
    int sockfd;
    std::string myAddress;

    void handleMessage(std::unique_ptr<Message> message,
                       const std::string &senderIp, const uint16_t &senderPort);
    void checkSockInit() const;
    void tryCreateSocket();
    void tryBindSocket();
    UdpListener::ReceivedPacket tryRecv(char *buffer);
};

#endif
