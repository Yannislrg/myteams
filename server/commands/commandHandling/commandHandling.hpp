/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#pragma once

#include <map>
#include <memory>
#include <string>
#include "../../client/client.hpp"
#include "../ICommand.hpp"

class Server;

class CommandHandling {
 public:
  CommandHandling() = default;
  ~CommandHandling() = default;
  CommandHandling(const CommandHandling& other) = delete;
  CommandHandling& operator=(const CommandHandling& other) = delete;
  CommandHandling(CommandHandling&& other) = delete;
  CommandHandling& operator=(CommandHandling&& other) = delete;

  void handleCommand(const std::string& rawCommand, Client& client,
                     Server& server);

 protected:
 private:
  std::map<std::string, std::unique_ptr<ICommand>> _commands;
};
