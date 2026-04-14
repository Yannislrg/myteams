/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ChannelCreatedHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class ChannelCreatedHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "CHANNEL_CREATED";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
