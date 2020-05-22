#include "IRC_Socket.h"

using namespace std;

bool IRC_Socket::Init() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        cout << "Error: initializing Winsock." << endl;
        return false;
    }

    if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cout << "Error: can`t create a socket" << endl;
        WSACleanup();
        return false;
    }

    int on = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char const*)&on, sizeof(on)) == -1) {
        cout << "Error: Invalid socket." << endl;
        WSACleanup();
        return false;
    }

    u_long mode = 0;
    ioctlsocket(_socket, FIONBIO, &mode);

    return true;
}

bool IRC_Socket::Connect(char const* host, int port) {
    hostent* he;

    if (!(he = gethostbyname(host))) {
        cout << "Error: Couldn`t resolve host: " << host << endl;
        WSACleanup();
        return false;
    }

    sockaddr_in addr;

    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((const in_addr*)he->h_addr);
    memset(&(addr.sin_zero), '\0', 8);

    if (connect(_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Error: Couldn`t connect to: " << host << endl;
        closesocket(_socket);
        return false;
    }

    _connected = true;

    return true;
}
void IRC_Socket::Disconnect() {
    if (_connected) {
        shutdown(_socket, 2);
        closesocket(_socket);
        _connected = false;
    }
}


bool IRC_Socket::SendData(char const* msg) {
    if (_connected)
        if (send(_socket, msg, strlen(msg), 0) == -1) {
            cout << "Error: Couldn`t send a message." << endl;
            return false;
        }
    return true;
}

string IRC_Socket::ReceiveData() {
    char buffer[MAX_DATA];

    memset(buffer, 0, MAX_DATA);

    int bytes = recv(_socket, buffer, MAX_DATA - 1, 0);

    if (bytes > 0)
        return string(buffer);
    else
        Disconnect();

    return "Error, receiving the message.\n";
}