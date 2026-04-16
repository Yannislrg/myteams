/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** CommandLineDispatcher
*/

#include "CommandLineDispatcher.hpp"
#include <algorithm>
#include <cctype>
#include <ostream>
#include <string>
#include "CommandSchemas.hpp"
#include "TokenParser.hpp"

namespace {

bool hasUnquotedArgs(std::string_view argsView) {
  std::size_t pos = 0;
  while (pos < argsView.size()) {
    while (pos < argsView.size() &&
           std::isspace(static_cast<unsigned char>(argsView[pos])) != 0) {
      ++pos;
    }
    if (pos >= argsView.size()) {
      break;
    }
    if (argsView[pos] != '"') {
      return true;
    }
    ++pos;
    while (pos < argsView.size() && argsView[pos] != '"') {
      ++pos;
    }
    if (pos >= argsView.size()) {
      return true;
    }
    ++pos;
  }
  return false;
}

const CommandSchema* findSchema(std::string_view command, std::size_t argc) {
  for (const auto& schema : schemas) {
    if (schema.name == command && argc >= schema.minArgs &&
        argc <= schema.maxArgs) {
      return &schema;
    }
  }
  return nullptr;
}

}  // namespace

std::string CommandLineDispatcher::trimLeft(const std::string& line) {
  std::size_t index = 0;
  while (index < line.size() &&
         std::isspace(static_cast<unsigned char>(line[index])) != 0) {
    ++index;
  }
  return line.substr(index);
}

std::optional<std::string> CommandLineDispatcher::extractCommand(
    const std::string& line) {
  const auto tokens = TokenParser::parse(line);
  if (!tokens.has_value() || tokens->empty()) {
    return std::nullopt;
  }
  return tokens->front();
}

bool CommandLineDispatcher::isSupportedServerCommand(std::string_view command) {
  return std::ranges::any_of(schemas, [command](const CommandSchema& schema) {
    return schema.name == command;
  });
}

std::optional<std::string> CommandLineDispatcher::validateArgs(
    std::string_view command, std::string_view argsView) {
  if (hasUnquotedArgs(argsView)) {
    return "Invalid command format: arguments must be quoted";
  }
  const auto tokens = TokenParser::parse(std::string(argsView));
  if (!tokens.has_value()) {
    return "Invalid command format";
  }
  const CommandSchema* schema = findSchema(command, tokens->size());
  if (schema == nullptr) {
    return "Wrong number of arguments for " + std::string(command);
  }
  for (std::size_t i = 0; i < tokens->size(); ++i) {
    if ((*tokens)[i].size() > schema->args[i].maxLen) {
      return std::string(schema->args[i].name) + " exceeds max length of " +
             std::to_string(schema->args[i].maxLen);
    }
  }
  return std::nullopt;
}

void CommandLineDispatcher::printHelp(std::ostream& output) {
  output << "/help\n";
  for (const auto& schema : schemas) {
    output << schema.usage << '\n';
  }
}
