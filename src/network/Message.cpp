#include "Message.h"
#include <cstring>
#include <stdexcept>

/// Serializes the message into a vector of bytes
std::vector<std::byte> Message::serialize() const {
  // Serialize the header
  std::vector<std::byte> serialized = header.serialize();

  // Append the data payload
  serialized.reserve(serialized.size() + data.size());
  for (const auto &byte : data) {
    serialized.push_back(byte);
  }

  return serialized;
}

/// Deserializes a message from a vector of bytes
Message Message::deserialize(const std::vector<std::byte> &raw_data) {
  if (raw_data.size() < 2) { // Minimum size for a valid header
    throw std::runtime_error("Invalid message: too short");
  }

  // Deserialize the header (first 2 bytes)
  Header header = Header::deserialize({raw_data.begin(), raw_data.begin() + 2});

  // Extract the data payload
  std::vector<std::byte> payload;
  if (raw_data.size() > 2) {
    payload.resize(raw_data.size() - 2);
    std::memcpy(payload.data(), &raw_data[2], payload.size());
  }

  return Message(header, payload);
}

/// Comparison operator for testing equality
bool Message::operator==(const Message &rhs) const {
  return header == rhs.header && data == rhs.data;
}

Message::Message(const Header &header, std::vector<std::byte> data)
    : header(header), data(std::move(data)) {}
