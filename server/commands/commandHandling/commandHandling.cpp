/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#include "commands/commandHandling/commandHandling.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "commands/create/create.hpp"
#include "commands/info/info.hpp"
#include "commands/list/list.hpp"
#include "commands/subscribe/subscribe.hpp"
#include "commands/unsubscribe/unsubscribe.hpp"
#include "commands/use/use.hpp"

namespace {
std::vector<std::string> parseArgs(const std::string& rawCommand) {
  std::vector<std::string> arguments;
  std::istringstream tokenStream(rawCommand);
  std::string word;
  while (tokenStream >> word) {
    if (word.front() == '"') {
      std::string quotedArg = word.substr(1);
      if (quotedArg.back() == '"') {
        quotedArg.pop_back();
        arguments.push_back(quotedArg);
      } else {
        std::string nextWord;
        while (tokenStream >> nextWord) {
          quotedArg += " " + nextWord;
          if (nextWord.back() == '"') {
            quotedArg.pop_back();
            arguments.push_back(quotedArg);
            break;
          }
        }
      }
    } else {
      arguments.push_back(word);
    }
  }
  return arguments;
}
}  // namespace

CommandHandling::CommandHandling() {
  _commands.emplace("/create", std::make_unique<Create>());
  _commands.emplace("/list", std::make_unique<List>());
  _commands.emplace("/use", std::make_unique<Use>());
  _commands.emplace("/info", std::make_unique<Info>());
  _commands.emplace("/subscribe", std::make_unique<Subscribe>());
  _commands.emplace("/unsubscribe", std::make_unique<Unsubscribe>());
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
    return;
  }
  commandEntry->second->execute(client, server);
}
