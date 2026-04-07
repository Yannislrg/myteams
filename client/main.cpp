/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** main
*/

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "ClientApplication.hpp"

static constexpr int EXIT_ERROR = 84;

namespace {
void printUsage() { std::cerr << "USAGE: ./myteams_cli ip port\n"; }
}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    printUsage();
    return EXIT_ERROR;
  }

  try {
    const uint16_t port = ClientApplication::parsePort(argv[2]);

    ClientApplication application(argv[1], port);
    application.run();
    return 0;
  } catch (const std::exception& exception) {
    printUsage();
    std::cerr << exception.what() << '\n';
    return EXIT_ERROR;
  }
}
