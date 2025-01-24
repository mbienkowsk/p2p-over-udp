#ifndef HEADER_H
#define HEADER_H
#endif

#include <cstddef>
#include <cstdint>
#include <vector>

const uint8_t PROTOCOL_VERSION = 1;

enum class MessageType : uint8_t {
    RESOURCE_ANNOUNCE = 0,
    RESOURCE_REQUEST = 1,
    RESOURCE_DATA = 2
};

/// Represents the header of a message in our protocol
class Header {
  public:
    /// Size of the header in bytes
    const static uint8_t SIZE = 2;

    /// Version of the protocol
    std::uint8_t protocolVersion;

    /// Type of the message (announce/request/data)
    MessageType messageType;

    /// Serializes the header into a vector of bytes
    std::vector<std::byte> serialize() const;

    /// Deserializes the header from a vector of 2 bytes
    static Header deserialize(const std::vector<std::byte> &raw_data);

    static bool validateMessageType(uint8_t mt);

    static bool validateHeaderSize(const std::vector<std::byte> &raw_data);

    bool operator==(const Header &rhs) const = default;

    Header(MessageType MessageType,
           std::uint8_t ProtocolVersion = PROTOCOL_VERSION)
        : protocolVersion(ProtocolVersion), messageType(MessageType) {}
};
