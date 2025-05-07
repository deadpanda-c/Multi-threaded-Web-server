#include "Server.hpp"
#include "Logging.hpp"

int main(int ac, char **av)
{
  if (ac < 2) {
    Logging::LogError("Usage: ./server <port>");
    return 1;
  }
  try {
    unsigned short port = static_cast<unsigned short>(std::stoi(av[1]));
    Server server(port);

    server.init();
    server.start();

  } catch (Server::ServerException &e) {
    Logging::LogError("Server exception: " + std::string(e.what()));
  }
  return 0;
}
