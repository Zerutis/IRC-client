#pragma once

#include <process.h>
typedef void (*ThreadFunction)(void* param);
typedef void ThreadReturn;
typedef int ThreadId;


class Thread {
    ThreadId _threadId;

public:
    Thread();
    ~Thread();

    bool Start(ThreadFunction callback, void* param);
};

ThreadReturn inputThread(void* client);