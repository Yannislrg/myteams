/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** main
*/

#include <cstdlib>
#include <iostream>
#include "server.hpp"

static constexpr int EXIT_ERROR = 84;
static constexpr uint64_t MAX_PORT = 65535;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "USAGE: ./myteams_server port\n";
    return EXIT_ERROR;
  }
  try {
    uint64_t rawPort = std::stoull(argv[1]);
    if (rawPort == 0 || rawPort > MAX_PORT) {
      std::cout << "USAGE: ./myteams_server port\n";
      return EXIT_ERROR;
    }
    auto port = static_cast<uint16_t>(rawPort);
    Server::getInstance().init(port);
    Server::getInstance().run();
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    return EXIT_ERROR;
  }
  return 0;
}
