#pragma once
#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H

#include <iostream>
#include <string>
#include <fcntl.h>
#include <algorithm>
#include <vector>
#include <list>

#include "IRC_Socket.h"
#include "IRC_Message.h"



class IRC_Client {
    std::string prefix;
    std::string nickname;
    std::string username;
    std::string host;

    IRC_Socket _socket;
    bool _debug;


public:
    IRC_Client() : _debug(false) { }

    bool InitSocket();
    bool Connect(char* host);
    void Disconnect();
    bool Connected() { return _socket.Connected(); };

    bool SendIRC(std::string msg);

    bool Login(std::string nickname, std::string username, std::string password = std::string());

    void Parse(std::string msg);
    void ReceiveData();

  //  void HookIRCCommand(std::string command, void (*function)(IRC_Message msg, IRC_Client* ));

    // Default internal handlers
    void HandleCTCP(IRC_Message msg);
    void HandlePrivMsg(IRC_Message msg);
    void HandleNotice(IRC_Message msg);
    void HandleChannelJoinPart(IRC_Message msg);
    void HandleUserNickChange(IRC_Message msg);
    void HandleUserQuit(IRC_Message msg);
    void HandleChannelNamesList(IRC_Message msg);
    void HandleNicknameInUse(IRC_Message msg);
    void HandleServerMessage(IRC_Message msg);
};

#endif // !IRC_CLIENT.H