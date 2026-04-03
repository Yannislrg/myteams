/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#include "commandHandling.hpp"
#include <sstream>
#include <string>
#include <vector>
#include "../../client/client.hpp"

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

void CommandHandling::handleCommand(const std::string& rawCommand,
                                    Client& client, Server& server) {
  auto arguments = parseArgs(rawCommand);
  if (arguments.empty()) {
    return;
  }
  client.setArgs(arguments);
  auto commandEntry = _commands.find(arguments[0]);
  if (commandEntry == _commands.end()) {
    return;
  }
  commandEntry->second->execute(client, server);
}
