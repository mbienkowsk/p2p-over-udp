#include "Message.h"
#include "UdpSender.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
  try {
    UdpSender sender("127.0.0.1", 8000);

    // Create and send ResourceAnnounceMessage
    Header announceHeader{MessageType::RESOURCE_ANNOUCE, 1};
    std::vector<std::string> resources = {"Resource1", "Resource2",
                                          "Resource3"};
    ResourceAnnounceMessage announceMessage(announceHeader, resources);
    sender.sendMessage(announceMessage);
    std::cout << "Sent: " << announceMessage << std::endl;

    // Create and send ResourceRequestMessage
    ResourceRequestMessage requestMessage("Resource1");
    sender.sendMessage(requestMessage);
    std::cout << "Sent: " << requestMessage << std::endl;

    // Create and send ResourceDataMessage
    std::vector<std::byte> data = {std::byte{0x60}, std::byte{0x65},
                                   std::byte{0x67}, std::byte{0x69}};
    ResourceDataMessage dataMessage("Resource2", data);
    sender.sendMessage(dataMessage);
    std::cout << "Sent: " << dataMessage << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
