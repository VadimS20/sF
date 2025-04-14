#include "serverSF.h"

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <array>
#include <system_error>
#include <vector>
#include <iostream>


template <typename T>
T ServerSF::Receive() const
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
void ServerSF::Send(T data) const
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


ServerSF::ServerSF(uint16_t port, std::string& error_message)
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

ServerSF::~ServerSF()
{
    close(connection_socket_);
    close(client_socket_);
}

void ServerSF::WaitForConnection(std::string& error_message)
{
    client_socket_ = accept(connection_socket_, NULL, NULL);
    if (client_socket_ == -1) {
        error_message = strerror(errno);
        return;
    }
}

ssize_t ServerSF::ReceiveBytes(char* buffer, size_t length) const
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

void ServerSF::Start(){
        std::string erorr_message;

        WaitForConnection(erorr_message);
        std::cout << erorr_message << std::endl;

        ErrorCode error;

        // Открываем файл для записи (режим добавления в конец файла)
        std::ofstream outfile("received_data.fboot",std::ios::binary);

        while (true) {
            auto received_request = ReceiveRequest(*this, error).value();
            if (received_request.xml_string.length() == 1) {
                std::cout << "CONNECTION CLOSED" << '\n';
                break;
            }

            // Записываем полученные данные в файл
            outfile << received_request.xml_string<< "\n";
            std::cout<< received_request.xml_string << "\n";

            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_string(received_request.xml_string.c_str());

            pugi::xml_node root = doc.document_element();

            if (std::string(root.attribute("Action").as_string()) == "CREATE") {
                pugi::xml_node child = root.first_child();
                //std::cout << std::string(child.name()) << std::endl;
            }

            pugi::xml_attribute id_attribute = root.attribute("ID");
            uint response_index = id_attribute.as_uint();

            std::string respose = "<Response ID=\"" + std::to_string(response_index) + "\" />";
            Send<char>('P');
            Send<uint16_t>(respose.size());
            SendString(respose);
            //std::cout << "Sent response: " << respose << '\n';

            //std::cout << "\n\n";

            //std::cout << "Received request: " << received_request.xml_string << '\n';
        }


        outfile.close();

}

std::string ServerSF::ReceiveString(size_t length) const
{
    std::string received_string(length+1 , '\0'); // Pre-allocate string with the desired length

    size_t number_of_left_bytes = length;
    char* data_ptr = received_string.data();

    ReceiveBytes(data_ptr, length);

    return received_string;
}

void ServerSF::SendString(const std::string& str) const
{
    send(client_socket_, str.c_str(), str.size(), 0);
}








std::optional<RawRequest> ReceiveRequest(const ServerSF& server, ErrorCode& error)
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