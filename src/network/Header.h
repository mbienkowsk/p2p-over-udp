#ifndef HEADER_H
#define HEADER_H
#endif

#include <cstddef>
#include <cstdint>
#include <vector>

/// Represents the header of a message in our protocol
class Header {
public:
  enum class MsgType : uint8_t {
    RESOURCE_ANNOUCE = 0,
    RESOURCE_REQUEST = 1,
    RESOURCE_DATA = 2
  };

  /// Version of the protocol
  std::uint8_t ProtocolVersion;

  /// Type of the message (announce/request/data)
  MsgType MessageType;
  // uint32_t DataSize; TODO: do we need this?

  /// Serializes the header into a vector of bytes
  std::vector<std::byte> serialize() const;

  /// Deserializes the header from a vector of 2 bytes
  static Header deserialize(const std::vector<std::byte> &raw_data);

  static bool validateMessageType(uint8_t mt);

  static bool validateHeaderSize(const std::vector<std::byte> &raw_data);

  bool operator==(const Header &rhs) const;

  Header(MsgType MessageType, std::uint8_t ProtocolVersion = 1)
      : ProtocolVersion(ProtocolVersion), MessageType(MessageType) {}
};
