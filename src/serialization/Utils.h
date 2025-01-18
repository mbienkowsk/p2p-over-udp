#include <string>
#include <vector>

/// Converts a string to a vector of bytes for serialization
std::vector<std::byte> serializeString(const std::string &str);

/// Deserializes a null-terminated string starting from a given
/// offset and advances it by the length of the string and past the
/// following separator.
std::string deserializeString(const std::vector<std::byte> &data,
                              size_t &offset);
