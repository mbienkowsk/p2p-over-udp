#include "Header.h"
#include <stdexcept>

std::vector<std::byte> Header::serialize() const {
  std::vector<std::byte> raw_data;
  raw_data.push_back(ProtocolVersion);
  raw_data.push_back(static_cast<std::byte>(MessageType));
  return raw_data;
};

Header Header::deserialize(const std::vector<std::byte> &raw_data) {
  if (raw_data.size() != 2) {
    throw std::runtime_error("Invalid header length");
  }

  Header header;
  header.ProtocolVersion = raw_data[0];
  header.MessageType = static_cast<Header::MsgType>(raw_data[1]);
  return header;
};
