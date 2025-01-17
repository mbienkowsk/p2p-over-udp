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

  /// Serializes the message into a vector of bytes
  virtual std::vector<std::byte> serialize() const = 0;

  /// Deserializes a message from a vector of bytes
  Message *from_bytes(const std::vector<std::byte> &raw_data);

  Message(Header header) : header(header) {};
};

/// Message broadcasted to announce available resources
class ResourceAnnounceMessage : Message {
public:
  /// Names of available resources
  std::vector<std::string> resourceNames;

  ResourceAnnounceMessage(Header header, std::vector<std::string> resourceNames)
      : Message(header), resourceNames(resourceNames) {};

  /// Serializes the message into a vector of bytes in the format:
  /// header, resource names with null separators between the resource names
  std::vector<std::byte> serialize() const override;
};

/// Message requesting a specific resource
class ResourceRequestMessage : Message {
public:
  /// Name of the requested resource
  std::string resource_name;

  ResourceRequestMessage(Header header, const std::string &resource_name)
      : Message(header), resource_name(resource_name) {};

  /// Serializes the message into a vector of bytes in the format:
  /// header, resource name as a contiguous sequence of bytes
  std::vector<std::byte> serialize() const override;
};

/// Message containing a requested resource
class ResourceDataMessage : Message {
public:
  /// Name of the resource
  std::string resourceName;

  /// Data of the resource
  std::vector<std::byte> resourceData;

  ResourceDataMessage(Header header, const std::string &resource_name,
                      const std::vector<std::byte> &resource_data)
      : Message(header), resourceName(resource_name),
        resourceData(resource_data) {};

  /// Serializes the message into a vector of bytes in the format:
  /// header, resource name, resource data with a null separator
  /// between the name and the data
  std::vector<std::byte> serialize() const override;
};

#endif
