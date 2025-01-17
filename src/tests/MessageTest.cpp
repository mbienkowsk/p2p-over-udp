#include "../network/Message.h"
#include "gtest/gtest.h"

// Test serialization of a valid message
TEST(MessageTest, SerializeValidMessage) {
  auto header = Header(Header::MsgType::RESOURCE_DATA);

  std::vector<std::byte> payload = {
      std::byte{0x48}, std::byte{0x65}, std::byte{0x6C}, std::byte{0x6C},
      std::byte{0x6F} // "Hello"
  };

  Message message(header, payload);
  std::vector<std::byte> serialized = message.serialize();

  // Check the size of the serialized data
  EXPECT_EQ(serialized.size(), 2 + payload.size());

  // Verify header serialization
  EXPECT_EQ(serialized[0], std::byte{1}); // Protocol version
  EXPECT_EQ(serialized[1],
            std::byte{static_cast<uint8_t>(
                Header::MsgType::RESOURCE_DATA)}); // Message type

  // Verify payload serialization
  for (size_t i = 0; i < payload.size(); ++i) {
    EXPECT_EQ(serialized[2 + i], payload[i]);
  }
}

// Test deserialization of a valid message
TEST(MessageTest, DeserializeValidMessage) {
  // Simulated serialized data
  std::vector<std::byte> raw_data = {
      std::byte{1}, std::byte{2}, // Header: ProtocolVersion=1,
                                  // MessageType=RESOURCE_DATA
      std::byte{0x48}, std::byte{0x65}, std::byte{0x6C}, std::byte{0x6C},
      std::byte{0x6F} // Payload: "Hello"
  };

  // Deserialize the message
  Message message = Message::deserialize(raw_data);

  // Verify header fields
  EXPECT_EQ(message.header.ProtocolVersion, 1);
  EXPECT_EQ(message.header.MessageType, Header::MsgType::RESOURCE_DATA);

  // Expected payload

  // Verify the payload matches the expected payload
  EXPECT_EQ(message.dataAsString(), std::string("Hello"));
}

// Test round-trip serialization/deserialization
TEST(MessageTest, SerializeDeserializeRoundTrip) {
  auto header = Header(Header::MsgType::RESOURCE_REQUEST);

  std::vector<std::byte> payload = {
      std::byte{0x42}, std::byte{0x43}, std::byte{0x44} // Arbitrary payload
  };

  Message original_message(header, payload);
  std::vector<std::byte> serialized = original_message.serialize();
  Message deserialized_message = Message::deserialize(serialized);

  // Verify the round-trip result
  EXPECT_EQ(original_message, deserialized_message);
}

// Test deserialization with invalid header size
TEST(MessageTest, DeserializeInvalidHeaderSize) {
  // Header too short
  std::vector<std::byte> raw_data_too_short = {std::byte{1}};
  EXPECT_THROW(Message::deserialize(raw_data_too_short), std::runtime_error);

  // Valid header size but invalid payload
  std::vector<std::byte> raw_data_too_long = {std::byte{1}, std::byte{2},
                                              std::byte{3}};
  EXPECT_NO_THROW(Message::deserialize(
      raw_data_too_long)); // Assumes extra bytes are ignored
}

// Test deserialization with invalid message type
TEST(MessageTest, DeserializeInvalidMessageType) {
  std::vector<std::byte> raw_data_invalid_type = {
      std::byte{1}, std::byte{99} // Invalid MsgType
  };

  EXPECT_THROW(Message::deserialize(raw_data_invalid_type), std::runtime_error);
}
