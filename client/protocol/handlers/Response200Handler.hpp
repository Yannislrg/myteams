/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response200Handler
*/

#pragma once

#include "ICommandHandler.hpp"

class Response200Handler : public ICommandHandler {
 public:
  [[nodiscard]] std::string getCommand() const override { return "200"; }
  void handle(const std::vector<std::string>& tokens) const override;
};
