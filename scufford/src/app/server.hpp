#ifndef FORTRESS_SERVER_HPP_
#define FORTRESS_SERVER_HPP_
#include "request_receiver.hpp"

#include <string>
#include <cstdint>
#include <system_error>
#include <iostream>   
#include <fstream>
#include <pugixml.hpp>  
#include <string>     

class Server {
  public:
    Server(uint16_t port, std::string& error_message);
    ~Server();

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

#include "server_impl.hpp"

#endif 
