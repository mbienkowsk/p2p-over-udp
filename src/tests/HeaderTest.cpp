#include "../network/Header.h"
#include "gtest/gtest.h"

TEST(PacketHeader, SerializationReversesDeserialization) {
  Header header(MessageType::RESOURCE_REQUEST);

  std::vector<std::byte> serialized = header.serialize();
  Header deserialized = Header::deserialize(serialized);

  EXPECT_EQ(header, deserialized);
}

TEST(HeaderTest, InvalidDeserializationThrows) {
  auto invalid_msg_type = std::vector({std::byte(1), std::byte(3)});
  EXPECT_THROW(Header::deserialize(invalid_msg_type), std::runtime_error);

  auto data_too_short = std::vector({std::byte(1)});
  EXPECT_THROW(Header::deserialize(data_too_short), std::runtime_error);

  auto data_too_long = std::vector({std::byte(1), std::byte(2), std::byte(3)});
  EXPECT_THROW(Header::deserialize(data_too_long), std::runtime_error);
}
