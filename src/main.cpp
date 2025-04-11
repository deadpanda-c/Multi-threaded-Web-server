#include "Server.hpp"
#include "Logging.hpp"

int main(int ac, char **av)
{
  if (ac < 2) {
    Logging::LogError("Usage: ./server <port>");
    return 1;
  }
  unsigned short port = static_cast<unsigned short>(std::stoi(av[1]));

  if (port < 1024 || port > 65535) {
    Logging::LogError("Port must be between 1024 and 65535");
    return 1;
  }
  return 0;
}
