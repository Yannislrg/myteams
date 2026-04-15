/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerMessageRouter
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "ICommandHandler.hpp"

class ServerMessageRouter {
 public:
  ServerMessageRouter();
  void routeFrame(const std::string& frame);

 private:
  void registerHandler(std::unique_ptr<ICommandHandler> handler);
  void handleListItem(const std::vector<std::string>& tokens) const;

  std::unordered_map<std::string, std::unique_ptr<ICommandHandler>> _handlers;
  std::string _listContext;
};
