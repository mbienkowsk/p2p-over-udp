#include "Network.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

BroadcastSender::BroadcastSender(int port, const std::string &broadcast_addr_str)
    : port_(port), broadcast_addr_str_(broadcast_addr_str)
{
    int sock;
    struct sockaddr_in broadcast_addr;
    setup_broadcast_socket(sock);

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(port);
    broadcast_addr.sin_addr.s_addr = inet_addr(broadcast_addr_str.c_str());
    this->broadcast_addr_ = std::make_unique<struct sockaddr_in>(broadcast_addr);
    this->sock_ = sock;
};

std::thread BroadcastSender::make_worker(std::function<ResourceAnnounceMessage()> msg_gen)
{
    return std::thread([this, msg_gen]()
                       {
            while (true)
            {
                ResourceAnnounceMessage msg = msg_gen();
                broadcast(sock_, *broadcast_addr_, msg);
                std::this_thread::sleep_for(std::chrono::seconds(3));
            } });
}

void setup_broadcast_socket(int &sock)
{
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("failed to create socket");
        exit(1);
    }

    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        perror("failed at boradcast setsockopt");
        exit(1);
    }
}

void broadcast(int sock, struct sockaddr_in &broadcast_addr, const ResourceAnnounceMessage &msg)
{
    spdlog::info(
        "Broadcasting message of type RESOURCE_ANNOUCE with {} resources",
        msg.resourceNames.size());

    auto bytes = msg.serialize();
    if (sendto(sock, bytes.data(), bytes.size(), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0)
    {
        perror("failed to send broadcastDD");
        exit(1);
    }
}
