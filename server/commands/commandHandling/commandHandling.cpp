/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#include "commandHandling.hpp"
#include <sstream>

CommandHandling::CommandHandling() = default;

namespace {
std::vector<std::string> parseArgs(const std::string& raw) {
  std::vector<std::string> args;
  std::istringstream iss(raw);
  std::string word;
  while (iss >> word) {
    args.push_back(word);
  }
  return args;
}
}  // namespace

void CommandHandling::handleCommand(const std::string& raw, Client& client,
                                    Server& server) {
  auto args = parseArgs(raw);
  if (args.empty()) {
    return;
  }
  client.setArgs(args);
  auto cmdIter = _commands.find(args[0]);
  if (cmdIter == _commands.end()) {
    return;
  }
  cmdIter->second->execute(client, server);
}
