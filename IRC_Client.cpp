#include "IRC_Client.h"
#include "IRC_Handler.h"

using namespace std;

bool IRC_Client::InitSocket() {
	return _socket.Init();
}
bool IRC_Client::Connect(char* host) {
	return _socket.Connect(host, PORT);
}
void IRC_Client::Disconnect() {
	_socket.Disconnect();
}

bool IRC_Client::SendIRC(string msg) {
	msg.append("\n");
	return _socket.SendData(msg.c_str());
}

bool IRC_Client::Login(string nickname, string username, string password) {

    this->nickname = nickname;
    this->username = username;

    if (SendIRC("HELLO"))
    {
        if (!password.empty() && !SendIRC("PASS " + password))
            return false;
        if (SendIRC("NICK " + nickname))
            if (SendIRC("USER " + username + " 8 * :Cpp IRC Client"))
                return true;
    }

    return false;
}

void IRC_Client::ReceiveData() {
	string buffer = _socket.ReceiveData();

    string line;
    istringstream iss(buffer);
    while (getline(iss, line))
    {
        if (line.find("\r") != string::npos)
            line = line.substr(0, line.size() - 1);
        Parse(line);
    }
}

void IRC_Client::Parse(string msg) {

    string original(msg);
    IRC_CommandPrefix cmdPrefix;

    // if command has prefix
    if (msg.substr(0, 1) == ":")
    {
        cmdPrefix.Parse(msg);
        msg = msg.substr(msg.find(" ") + 1);
    }

    string command = msg.substr(0, msg.find(" "));
    transform(command.begin(), command.end(), command.begin(), towupper);
    if (msg.find(" ") != std::string::npos)
        msg = msg.substr(msg.find(" ") + 1);
    else
        msg = "";

    std::vector<std::string> parameters;

    if (msg != "")
    {
        if (msg.substr(0, 1) == ":")
            parameters.push_back(msg.substr(1));
        else
        {
            size_t pos1 = 0, pos2;
            while ((pos2 = msg.find(" ", pos1)) != std::string::npos)
            {
                parameters.push_back(msg.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
                if (msg.substr(pos1, 1) == ":")
                {
                    parameters.push_back(msg.substr(pos1 + 1));
                    break;
                }
            }
            if (parameters.empty())
                parameters.push_back(msg);
        }
    }

    if (command == "ERROR")
    {
        std::cout << original << std::endl;
        Disconnect();
        return;
    }

    if (command == "PING")
    {
        cout << "I heard PING, I say PONG" << endl;
        SendIRC("PONG :" + parameters.at(0));
        return;
    }

    IRC_Message ircMessage(command, cmdPrefix, parameters);

    // Default handler
    int commandIndex = GetCommandHandler(command);
    if (commandIndex < NumOfCmds)
    {
        IRC_CommandHandler& cmdHandler = IRC_CommandTable[commandIndex];
        (this->*cmdHandler.handler)(ircMessage);
    }
    else if (_debug)
        std::cout << original << std::endl;

    // Try to call hook (if any matches)
    //CallHook(command, ircMessage);
}
