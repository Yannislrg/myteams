/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** UserLoggedOutHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class UserLoggedOutHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "USER_LOGGED_OUT";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
