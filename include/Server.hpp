#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>

#define WRONG_PORT_ERROR "Port number must be between 1024 and 65535"

class Server {
  public:
    class ServerException : public std::exception {
      public:
        ServerException(const std::string& message) : _message(message) {}
        const char* what() const noexcept override { return _message.c_str(); }
      private:
        std::string _message;
    };
    Server(unsigned short port);
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
