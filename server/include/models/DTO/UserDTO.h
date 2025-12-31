// dto/: Contendrá archivos como UserDTO.h, RoomDTO.h y ChatDTO.h. Aquí defines las structs que mapean exactamente los JSONs del protocolo usando NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE.
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

/*
{ "type": "IDENTIFY",
  "username": "Kimberly" }
*/

namespace UserDTO {

    struct IDENTIFY {
      public:
        string type = "NEW_USER";
        string username;
  
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(IDENTIFY, type, username)
    };

    struct IDENTIFYResponse {
        std::string type = "RESPONSE";
        std::string operation;
        std::string result;
        std::string extra;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IDENTIFYResponse, type, operation, result, extra)
}