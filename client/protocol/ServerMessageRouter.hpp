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
  [[nodiscard]] bool shouldDisconnect() const { return _shouldDisconnect; }

 private:
  void registerHandler(std::unique_ptr<ICommandHandler> handler);
  void registerCommandHandlers();
  void registerListHandlers();

  using ListHandler = std::function<void(const std::vector<std::string>&)>;

  User _user;
  bool _shouldDisconnect = false;
  std::unordered_map<std::string, std::unique_ptr<ICommandHandler>> _handlers;
  std::unordered_map<std::string, ListHandler> _listHandlers;
  std::string _listContext;
};
