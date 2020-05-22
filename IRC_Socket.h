#pragma once
#include <iostream>
#include <string>
#include <sstream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <fcntl.h>

const int PORT = 6667;
const int MAX_DATA = 4096;

class IRC_Socket {
	int _socket;
    bool _connected;

public:
    IRC_Socket() :_socket(0), _connected(false) {}

    bool Init();

    bool Connect(char const* host, int port);
    void Disconnect();

    bool Connected() { return _connected; };
    bool SendData(char const* data);
    std::string ReceiveData();
};