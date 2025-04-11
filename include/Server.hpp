#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


class Server {
  public:
    Server(const std::string& host, unsigned short port);
    ~Server();

    void init();
    void start();
    void stop();

  private:
    void acceptConnections();
    void handleClient(int clientSocket);
    void sendResponse(int clientSocket, const std::string& response);
    std::string receiveRequest(int clientSocket);

    std::string _host;
    unsigned short _port;
    int _socket;
    bool _running;
    std::vector<std::thread> _threads;
};
