/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerMessageRouter
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "ICommandHandler.hpp"
#include "User.hpp"

class ServerMessageRouter {
 public:
  ServerMessageRouter();
  void routeFrame(const std::string& frame);
  void setPendingCommand(std::string cmd);
  [[nodiscard]] bool hasLoggedUser() const { return _user.isLoggedIn(); }
  [[nodiscard]] const std::string& getLoggedUserUuid() const {
    return _user.getUuid();
  }
  [[nodiscard]] const std::string& getLoggedUserName() const {
    return _user.getName();
  }
  void clearLoggedUser() { _user.logout(); }
  [[nodiscard]] bool shouldDisconnect() const { return _shouldDisconnect; }

 private:
  void registerHandler(std::unique_ptr<ICommandHandler> handler);
  void registerCommandHandlers();
  void registerListHandlers();

  using ListHandler = std::function<void(const std::vector<std::string>&)>;

  User _user;
  bool _shouldDisconnect = false;
  std::string _pendingCommand;
  std::unordered_map<std::string, std::unique_ptr<ICommandHandler>> _handlers;
  std::unordered_map<std::string, ListHandler> _listHandlers;
  std::string _listContext;
};
