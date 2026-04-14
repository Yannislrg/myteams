/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** PrivateMessageHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class PrivateMessageHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "PRIVATE_MESSAGE";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
