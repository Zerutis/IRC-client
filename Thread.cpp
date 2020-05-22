#pragma once
#include <string>
#include <iostream>

#include "Thread.h"
#include "IRC_Client.h"


Thread::Thread() : _threadId(0) {}

Thread::~Thread() { }

bool Thread::Start(ThreadFunction callback, void* param) {
    _threadId = _beginthread(callback, 0, param);
    if (_threadId)
        return true;

    return false;
}

ThreadReturn inputThread(void* client)
{
    std::string command;

   /* commandHandler.AddCommand("msg", 2, &msgCommand);
    commandHandler.AddCommand("join", 1, &joinCommand);
    commandHandler.AddCommand("part", 1, &partCommand);
    commandHandler.AddCommand("ctcp", 2, &ctcpCommand);*/

    while (true)
    {
        getline(std::cin, command);
        if (command == "")
            continue;

       // if (command[0] == '/')
      //      commandHandler.ParseCommand(command, (IRC_Client*)client);
       // else
        //    ((IRC_Client*)client)->SendIRC(command);

        if (command == "quit")
            break;
    }
    _endthread();
}