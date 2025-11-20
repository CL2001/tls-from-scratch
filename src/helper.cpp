#include "helper.hpp"

std::string_view extractValue(std::string_view input, std::string_view key)
{
    // Find the key
    size_t pos = input.find(key);
    if (pos == std::string::npos)
        return {};

    pos += key.size();

    // Expect ": {"
    if (pos + 3 > input.size() || input[pos] != ':' || input[pos+1] != ' ' || input[pos+2] != '{')
        return {};

    // Move to the character after '{'
    pos += 3;

    // Find the closing '}'
    size_t end = input.find('}', pos);
    if (end == std::string::npos)
        return {};

    // Return view into original text
    return input.substr(pos, end - pos);
}