#pragma once
#include <string>
#include <vector>

extern std::vector<std::string> split(std::string const& text, char sep) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos)
        {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
}

struct IRC_CommandPrefix {
    std::string prefix;
    std::string nick;
    std::string user;
    std::string host;

    void Parse(std::string data)
    {
        if (data == "")
            return;

        prefix = data.substr(1, data.find(" ") - 1);
        std::vector<std::string> tokens;

        if (prefix.find("@") != std::string::npos)
        {
            tokens = split(prefix, '@');
            nick = tokens.at(0);
            host = tokens.at(1);
        }
        if (nick != "" && nick.find("!") != std::string::npos)
        {
            tokens = split(nick, '!');
            nick = tokens.at(0);
            user = tokens.at(1);
        }
    };
};

struct IRC_Message {
    std::string command;
    IRC_CommandPrefix prefix;
    std::vector<std::string> parameters;

    IRC_Message() {}
    IRC_Message(std::string cmd, IRC_CommandPrefix p, std::vector<std::string> params) :
        command(cmd), prefix(p), parameters(params) {}
};