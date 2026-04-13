/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** main
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include "server.hpp"

static constexpr int EXIT_ERROR = 84;
static constexpr uint64_t MAX_PORT = 65535;

namespace {

void printUsage() {
  std::cout
      << "USAGE: ./myteams_server port\n\n"
      << "    port is the port number on which the server socket listens\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    printUsage();
    return 0;
  }

  if (argc != 2) {
    printUsage();
    return EXIT_ERROR;
  }
  try {
    uint64_t rawPort = std::stoull(argv[1]);
    if (rawPort == 0 || rawPort > MAX_PORT) {
      printUsage();
      return EXIT_ERROR;
    }
    auto port = static_cast<uint16_t>(rawPort);
    Server::getInstance().init(port);
    Server::getInstance().run();
  } catch (const std::exception& e) {
    (void)e;
    printUsage();
    return EXIT_ERROR;
  }
  return 0;
}
