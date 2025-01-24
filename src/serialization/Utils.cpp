#include "Utils.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::byte> serializeString(const std::string &str) {
    std::vector<std::byte> bytes(str.size());
    std::transform(str.begin(), str.end(), bytes.begin(),
                   [](char c) { return std::byte(c); });
    return bytes;
}

std::string deserializeString(const std::vector<std::byte> &data,
                              size_t &offset) {
    size_t start = offset;
    while (offset < data.size() && data[offset] != std::byte(0)) {
        ++offset;
    }

    std::string result(reinterpret_cast<const char *>(&data[start]),
                       offset - start);
    ++offset; // Skip the null terminator
    return result;
}
