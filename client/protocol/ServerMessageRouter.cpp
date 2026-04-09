/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerMessageRouter
*/

#include "ServerMessageRouter.hpp"
#include <memory>
#include <string>
#include "TokenParser.hpp"
#include "handlers/ChannelCreatedHandler.hpp"
#include "handlers/PrivateMessageHandler.hpp"
#include "handlers/ReplyCreatedHandler.hpp"
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
}

void ServerMessageRouter::registerHandler(
    std::unique_ptr<ICommandHandler> handler) {
  const std::string command = handler->getCommand();
  _handlers[command] = std::move(handler);
}

void ServerMessageRouter::routeFrame(const std::string& frame) const {
  const auto tokensResult = TokenParser::parse(frame);
  if (!tokensResult.has_value()) {
    return;
  }
  const std::vector<std::string>& tokens = tokensResult.value();
  if (tokens.empty()) {
    return;
  }

  std::string command = tokens[0];
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
