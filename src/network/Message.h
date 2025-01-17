#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"
#include <cstddef>
#include <string>
#include <vector>

/// Represents a message that can be sent over the network
class Message {
public:
  /// The header of the message
  Header header;

  /// The payload data
  std::vector<std::byte> data;

  /// Serializes the message into a vector of bytes
  std::vector<std::byte> serialize() const;

  /// Deserializes a message from a vector of bytes
  static Message deserialize(const std::vector<std::byte> &raw_data);

  /// Comparison operator for testing equality
  bool operator==(const Message &rhs) const;

  Message(const Header &header, std::vector<std::byte> data);

  const std::string dataAsString() const;
};

#endif
