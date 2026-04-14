/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ICommandHandler
*/

#pragma once

#include <string>
#include <vector>

class ICommandHandler {
 public:
  ICommandHandler() = default;
  virtual ~ICommandHandler() = default;

  ICommandHandler(const ICommandHandler& other) = delete;
  ICommandHandler& operator=(const ICommandHandler& other) = delete;
  ICommandHandler(ICommandHandler&& other) = delete;
  ICommandHandler& operator=(ICommandHandler&& other) = delete;

  [[nodiscard]] virtual std::string getCommand() const = 0;
  virtual void handle(const std::vector<std::string>& tokens) const = 0;
};
