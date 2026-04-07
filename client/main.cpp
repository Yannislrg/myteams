/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** main
*/

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include "ClientApplication.hpp"

static constexpr int EXIT_ERROR = 84;

namespace {
void printUsage(std::ostream& ostream) {
  ostream
      << "USAGE: ./myteams_cli ip port\n\n"
         "    ip is the server ip address on which the server socket listens\n"
         "    port is the port number on which the server socket listens\n";
}
}  // namespace

int main(int argc, char** argv) {
  if (argc == 2 && std::string_view(argv[1]) == "--help") {
    printUsage(std::cout);
    return 0;
  }

  if (argc != 3) {
    printUsage(std::cerr);
    return EXIT_ERROR;
  }

  try {
    const uint16_t port = ClientApplication::parsePort(argv[2]);

    ClientApplication application(argv[1], port);
    application.run();
    return 0;
  } catch (const std::exception& exception) {
    printUsage(std::cerr);
    std::cerr << exception.what() << '\n';
    return EXIT_ERROR;
  }
}
