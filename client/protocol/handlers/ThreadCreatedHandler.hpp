/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ThreadCreatedHandler
*/

#pragma once

#include "ICommandHandler.hpp"

class ThreadCreatedHandler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override {
    return "THREAD_CREATED";
  }
  void handle(const std::vector<std::string>& tokens) const override;
};
