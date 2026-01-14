#include "external/json.hpp"
#include <string>

using namespace std;

// /*
// { "type": "IDENTIFY",
//   "username": "Kimberly" }
// */

// namespace UserDTO {

//     struct IDENTIFY {
//       public:
//         string type;
//         string username;

//         NLOHMANN_DEFINE_TYPE_INTRUSIVE(IDENTIFY, type, username)
//     };

//     struct Response {
//         std::string type = "RESPONSE";
//         std::string operation;
//         std::string result;
//         std::string extra;

//         NLOHMANN_DEFINE_TYPE_INTRUSIVE(Response, type, operation, result, extra)
//     };

//     struct NewUser {
//         std::string type = "NEW_USER";
//         std::string username;

//         NLOHMANN_DEFINE_TYPE_INTRUSIVE(NewUser, type, username)
//     };
// }

namespace dto {

struct User {
    std::string username;
    std::string status;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, status)
};

}
