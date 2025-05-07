#include "Server.hpp"
#include "Logging.hpp"

Server::Server(unsigned short port) : _host("0.0.0.0"), _port(port), _socket(-1), _running(false) {
  if (port < 1024 || port > 65535)
    throw ServerException(WRONG_PORT_ERROR);
}

Server::~Server() {
  stop();
}

void Server::init() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket < 0) {
      throw ServerException(SOCKET_ERROR);
  }

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(_port);

  if (bind(_socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
    throw ServerException(BIND_ERROR);
  }

  if (listen(_socket, 5) < 0) {
    throw ServerException(LISTEN_ERROR);
  }

  _running = true;
  Logging::Log("Server initialized on port " + std::to_string(_port));
}

void Server::start() {
  std::cout << "Server started, waiting for connections..." << std::endl;
  acceptConnections();
}

void Server::stop() {
  if (_running) {
    _running = false;
    close(_socket);
    for (auto& thread : _threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
    Logging::Log("Server stopped");
  }
}

void Server::acceptConnections() {
  while (_running) {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);

    if (clientSocket < 0) {
      if (_running) {
        throw ServerException(ACCEPT_ERROR);
      }
      continue;
    }

    _threads.emplace_back(&Server::handleClient, this, clientSocket);
  }
}

std::string Server::processRequest(const std::string& request)
{
    std::istringstream stream(request);
    std::string method, path, version;
    stream >> method >> path >> version;

    if (method == "GET") {
        if (path == "/") {
            std::string body = "Hello, World!";
            return "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
        } else if (path == "/ping") {
            std::string body = "pong";
            return "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
        }
    } else if (method == "POST") {
        if (path == "/submit") {
            std::string body = "Submitted";
            return "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
        }
    }

    std::string body = "Unsupported request";
    return "HTTP/1.1 400 Bad Request\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
}

void Server::handleClient(int clientSocket) {
  try {
    std::string request = receiveRequest(clientSocket);
    Logging::Log("Received request: " + request);

    std::string response = processRequest(request);
    sendResponse(clientSocket, response);
  } catch (const std::exception& e) {
    Logging::LogError("Error handling client: " + std::string(e.what()));
  }

  close(clientSocket);
}

std::string Server::receiveRequest(int clientSocket) {
  char buffer[1024];
  ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

  if (bytesReceived <= 0)
    throw ServerException(RECV_ERROR);

  buffer[bytesReceived] = '\0';
  return std::string(buffer);
}

void Server::sendResponse(int clientSocket, const std::string& response) {
  if (send(clientSocket, response.c_str(), response.length(), 0) < 0) {
    throw ServerException(SEND_ERROR);
  }
}
