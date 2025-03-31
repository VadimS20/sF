#ifndef FORTRESS_REQUEST_RECEIVER_HPP_
#define FORTRESS_REQUEST_RECEIVER_HPP_

#include <optional>
#include <string>
#include "server.hpp"

enum class ErrorCode {
    NO_ERRORS,
    WRONG_START_OF_RESOURCE_NAME,
};

struct RawRequest {
    std::string resource_name;
    std::string xml_string;
};

std::optional<RawRequest> ReceiveRequest(const Server& server, ErrorCode& error);


#endif // FORTRESS_REQUEST_RECEIVER_HPP_
