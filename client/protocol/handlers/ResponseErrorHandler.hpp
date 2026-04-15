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
  explicit ResponseErrorHandler(std::string code) : _code(std::move(code)) {}

  [[nodiscard]] std::string getCommand() const override { return _code; }
  void handle(const std::vector<std::string>& tokens) const override;

 private:
  std::string _code;
};
