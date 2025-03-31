#ifndef FORTRESS_SERVER_IMPL_HPP_
#define FORTRESS_SERVER_IMPL_HPP_

#include "server.hpp"

#include <arpa/inet.h>
#include <iostream>

template <typename T>
T Server::Receive() const
{
    static_assert(std::is_pod_v<T>, "Requires a POD type");

    T received_value{};
    size_t data_size = sizeof(T);

    if (ReceiveBytes(reinterpret_cast<char*>(&received_value), data_size) != 0) {
        return T{}; // Return default value if the receive fails
    }

    if constexpr (std::is_same_v<T, uint16_t>) {
        return ntohs(received_value); // Network to host byte order for uint16_t
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        return ntohl(received_value); // Network to host byte order for uint32_t
    } else {
        return received_value; // No conversion needed for other types
    }
}

template <typename T>
void Server::Send(T data) const
{
    static_assert(std::is_pod_v<T>, "Requires a POD type");

    size_t data_size = sizeof(T);

    if constexpr (std::is_same_v<T, uint16_t>) {
        uint16_t converted_uint16 = htons(data);
        send(client_socket_, &converted_uint16, data_size, 0);
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        uint32_t converted_uint32 = htonl(data);
        send(client_socket_, &converted_uint32, data_size, 0);
    } else {
        send(client_socket_, &data, data_size, 0);
    }
}

#endif // FORTRESS_SERVER_IMPL_HPP_
