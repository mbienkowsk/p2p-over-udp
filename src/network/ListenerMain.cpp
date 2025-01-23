#include "Downloader.h"
#include "Listener.h"
#include <atomic>
#include <iostream>
#include <thread>

int main() {
    int port = 8000;

    auto localResourceManager =
        std::make_shared<LocalResourceManager>("../host_resources");

    auto peerResourceMap = std::make_shared<PeerResourceMap>();

    UdpListener listener(port, localResourceManager, peerResourceMap);

    listener.start();

    UdpSender sender("0.0.0.0", 8000);
    ResourceRequestMessage msg(Header(MessageType::RESOURCE_REQUEST),
                               "resource1");
    {
        auto d1 = Downloader::create(sender, msg);
        auto res = d1->start();
        std::cout << "Downloader started: " << res << std::endl;
    }

    UdpSender sender2("0.0.0.0", 8000);
    ResourceRequestMessage msg2(Header(MessageType::RESOURCE_REQUEST),
                                "resource1");
    auto d2 = Downloader::create(sender2, msg2);
    auto res2 = d2->start();

    listener.listen();

    return 0;
}
