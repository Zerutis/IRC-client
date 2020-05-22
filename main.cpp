#include <iostream>
#include <string>
#include <signal.h>
#include <cstdlib>

#include "IRC_Socket.h"
#include "IRC_Client.h"
#include "IRC_Signals.h"
#include "Thread.h"

using namespace std;

int main(int argc, char* argv[]) {
    char* host = new char[30];
    char* channel = new char[20];
    char* nickname = new char[8];

    if (argc == 1) {
        cout << "Hello, this is an IRC client.\n Please enter: Host Channel Nickname\n";
        cout << "host: ";
        cin >> host;
        cout << "Channel: ";
        cin >> channel;
        cout << "Nickname: ";
        cin >> nickname;
    }
    else if (argc >=4){
        cout << "Hello, this is an IRC client.\n";
        host = argv[1];
        channel = argv[2];
        nickname = argv[3];
        cout << "You are trying to connect to\nHost: " << host << "\nChannel: " << channel << "\nWith nickname: " << nickname << endl;
    }

    IRC_Client client;

    Thread thread;
    thread.Start(&inputThread,&client);

    if (client.InitSocket()) {
        cout << "Socket initialized. CONNECTING...\n";
        if (client.Connect(host)) {
            cout << "Connected Successfully\n Logging...\n";


            if (client.Login(nickname, "Zerutis007")) {
                cout << "Logged!!!\n";

                running = true;
                signal(SIGINT, signalHandler);

                while (client.Connected() && running)
                    client.ReceiveData();
            }
            if (client.Connected())
                client.Disconnect();

            cout << "Disconnected from host.\n";
        }
    }

    return 0;
}