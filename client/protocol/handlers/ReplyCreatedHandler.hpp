/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ReplyCreatedHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class ReplyCreatedHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "REPLY_CREATED";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
