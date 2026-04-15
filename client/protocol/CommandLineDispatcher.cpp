/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** CommandLineDispatcher
*/

#include "CommandLineDispatcher.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <ostream>
#include "TokenParser.hpp"

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
  static constexpr std::array<std::string_view, 13> serverCommands = {
      "/login",    "/logout",    "/users",      "/user",        "/send",
      "/messages", "/subscribe", "/subscribed", "/unsubscribe", "/use",
      "/create",   "/list",      "/info",
  };

  return std::ranges::find(serverCommands, command) != serverCommands.end();
}

void CommandLineDispatcher::printHelp(std::ostream& output) {
  output << "/help\n"
         << "/login \"user_name\"\n"
         << "/logout\n"
         << "/users\n"
         << "/user \"user_uuid\"\n"
         << "/send \"user_uuid\" \"message_body\"\n"
         << "/messages \"user_uuid\"\n"
         << "/subscribe \"team_uuid\"\n"
         << "/subscribed ?\"team_uuid\"\n"
         << "/unsubscribe \"team_uuid\"\n"
         << "/use ?\"team_uuid\" ?\"channel_uuid\" ?\"thread_uuid\"\n"
         << "/create\n"
         << "/list\n"
         << "/info\n";
}
