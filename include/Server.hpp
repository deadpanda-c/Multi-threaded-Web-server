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
#include <sstream>

#define WRONG_PORT_ERROR "Port number must be between 1024 and 65535"
#define SOCKET_ERROR "Socket creation failed"
#define BIND_ERROR "Binding failed"
#define LISTEN_ERROR "Listening failed"
#define ACCEPT_ERROR "Accepting connection failed"
#define RECV_ERROR "Receiving data failed"
#define SEND_ERROR "Sending data failed"

#define RESPONSE_OK "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n"
#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n"
#define RESPONSE_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n"
#define RESPONSE_INTERNAL_SERVER_ERROR "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n"

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
    std::string processRequest(const std::string& request);

    std::string _host;
    unsigned short _port;
    int _socket;
    bool _running;
    std::vector<std::thread> _threads;
};
