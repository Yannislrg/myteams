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

bool CommandLineDispatcher::hasUnquotedArgs(std::string_view argsView) {
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
         << "/create \"team_name\" \"team_description\" (no /use context)\n"
         << "/create \"channel_name\" \"channel_description\" (after /use "
            "\"team_uuid\")\n"
         << "/create \"thread_title\" \"thread_message\" (after /use "
            "\"team_uuid\" \"channel_uuid\")\n"
         << "/create \"comment_body\" (after /use \"team_uuid\" "
            "\"channel_uuid\" \"thread_uuid\")\n"
         << "/list (lists teams, channels, threads, or replies depending on "
            "/use context)\n"
         << "/info (shows current user, team, channel, or thread depending "
            "on /use context)\n";
}
