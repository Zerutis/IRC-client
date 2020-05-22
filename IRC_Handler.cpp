#include "IRC_Handler.h"

using namespace std;

IRC_CommandHandler IRC_CommandsTable[NumOfCmds] =
{
    { "PRIVMSG",            &IRC_Client::HandlePrivMsg                   },
    { "NOTICE",             &IRC_Client::HandleNotice                    },
    { "JOIN",               &IRC_Client::HandleChannelJoinPart           },
    { "PART",               &IRC_Client::HandleChannelJoinPart           },
    { "NICK",               &IRC_Client::HandleUserNickChange            },
    { "QUIT",               &IRC_Client::HandleUserQuit                  },
    { "353",                &IRC_Client::HandleChannelNamesList          },
    { "433",                &IRC_Client::HandleNicknameInUse             },
    { "001",                &IRC_Client::HandleServerMessage             },
    { "002",                &IRC_Client::HandleServerMessage             },
    { "003",                &IRC_Client::HandleServerMessage             },
    { "004",                &IRC_Client::HandleServerMessage             },
    { "005",                &IRC_Client::HandleServerMessage             },
    { "250",                &IRC_Client::HandleServerMessage             },
    { "251",                &IRC_Client::HandleServerMessage             },
    { "252",                &IRC_Client::HandleServerMessage             },
    { "253",                &IRC_Client::HandleServerMessage             },
    { "254",                &IRC_Client::HandleServerMessage             },
    { "255",                &IRC_Client::HandleServerMessage             },
    { "265",                &IRC_Client::HandleServerMessage             },
    { "266",                &IRC_Client::HandleServerMessage             },
    { "366",                &IRC_Client::HandleServerMessage             },
    { "372",                &IRC_Client::HandleServerMessage             },
    { "375",                &IRC_Client::HandleServerMessage             },
    { "376",                &IRC_Client::HandleServerMessage             },
    { "439",                &IRC_Client::HandleServerMessage             },
};

void IRC_Client::HandleCTCP(IRC_Message message)
{
    string to = message.parameters.at(0);
    string text = message.parameters.at(message.parameters.size() - 1);

    // Remove '\001' from start/end of the string
    text = text.substr(1, text.size() - 2);

    cout << "[" + message.prefix.nick << " requested CTCP " << text << "]" << endl;

    if (to == nickname)
    {
        if (text == "VERSION") // Respond to CTCP VERSION
        {
            SendIRC("NOTICE " + message.prefix.nick + " :\001Rokas Þeruolis\001");
            return;
        }

        // CTCP not implemented
        SendIRC("NOTICE " + message.prefix.nick + " :\001ERRMSG " + text + " :Not implemented\001");
    }
}

void IRC_Client::HandlePrivMsg(IRC_Message message)
{
    string to = message.parameters.at(0);
    string text = message.parameters.at(message.parameters.size() - 1);

    // Handle Client-To-Client Protocol
    if (text[0] == '\001')
    {
        HandleCTCP(message);
        return;
    }

    if (to[0] == '#')
        cout << "From " + message.prefix.nick << " @ " + to + ": " << text << endl;
    else
        cout << "From " + message.prefix.nick << ": " << text << endl;
}

void IRC_Client::HandleNotice(IRC_Message message)
{
    string from = message.prefix.nick != "" ? message.prefix.nick : message.prefix.prefix;
    string text;

    if (!message.parameters.empty())
        text = message.parameters.at(message.parameters.size() - 1);

    if (!text.empty() && text[0] == '\001')
    {
        text = text.substr(1, text.size() - 2);
        if (text.find(" ") == string::npos)
        {
            cout << "[Invalid " << text << " reply from " << from << "]" << endl;
            return;
        }
        string ctcp = text.substr(0, text.find(" "));
        cout << "[" << from << " " << ctcp << " reply]: " << text.substr(text.find(" ") + 1) << endl;
    }
    else
        cout << "-" << from << "- " << text << endl;
}

void IRC_Client::HandleChannelJoinPart(IRC_Message message)
{
    string channel = message.parameters.at(0);
    string action = message.command == "JOIN" ? "joins" : "leaves";
    cout << message.prefix.nick << " " << action << " " << channel << endl;
}

void IRC_Client::HandleUserNickChange(IRC_Message message)
{
    string newNick = message.parameters.at(0);
    cout << message.prefix.nick << " changed his nick to " << newNick << endl;
}

void IRC_Client::HandleUserQuit(IRC_Message message)
{
    string text = message.parameters.at(0);
    cout << message.prefix.nick << " quits (" << text << ")" << endl;
}

void IRC_Client::HandleChannelNamesList(IRC_Message message)
{
    string channel = message.parameters.at(2);
    string nicks = message.parameters.at(3);
    cout << "People on " << channel << ":" << endl << nicks << endl;
}

void IRC_Client::HandleNicknameInUse(IRC_Message message)
{
    cout << message.parameters.at(1) << " " << message.parameters.at(2) << endl;
}

void IRC_Client::HandleServerMessage(IRC_Message message)
{
    if (message.parameters.empty())
        return;

    vector<string>::const_iterator itr = message.parameters.begin();
    ++itr; // skip the first parameter (our nick)
    for (; itr != message.parameters.end(); ++itr)
        cout << *itr << " ";
    cout << endl;
}