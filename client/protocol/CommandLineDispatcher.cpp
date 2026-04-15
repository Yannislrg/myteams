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
#include <string_view>
#include "TokenParser.hpp"

namespace {

struct CommandEntry {
  std::string_view name;
  std::string_view usage;
};

constexpr std::array<CommandEntry, 16> commands = {{
    {.name = "/login", .usage = R"(/login "user_name")"},
    {.name = "/logout", .usage = "/logout"},
    {.name = "/users", .usage = "/users"},
    {.name = "/user", .usage = R"(/user "user_uuid")"},
    {.name = "/send", .usage = R"(/send "user_uuid" "message_body")"},
    {.name = "/messages", .usage = R"(/messages "user_uuid")"},
    {.name = "/subscribe", .usage = R"(/subscribe "team_uuid")"},
    {.name = "/subscribed", .usage = R"(/subscribed ?"team_uuid")"},
    {.name = "/unsubscribe", .usage = R"(/unsubscribe "team_uuid")"},
    {.name = "/use", .usage = R"(/use ?"team_uuid" ?"channel_uuid" ?"thread_uuid")"},
    {.name = "/create",
     .usage = R"(/create "team_name" "team_description" (no /use context))"},
    {.name = "/create",
     .usage = R"(/create "channel_name" "channel_description" (after /use "team_uuid"))"},
    {.name = "/create",
     .usage = R"(/create "thread_title" "thread_message" (after /use "team_uuid" "channel_uuid"))"},
    {.name = "/create",
     .usage = R"(/create "comment_body" (after /use "team_uuid" "channel_uuid" "thread_uuid"))"},
    {.name = "/list",
     .usage = "/list (lists teams, channels, threads, or replies depending on /use context)"},
    {.name = "/info",
     .usage = "/info (shows current user, team, channel, or thread depending on /use context)"},
}};

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
  return std::ranges::any_of(
      commands,
      [command](const CommandEntry& entry) { return entry.name == command; });
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
  output << "/help\n";
  for (const auto& entry : commands) {
    output << entry.usage << '\n';
  }
}
