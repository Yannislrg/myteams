/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ResponseStatusHandler
*/

#include "ResponseStatusHandler.hpp"
#include <ctime>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include "LoggingClientC.hpp"

namespace {
constexpr std::size_t minHeaderTokens = 3;

constexpr std::size_t userMinTokens = 5;
constexpr std::size_t infoMinTokens = 6;
constexpr std::size_t threadInfoMinTokens = 8;
constexpr std::size_t subscribeMinTokens = 4;
constexpr std::size_t loginTokens = 4;

constexpr std::size_t teamChannelMinTokens = 6;
constexpr std::size_t threadCreateMinTokens = 8;
constexpr std::size_t replyCreateMinTokens = 7;

constexpr std::size_t userUuidIndex = 2;
constexpr std::size_t userNameIndex = 3;
constexpr std::size_t userStatusIndex = 4;
constexpr std::size_t teamUuidIndex = 3;

constexpr std::size_t infoUuidIndex = 3;
constexpr std::size_t infoNameIndex = 4;
constexpr std::size_t infoDescriptionIndex = 5;

constexpr std::size_t threadTimestampIndex = 5;
constexpr std::size_t threadTitleIndex = 6;
constexpr std::size_t threadBodyIndex = 7;

template <typename T, typename ParseFunc>
bool tryParse(const std::string& value, T& out, ParseFunc parseFunc) {
  try {
    out = parseFunc(value);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

std::optional<time_t> parseTimestamp(const std::string& value) {
  time_t timestamp = 0;
  if (!tryParse(value, timestamp, [](const std::string& raw) {
        return static_cast<time_t>(std::stoll(raw));
      })) {
    return std::nullopt;
  }
  return timestamp;
}
}  // namespace

ResponseStatusHandler::ResponseStatusHandler(std::string code, User& user,
                                             bool& shouldDisconnect,
                                             const std::string& pendingCommand)
    : _code(std::move(code)),
      _user(user),
      _shouldDisconnect(shouldDisconnect),
      _pendingCommand(pendingCommand) {}

void ResponseStatusHandler::handle(const Tokens& tokens) const {
  using StatusHandler = void (ResponseStatusHandler::*)(const Tokens&) const;
  static const std::unordered_map<std::string, StatusHandler> handlers = {
      {"200", &ResponseStatusHandler::handle200},
      {"201", &ResponseStatusHandler::handle201},
      {"400", &ResponseStatusHandler::handle400},
      {"401", &ResponseStatusHandler::handle401},
      {"403", &ResponseStatusHandler::handle403},
      {"404", &ResponseStatusHandler::handle404},
      {"409", &ResponseStatusHandler::handle409},
  };

  const auto iterator = handlers.find(_code);
  if (iterator == handlers.end()) {
    return;
  }
  (this->*(iterator->second))(tokens);
}

void ResponseStatusHandler::handle200(const Tokens& tokens) const {
  if (tokens.size() < 2) {
    return;
  }

  using OkHandler = void (ResponseStatusHandler::*)(const Tokens&) const;
  static const std::unordered_map<std::string, OkHandler> handlers = {
      {"OK", &ResponseStatusHandler::handle200Ok},
      {"USER", &ResponseStatusHandler::handle200User},
      {"SUBSCRIBE", &ResponseStatusHandler::handle200Subscribe},
      {"UNSUBSCRIBE", &ResponseStatusHandler::handle200Unsubscribe},
      {"INFO", &ResponseStatusHandler::handle200Info},
  };

  const auto iterator = handlers.find(tokens[1]);
  if (iterator == handlers.end()) {
    return;
  }
  (this->*(iterator->second))(tokens);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle201(const Tokens& tokens) const {
  if (tokens.size() < minHeaderTokens) {
    return;
  }
  const std::string& type = tokens[2];

  if (type == "TEAM" && tokens.size() >= teamChannelMinTokens) {
    (void)client_print_team_created(tokens[3].c_str(), tokens[4].c_str(),
                                    tokens[infoDescriptionIndex].c_str());
    return;
  }
  if (type == "CHANNEL" && tokens.size() >= teamChannelMinTokens) {
    (void)client_print_channel_created(tokens[3].c_str(), tokens[4].c_str(),
                                       tokens[infoDescriptionIndex].c_str());
    return;
  }
  if (type == "THREAD" && tokens.size() >= threadCreateMinTokens) {
    const auto timestamp = parseTimestamp(tokens[infoDescriptionIndex]);
    if (!timestamp.has_value()) {
      return;
    }
    (void)client_print_thread_created(
        tokens[3].c_str(), tokens[4].c_str(), timestamp.value(),
        tokens[threadTitleIndex].c_str(), tokens[threadBodyIndex].c_str());
    return;
  }
  if (type == "REPLY" && tokens.size() >= replyCreateMinTokens) {
    constexpr std::size_t replyBodyIndex = 6;
    const auto timestamp = parseTimestamp(tokens[infoDescriptionIndex]);
    if (!timestamp.has_value()) {
      return;
    }
    (void)client_print_reply_created(tokens[3].c_str(), tokens[4].c_str(),
                                     timestamp.value(),
                                     tokens[replyBodyIndex].c_str());
  }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle400(const Tokens& /*tokens*/) const {
  std::cout << "Error: bad request\n";
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle401(const Tokens& /*tokens*/) const {
  (void)client_error_unauthorized();
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle403(const Tokens& /*tokens*/) const {
  (void)client_error_unauthorized();
}

void ResponseStatusHandler::handle404(const Tokens& tokens) const {
  if (tokens.size() >= 4) {
    const std::string& type = tokens[2];
    const std::string& uuid = tokens[3];

    if (type == "TEAM") {
      (void)client_error_unknown_team(uuid.c_str());
    } else if (type == "CHANNEL") {
      (void)client_error_unknown_channel(uuid.c_str());
    } else if (type == "THREAD") {
      (void)client_error_unknown_thread(uuid.c_str());
    } else if (type == "USER") {
      (void)client_error_unknown_user(uuid.c_str());
    }
    return;
  }
  if (tokens.size() >= 3) {
    if (_pendingCommand == "/user" || _pendingCommand == "/send" ||
        _pendingCommand == "/messages") {
      (void)client_error_unknown_user(tokens[2].c_str());
    }
    return;
  }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle409(const Tokens& /*tokens*/) const {
  (void)client_error_already_exist();
}

void ResponseStatusHandler::handle200Ok(const Tokens& tokens) const {
  if (tokens.size() >= loginTokens) {
    _user.login(tokens[userUuidIndex], tokens[userNameIndex]);
    return;
  }
  if (_pendingCommand == "/logout") {
    (void)client_event_logged_out(_user.getUuid().c_str(),
                                  _user.getName().c_str());
    _user.logout();
    _shouldDisconnect = true;
  }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200User(const Tokens& tokens) const {
  if (tokens.size() < userMinTokens) {
    return;
  }
  int status = 0;
  if (!tryParse(tokens[userStatusIndex], status,
                [](const std::string& value) { return std::stoi(value); })) {
    return;
  }
  (void)client_print_user(tokens[userUuidIndex].c_str(),
                          tokens[userNameIndex].c_str(), status);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200Subscribe(const Tokens& tokens) const {
  if (tokens.size() < subscribeMinTokens) {
    return;
  }
  const auto& teamUuid = tokens[teamUuidIndex];
  (void)client_print_subscribed(tokens[userUuidIndex].c_str(),
                                teamUuid.c_str());
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200Unsubscribe(const Tokens& tokens) const {
  if (tokens.size() < subscribeMinTokens) {
    return;
  }
  const auto& teamUuid = tokens[teamUuidIndex];
  (void)client_print_unsubscribed(tokens[userUuidIndex].c_str(),
                                  teamUuid.c_str());
}

void ResponseStatusHandler::handle200Info(const Tokens& tokens) const {
  if (tokens.size() < minHeaderTokens) {
    return;
  }

  using InfoHandler = void (ResponseStatusHandler::*)(const Tokens&) const;
  static const std::unordered_map<std::string, InfoHandler> handlers = {
      {"USER", &ResponseStatusHandler::handle200InfoUser},
      {"TEAM", &ResponseStatusHandler::handle200InfoTeam},
      {"CHANNEL", &ResponseStatusHandler::handle200InfoChannel},
      {"THREAD", &ResponseStatusHandler::handle200InfoThread},
  };

  const auto iterator = handlers.find(tokens[2]);
  if (iterator == handlers.end()) {
    return;
  }
  (this->*(iterator->second))(tokens);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200InfoUser(const Tokens& tokens) const {
  if (tokens.size() < infoMinTokens) {
    return;
  }
  int status = 0;
  if (!tryParse(tokens[infoDescriptionIndex], status,
                [](const std::string& value) { return std::stoi(value); })) {
    return;
  }
  (void)client_print_user(tokens[infoUuidIndex].c_str(),
                          tokens[infoNameIndex].c_str(), status);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200InfoTeam(const Tokens& tokens) const {
  if (tokens.size() < infoMinTokens) {
    return;
  }
  (void)client_print_team(tokens[infoUuidIndex].c_str(),
                          tokens[infoNameIndex].c_str(),
                          tokens[infoDescriptionIndex].c_str());
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200InfoChannel(const Tokens& tokens) const {
  if (tokens.size() < infoMinTokens) {
    return;
  }
  (void)client_print_channel(tokens[infoUuidIndex].c_str(),
                             tokens[infoNameIndex].c_str(),
                             tokens[infoDescriptionIndex].c_str());
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ResponseStatusHandler::handle200InfoThread(const Tokens& tokens) const {
  if (tokens.size() < threadInfoMinTokens) {
    return;
  }
  const auto timestamp = parseTimestamp(tokens[threadTimestampIndex]);
  if (!timestamp.has_value()) {
    return;
  }
  (void)client_print_thread(tokens[infoUuidIndex].c_str(),
                            tokens[infoNameIndex].c_str(), timestamp.value(),
                            tokens[threadTitleIndex].c_str(),
                            tokens[threadBodyIndex].c_str());
}
