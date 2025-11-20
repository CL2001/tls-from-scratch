#include "json.hpp"


Json::Json(std::string_view json) : json(json)
{
    if (!isValid())
        json = {};
};


bool Json::isValid()
{
    size_t brace = 0;
    size_t bracket = 0;

    for (char c : json)
    {
        switch (c)
        {
        case '{': brace++; break;
        case '}': if (brace == 0) return false; brace--; break;
        case '[': bracket++; break;
        case ']': if (bracket == 0) return false; bracket--; break;
        default: break;
        }
    }

    return (brace == 0 && bracket == 0);
}


std::string_view Json::operator[](std::string_view key)
{
    // ---- Find the key ----
    std::string quotedKey;
    quotedKey = "\"";
    quotedKey += key;
    quotedKey += "\"";

    size_t pos = json.find(quotedKey);
    if (pos == std::string::npos)
        return {};

    pos += quotedKey.size();

    // ---- Skip whitespace ----
    while (pos < json.size() && isspace(json[pos]))
        pos++;

    // ---- Expect ':' ----
    if (pos >= json.size() || json[pos] != ':')
        return {};
    pos++;

    // ---- Skip whitespace ----
    while (pos < json.size() && isspace(json[pos]))
        pos++;

    // ---- Determine value type ----
    char c = json[pos];

    if (c == '"')
    {
        size_t start = pos + 1;
        size_t end = json.find('"', start);
        if (end == std::string::npos)
            return {};
        return json.substr(start, end - start);
    }

    if (c == '{' || c == '[')
    {
        char open = c;
        char close = (c == '{') ? '}' : ']';
        size_t depth = 0;
        size_t i = pos;

        for (; i < json.size(); i++)
        {
            if (json[i] == open)
                depth++;
            else if (json[i] == close)
            {
                depth--;
                if (depth == 0)
                    break;
            }
        }

        if (i >= json.size())
            return {};

        return json.substr(pos, i - pos + 1);
    }

    size_t start = pos;
    while (pos < json.size() &&
           json[pos] != ',' &&
           json[pos] != '}' &&
           json[pos] != ']' &&
           !isspace(json[pos]))
        pos++;

    return json.substr(start, pos - start);
}
