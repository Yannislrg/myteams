/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerMessageRouter
*/

#include "ServerMessageRouter.hpp"
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include "TokenParser.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "logging_client.h"
#ifdef __cplusplus
}
#endif
#include "handlers/ChannelCreatedHandler.hpp"
#include "handlers/PrivateMessageHandler.hpp"
#include "handlers/ReplyCreatedHandler.hpp"
#include "handlers/ResponseStatusHandler.hpp"
#include "handlers/TeamCreatedHandler.hpp"
#include "handlers/ThreadCreatedHandler.hpp"
#include "handlers/UserLoggedInHandler.hpp"
#include "handlers/UserLoggedOutHandler.hpp"

namespace {
std::optional<int64_t> safeLL(const std::string& str) noexcept {
  try {
    return std::stoll(str);
  } catch (...) {
    return std::nullopt;
  }
}

constexpr std::size_t MIN_TOKENS_BASIC = 4;
constexpr std::size_t MIN_TOKENS_REPLY = 5;
constexpr std::size_t MIN_TOKENS_THREAD = 6;
}  // namespace

ServerMessageRouter::ServerMessageRouter() {
  registerCommandHandlers();
  registerListHandlers();
}

void ServerMessageRouter::setPendingCommand(std::string cmd) {
  _pendingCommand = std::move(cmd);
}

void ServerMessageRouter::registerCommandHandlers() {
  registerHandler(std::make_unique<TeamCreatedHandler>());
  registerHandler(std::make_unique<ChannelCreatedHandler>());
  registerHandler(std::make_unique<ThreadCreatedHandler>());
  registerHandler(std::make_unique<ReplyCreatedHandler>());
  registerHandler(std::make_unique<UserLoggedInHandler>());
  registerHandler(std::make_unique<UserLoggedOutHandler>());
  registerHandler(std::make_unique<PrivateMessageHandler>());
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "200", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "201", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "400", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "401", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "403", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "404", _user, _shouldDisconnect, _pendingCommand));
  registerHandler(std::make_unique<ResponseStatusHandler>(
      "409", _user, _shouldDisconnect, _pendingCommand));
}

void ServerMessageRouter::registerListHandlers() {
  _listHandlers["USERS"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_BASIC) {
      return;
    }
    auto val = safeLL(tok[3]);
    if (!val) {
      return;
    }
    (void)client_print_users(tok[1].c_str(), tok[2].c_str(),
                             static_cast<int>(*val));
  };
  _listHandlers["TEAMS"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_BASIC) {
      return;
    }
    (void)client_print_teams(tok[1].c_str(), tok[2].c_str(), tok[3].c_str());
  };
  _listHandlers["CHANNELS"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_BASIC) {
      return;
    }
    (void)client_team_print_channels(tok[1].c_str(), tok[2].c_str(),
                                     tok[3].c_str());
  };
  _listHandlers["THREADS"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_THREAD) {
      return;
    }
    auto val = safeLL(tok[3]);
    if (!val) {
      return;
    }
    auto timestamp = static_cast<time_t>(*val);
    (void)client_channel_print_threads(
        tok[1].c_str(), tok[2].c_str(), timestamp, tok[4].c_str(),
        tok[5].c_str());  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  };
  _listHandlers["REPLIES"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_REPLY) {
      return;
    }
    auto val = safeLL(tok[3]);
    if (!val) {
      return;
    }
    auto timestamp = static_cast<time_t>(*val);
    (void)client_thread_print_replies(tok[1].c_str(), tok[2].c_str(), timestamp,
                                      tok[4].c_str());
  };
  _listHandlers["MESSAGES"] = [](const std::vector<std::string>& tok) {
    if (tok.size() < MIN_TOKENS_BASIC) {
      return;
    }
    auto val = safeLL(tok[2]);
    if (!val) {
      return;
    }
    auto timestamp = static_cast<time_t>(*val);
    (void)client_private_message_print_messages(tok[1].c_str(), timestamp,
                                                tok[3].c_str());
  };
}

void ServerMessageRouter::registerHandler(
    std::unique_ptr<ICommandHandler> handler) {
  const std::string command = handler->getCommand();
  _handlers[command] = std::move(handler);
}

void ServerMessageRouter::routeFrame(const std::string& frame) {
  const auto tokensResult = TokenParser::parse(frame);
  if (!tokensResult.has_value()) {
    return;
  }
  const std::vector<std::string>& tokens = tokensResult.value();
  if (tokens.empty()) {
    return;
  }
  const std::string& first = tokens[0];

  if (first == "210-BEGIN") {
    if (tokens.size() >= 2) {
      _listContext = tokens[1];
    }
    return;
  }
  if (first == "210-END") {
    _listContext.clear();
    return;
  }
  if (first == "210") {
    const auto listHandlerEntry = _listHandlers.find(_listContext);
    if (listHandlerEntry != _listHandlers.end()) {
      listHandlerEntry->second(tokens);
    }
    return;
  }
  std::string command = first;
  if (command == "EVENT") {
    if (tokens.size() < 2) {
      return;
    }
    command = tokens[1];
    if (command == "USER_LOGGED_OUT" && _pendingCommand == "/logout") {
      return;
    }
  }
  const auto handlerEntry = _handlers.find(command);
  if (handlerEntry == _handlers.end()) {
    return;
  }
  handlerEntry->second->handle(tokens);
}
