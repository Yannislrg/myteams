/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TeamCreatedHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class TeamCreatedHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "TEAM_CREATED";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
