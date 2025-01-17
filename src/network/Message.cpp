#include "Message.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>

#define UNIMPLEMENTED                                                          \
  throw std::logic_error(std::string(__FUNCTION__) + " is not implemented "    \
                                                     "yet.");

/// Deserializes a message from a vector of bytes
Message *Message::from_bytes(const std::vector<std::byte> &raw_data) {
  if (raw_data.size() < Header::SIZE) { // Minimum size for a valid header
    throw std::runtime_error("Invalid message: too short");
  }

  // Deserialize the header (first 2 bytes)
  Header header =
      Header::deserialize({raw_data.begin(), raw_data.begin() + Header::SIZE});

  // Extract the data payload
  std::vector<std::byte> payload;
  payload.resize(raw_data.size() - Header::SIZE);
  std::memcpy(payload.data(), &raw_data[Header::SIZE], payload.size());

  switch (header.messageType) {
  case MessageType::RESOURCE_ANNOUCE:
    return new ResourceAnnounceMessage(
        ResourceAnnounceMessage::fromHeaderAndPayload(header, payload));
  case MessageType::RESOURCE_REQUEST:
    return new ResourceRequestMessage(
        ResourceRequestMessage::fromHeaderAndPayload(header, payload));
  case MessageType::RESOURCE_DATA:
    return new ResourceDataMessage(
        ResourceDataMessage::fromHeaderAndPayload(header, payload));
  }
}

/// Converts a string to a vector of bytes for serialization
std::vector<std::byte> serializeString(const std::string &str) {
  std::vector<std::byte> bytes(str.size());
  std::transform(str.begin(), str.end(), bytes.begin(),
                 [](char c) { return static_cast<std::byte>(c); });
  return bytes;
}

/// Deserializes a null-terminated string starting from a given
/// offset and advances it by the length of the string and past the
/// following separator.
std::string deserializeString(const std::vector<std::byte> &data,
                              size_t &offset) {
  size_t start = offset;
  while (offset < data.size() && data[offset] != std::byte(0)) {
    ++offset;
  }

  if (offset >= data.size()) {
    throw std::runtime_error(
        "Invalid data: string not properly null-terminated.");
  }

  std::string result(reinterpret_cast<const char *>(&data[start]),
                     offset - start);
  ++offset; // Skip the null terminator
  return result;
}

std::vector<std::byte> ResourceRequestMessage::serialize() const {
  auto rv = header.serialize();
  auto rNameAsBytes = serializeString(resource_name);
  rv.insert(rv.end(), rNameAsBytes.begin(), rNameAsBytes.end());
  return rv;
}

std::vector<std::byte> ResourceDataMessage::serialize() const {
  auto rv = header.serialize();
  auto rNameAsBytes = serializeString(resourceName);

  rv.insert(rv.end(), rNameAsBytes.begin(), rNameAsBytes.end());
  rv.push_back(std::byte(0));
  rv.insert(rv.end(), resourceData.begin(), resourceData.end());

  return rv;
}

std::vector<std::byte> ResourceAnnounceMessage::serialize() const {
  auto rv = header.serialize();

  for (const auto &rName : resourceNames) {
    auto rNameAsBytes = serializeString(rName);
    rv.insert(rv.end(), rNameAsBytes.begin(), rNameAsBytes.end());
    rv.push_back(std::byte(0));
  }
  return rv;
}

ResourceAnnounceMessage ResourceAnnounceMessage::fromHeaderAndPayload(
    Header header, const std::vector<std::byte> &raw_data) {
  size_t offset = 0;

  // Deserialize resource names
  std::vector<std::string> resource_names;
  while (offset < raw_data.size()) {
    std::string resource_name = deserializeString(raw_data, offset);
    resource_names.push_back(resource_name);
  }

  return ResourceAnnounceMessage(header, resource_names);
}

ResourceDataMessage ResourceDataMessage::fromHeaderAndPayload(
    Header header, const std::vector<std::byte> &raw_data) {
  size_t offset = 0;

  // Deserialize the resource name
  std::string resource_name = deserializeString(raw_data, offset);

  // Extract the resource data
  std::vector<std::byte> resource_data(raw_data.begin() + offset,
                                       raw_data.end());

  return ResourceDataMessage(header, resource_name, resource_data);
}

ResourceRequestMessage ResourceRequestMessage::fromHeaderAndPayload(
    Header header, const std::vector<std::byte> &raw_data) {
  size_t offset = 0;

  // Deserialize the resource name
  std::string resource_name = deserializeString(raw_data, offset);

  return ResourceRequestMessage(header, resource_name);
}
