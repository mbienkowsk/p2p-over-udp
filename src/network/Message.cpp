#include "Message.h"
#include "../serialization/Utils.h"
#include <cstring>
#include <memory>
#include <stdexcept>

/// Deserializes a message from a vector of bytes
std::unique_ptr<Message>
Message::from_bytes(const std::vector<std::byte> &raw_data) {
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
    return std::make_unique<ResourceAnnounceMessage>(
        ResourceAnnounceMessage::fromHeaderAndPayload(header, payload));

  case MessageType::RESOURCE_REQUEST:
    return std::make_unique<ResourceRequestMessage>(ResourceRequestMessage(
        ResourceRequestMessage::fromHeaderAndPayload(header, payload)));

  case MessageType::RESOURCE_DATA:
    return std::make_unique<ResourceDataMessage>(ResourceDataMessage(
        ResourceDataMessage::fromHeaderAndPayload(header, payload)));
    return std::make_unique<ResourceDataMessage>(ResourceDataMessage(
        ResourceDataMessage::fromHeaderAndPayload(header, payload)));
  default:
    throw std::runtime_error("Invalid message type");
  }
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
