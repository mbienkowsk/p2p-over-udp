#include <cstddef>
#include <cstdint>
#include <vector>

class Header {
public:
  enum class MsgType : uint8_t {
    RESOURCE_ANNOUCE = 0,
    RESOURCE_REQUEST = 1,
    RESOURCE_DATA = 2
  };

  /// Version of the protocol
  std::byte ProtocolVersion;

  /// Type of the message (announce/request/data)
  MsgType MessageType;
  // uint32_t DataSize; TODO: do we need this?

  /// Serializes the header into a vector of bytes
  std::vector<std::byte> serialize() const;

  /// Deserializes the header from a vector of 2 bytes
  static Header deserialize(const std::vector<std::byte> &raw_data);

  bool operator==(const Header &rhs) const;
};

class Message {
public:
  Header header;
  std::vector<uint8_t> data;
};
