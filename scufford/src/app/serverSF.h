#pragma once

#include <string>
#include <cstdint>
#include <system_error>
#include <iostream>   
#include <fstream>
#include <pugixml.hpp>  
#include <string>     
#include <optional>


class ServerSF {
  public:
    ServerSF(uint16_t port, std::string& error_message);
    ~ServerSF();

    void WaitForConnection(std::string& error_message);
    template <typename T>
    T Receive() const;
    template <typename T>
    void Send(T data) const;
    std::string ReceiveString(size_t length) const;
    void SendString(const std::string& str) const;
    void Start();

  private:
    int connection_socket_;
    int client_socket_;

    ssize_t ReceiveBytes(char* buffer, size_t length) const;
};




enum class ErrorCode {
  NO_ERRORS,
  WRONG_START_OF_RESOURCE_NAME,
};

struct RawRequest {
  std::string resource_name;
  std::string xml_string;
};

std::optional<RawRequest> ReceiveRequest(const ServerSF& server, ErrorCode& error);