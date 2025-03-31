#include "server.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <array>
#include <system_error>
#include <vector>

Server::Server(uint16_t port, std::string& error_message)
{
    // Create socket
    connection_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_socket_ == -1) {
        error_message = strerror(errno);
        return;
    }

    // Bind socket
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (bind(connection_socket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1) {
        error_message = strerror(errno);
        return;
    }

    // Listen socket
    if (listen(connection_socket_, 5) == -1) {
        error_message = strerror(errno);
        return;
    }
}

Server::~Server()
{
    close(connection_socket_);
    close(client_socket_);
}

void Server::WaitForConnection(std::string& error_message)
{
    client_socket_ = accept(connection_socket_, NULL, NULL);
    if (client_socket_ == -1) {
        error_message = strerror(errno);
        return;
    }
}

ssize_t Server::ReceiveBytes(char* buffer, size_t length) const
{
    // error.clear();

    char* data_ptr = buffer;

    ssize_t number_of_left_bytes = static_cast<ssize_t>(length);
    while (number_of_left_bytes > 0) {
        ssize_t received_bytes_count = recv(client_socket_, data_ptr, static_cast<size_t>(number_of_left_bytes), 0);
        if (received_bytes_count < 0) {
            if (errno == EINTR) {
                continue;
            }
            // error = std::error_code(errno, std::generic_category());
            return -1; // Error encountered
        }
        if (received_bytes_count == 0) {
            // Connection closed, return data received so far
            return static_cast<ssize_t>(length) - number_of_left_bytes;
        }

        data_ptr += received_bytes_count;
        number_of_left_bytes -= received_bytes_count;
    }

    return 0;
}

std::string Server::ReceiveString(size_t length) const
{
    std::string received_string(length + 1, '\0'); // Pre-allocate string with the desired length

    size_t number_of_left_bytes = length;
    char* data_ptr = received_string.data();

    ReceiveBytes(data_ptr, length);

    return received_string;
}

void Server::SendString(const std::string& str) const
{
    send(client_socket_, str.c_str(), str.size(), 0);
}

