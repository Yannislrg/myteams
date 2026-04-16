/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response200Handler
*/

#pragma once

#include "ICommandHandler.hpp"
#include "User.hpp"

class Response200Handler : public ICommandHandler {
 public:
  Response200Handler(User& user, bool& shouldDisconnect,
                     const std::string& pendingCommand);
  [[nodiscard]] std::string getCommand() const override { return "200"; }
  void handle(const std::vector<std::string>& tokens) const override;

 private:
  User& _user;
  bool& _shouldDisconnect;
  const std::string& _pendingCommand;
};
