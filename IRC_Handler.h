#ifndef IRC_HANDLER_H
#define IRC_HANDLER_H

#include "IRC_Client.h"

#define NumOfCmds 26

struct IRC_CommandHandler
{
    std::string command;
    void (IRC_Client::* handler)(IRC_Message message);
};

extern IRC_CommandHandler IRC_CommandTable[NumOfCmds];

inline int GetCommandHandler(std::string command)
{
    for (int i = 0; i < NumOfCmds; ++i)
    {
        if (IRC_CommandTable[i].command == command)
            return i;
    }

    return NumOfCmds;
}
#endif // !IRC_HANDLER_H