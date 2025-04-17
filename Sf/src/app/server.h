#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

class Server{
public:
    static int server(int port);
};