/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerMessageRouter
*/

#include "ServerMessageRouter.hpp"
#include <ctime>
#include <memory>
#include <string>
#include "TokenParser.hpp"
#include "handlers/ChannelCreatedHandler.hpp"
#include "handlers/LoggingClientC.hpp"
#include "handlers/PrivateMessageHandler.hpp"
#include "handlers/ReplyCreatedHandler.hpp"
#include "handlers/Response200Handler.hpp"
#include "handlers/Response201Handler.hpp"
#include "handlers/ResponseErrorHandler.hpp"
#include "handlers/TeamCreatedHandler.hpp"
#include "handlers/ThreadCreatedHandler.hpp"
#include "handlers/UserLoggedInHandler.hpp"
#include "handlers/UserLoggedOutHandler.hpp"

ServerMessageRouter::ServerMessageRouter() {
  registerHandler(std::make_unique<TeamCreatedHandler>());
  registerHandler(std::make_unique<ChannelCreatedHandler>());
  registerHandler(std::make_unique<ThreadCreatedHandler>());
  registerHandler(std::make_unique<ReplyCreatedHandler>());
  registerHandler(std::make_unique<UserLoggedInHandler>());
  registerHandler(std::make_unique<UserLoggedOutHandler>());
  registerHandler(std::make_unique<PrivateMessageHandler>());
  registerHandler(std::make_unique<Response200Handler>());
  registerHandler(std::make_unique<Response201Handler>());
  registerHandler(std::make_unique<ResponseErrorHandler>("400"));
  registerHandler(std::make_unique<ResponseErrorHandler>("401"));
  registerHandler(std::make_unique<ResponseErrorHandler>("403"));
  registerHandler(std::make_unique<ResponseErrorHandler>("404"));
  registerHandler(std::make_unique<ResponseErrorHandler>("409"));
}

void ServerMessageRouter::registerHandler(
    std::unique_ptr<ICommandHandler> handler) {
  const std::string command = handler->getCommand();
  _handlers[command] = std::move(handler);
}

void ServerMessageRouter::handleListItem(
    const std::vector<std::string>& tokens) const {
  constexpr std::size_t MIN_USER_TEAM_CHAN = 4;
  constexpr std::size_t MIN_THREAD = 6;
  constexpr std::size_t MIN_REPLY = 5;

  if (_listContext == "USERS" && tokens.size() >= MIN_USER_TEAM_CHAN) {
    (void)client_print_users(tokens[1].c_str(), tokens[2].c_str(),
                             std::stoi(tokens[3]));
  } else if (_listContext == "TEAMS" && tokens.size() >= MIN_USER_TEAM_CHAN) {
    (void)client_print_teams(tokens[1].c_str(), tokens[2].c_str(),
                             tokens[3].c_str());
  } else if (_listContext == "CHANNELS" &&
             tokens.size() >= MIN_USER_TEAM_CHAN) {
    (void)client_team_print_channels(tokens[1].c_str(), tokens[2].c_str(),
                                     tokens[3].c_str());
  } else if (_listContext == "THREADS" && tokens.size() >= MIN_THREAD) {
    auto timestamp = static_cast<time_t>(std::stoll(tokens[3]));
    (void)client_channel_print_threads(
        tokens[1].c_str(), tokens[2].c_str(), timestamp, tokens[4].c_str(),
        tokens[5].c_str());  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  } else if (_listContext == "REPLIES" && tokens.size() >= MIN_REPLY) {
    auto timestamp = static_cast<time_t>(std::stoll(tokens[3]));
    (void)client_thread_print_replies(tokens[1].c_str(), tokens[2].c_str(),
                                      timestamp, tokens[4].c_str());
  } else if (_listContext == "MESSAGES" &&
             tokens.size() >= MIN_USER_TEAM_CHAN) {
    auto timestamp = static_cast<time_t>(std::stoll(tokens[2]));
    (void)client_private_message_print_messages(tokens[1].c_str(), timestamp,
                                                tokens[3].c_str());
  }
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
    handleListItem(tokens);
    return;
  }
  std::string command = first;
  if (command == "EVENT") {
    if (tokens.size() < 2) {
      return;
    }
    command = tokens[1];
  }
  const auto handlerEntry = _handlers.find(command);
  if (handlerEntry == _handlers.end()) {
    return;
  }
  handlerEntry->second->handle(tokens);
}
