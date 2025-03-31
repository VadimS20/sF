#include "request_receiver.hpp"

#include <array>
#include <bitset>

// static std::string ReceiveResourceNameStart(const Server& server)
// {

//     return received_resource_name_start;
// }

std::optional<RawRequest> ReceiveRequest(const Server& server, ErrorCode& error)
{
    error = ErrorCode::NO_ERRORS;

    RawRequest received_request;

    server.Receive<char>();
    uint16_t resource_name_len = server.Receive<uint16_t>();
    received_request.resource_name = server.ReceiveString(resource_name_len);

    server.Receive<char>();
    uint16_t xml_string_len = server.Receive<uint16_t>();
    received_request.xml_string = server.ReceiveString(xml_string_len);

    return received_request;
}
