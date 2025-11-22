#pragma once
#include <string>


class Json {
private:
    std::string json;
    bool isValid();
public:
    Json(std::string);
    std::string operator[](std::string);
    friend std::ostream& operator<<(std::ostream& os, const Json& j)
    {
        os << j.json;
        return os;
    }
};
