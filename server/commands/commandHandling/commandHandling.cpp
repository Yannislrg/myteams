/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#include "commandHandling.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "../../client/client.hpp"
#include "../create/create.hpp"

namespace {
std::vector<std::string> parseArgs(const std::string& rawCommand) {
  std::vector<std::string> arguments;
  std::istringstream tokenStream(rawCommand);
  std::string word;
  while (tokenStream >> word) {
    arguments.push_back(word);
  }
  return arguments;
}
}  // namespace

CommandHandling::CommandHandling() {
  _commands.emplace("/create", std::make_unique<Create>());
}

void CommandHandling::handleCommand(const std::string& rawCommand,
                                    Client& client, Server& server) {
  auto arguments = parseArgs(rawCommand);
  if (arguments.empty()) {
    return;
  }
  client.setArgs(arguments);
  auto commandEntry = _commands.find(arguments[0]);
  if (commandEntry == _commands.end()) {
    std::cerr << "Unknown command: " << arguments[0] << std::endl;
    return;
  }
  std::cout << "Executing command: " << arguments[0] << std::endl;
  commandEntry->second->execute(client, server);
}
