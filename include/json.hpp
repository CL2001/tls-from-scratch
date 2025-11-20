#pragma once
#include <string>


class Json {
private:
    std::string_view json;
    bool isValid();
public:
    Json(std::string_view);
    std::string_view operator[](std::string_view);
    friend std::ostream& operator<<(std::ostream& os, const Json& j)
    {
        os << j.json;
        return os;
    }
};
