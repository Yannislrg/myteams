/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** UserLoggedInHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class UserLoggedInHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "USER_LOGGED_IN";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
