/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response201Handler
*/

#pragma once

#include "ICommandHandler.hpp"

class Response201Handler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override { return "201"; }
  void handle(const std::vector<std::string>& tokens) const override;
};
