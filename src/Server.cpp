#include "Server.hpp"

Server::Server(const std::string &host, unsigned short port) : _host(host), _port(port), _running(false)
{

}

Server::~Server()
{
  close(_socket);
}

void Server::init()
{

}

void Server::start()
{
  _running = true;

}

void Server::stop()
{
  _running = false;
  close(_socket);
}

void Server::acceptConnections()
{

}

void Server::handleClient(int clientSocket)
{

}

void Server::sendResponse(int clientSocket, const std::string &response)
{
  send(clientSocket, response.c_str(), response.size(), 0);
}

std::string Server::receiveRequest(int clientSocket)
{
  return std::string();
}
