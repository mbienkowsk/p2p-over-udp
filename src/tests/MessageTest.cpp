#ifndef SERIALIZATION_UTIL_H
#define SERIALIZATION_UTIL_H

#include "../network/Message.h"
#include "../serialization/Utils.h"
#include "gtest/gtest.h"

TEST(MessageSerialization, AnnounceMessageRoundTrip) {
    ResourceAnnounceMessage msg({"resource1", "resource2"});
    auto serialized = msg.serialize();
    auto deserialized = ResourceAnnounceMessage::fromHeaderAndPayload(
        Header::deserialize({serialized.begin(), serialized.begin() + 2}),
        {serialized.begin() + 2, serialized.end()});

    EXPECT_EQ(msg, deserialized);
}

TEST(MessageSerialization, RequestMessageRoundTrip) {
    ResourceRequestMessage msg(Header(MessageType::RESOURCE_REQUEST),
                               "resource1");
    auto serialized = msg.serialize();
    auto deserialized = ResourceRequestMessage::fromHeaderAndPayload(
        Header::deserialize({serialized.begin(), serialized.begin() + 2}),
        {serialized.begin() + 2, serialized.end()});

    EXPECT_EQ(msg.header, deserialized.header);
    EXPECT_EQ(msg.resource_name, deserialized.resource_name);
}

TEST(MessageSerialization, DataMessageRoundTrip) {
    ResourceDataMessage msg(Header(MessageType::RESOURCE_DATA), "resource1",
                            {std::byte(0x01), std::byte(0x02)});
    auto serialized = msg.serialize();
    auto deserialized = ResourceDataMessage::fromHeaderAndPayload(
        Header::deserialize({serialized.begin(), serialized.begin() + 2}),
        {serialized.begin() + 2, serialized.end()});

    EXPECT_EQ(msg, deserialized);
}

#endif
