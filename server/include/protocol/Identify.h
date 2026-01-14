#include "external/json.hpp"

namespace protocol {

struct Identify {
    std::string type = "IDENTIFY";
    std::string username;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Identify, type, username)
};

struct Response {
    std::string type = "RESPONSE";
    std::string operation;
    std::string result;
    std::string extra;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Response, type, operation, result, extra)
};

struct NewUser {
    std::string type = "NEW_USER";
    std::string username;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NewUser, type, username)
};

}
