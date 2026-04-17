/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ResponseErrorHandler
*/

#pragma once

#include <string>
#include <utility>
#include "ICommandHandler.hpp"

class ResponseErrorHandler : public ICommandHandler {
 public:
  ResponseErrorHandler(std::string code, const std::string& pendingCommand)
      : _code(std::move(code)), _pendingCommand(pendingCommand) {}

  [[nodiscard]] std::string getCommand() const override { return _code; }
  void handle(const std::vector<std::string>& tokens) const override;

 private:
  std::string _code;
  const std::string& _pendingCommand;
};
