#include "Header.h"
#include <cstddef>
#include <stdexcept>

std::vector<std::byte> Header::serialize() const {
    std::vector<std::byte> raw_data;
    raw_data.push_back(std::byte(protocolVersion));
    raw_data.push_back(static_cast<std::byte>(messageType));
    return raw_data;
};

Header Header::deserialize(const std::vector<std::byte> &raw_data) {
    // Validate header size
    if (!Header::validateHeaderSize(raw_data)) {
        throw std::runtime_error("Invalid header length");
    }

    const auto protocolVersion = std::to_integer<uint8_t>(raw_data[0]);
    // Extract the message type as an integer
    uint8_t messageType = std::to_integer<uint8_t>(raw_data[1]);

    if (!Header::validateMessageType(messageType)) {
        throw std::runtime_error("Invalid message type");
    }

    return Header(static_cast<MessageType>(messageType), protocolVersion);
}

/// Validates the message type
bool Header::validateMessageType(uint8_t mt) { return mt <= 2; }

/// Validates the size of the header before deserialization
bool Header::validateHeaderSize(const std::vector<std::byte> &raw_data) {
    return raw_data.size() == 2;
}
