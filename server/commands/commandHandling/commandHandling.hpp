/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#pragma once

#include <map>
#include <string>
#include "../../client/client.hpp"
#include "../ICommand.hpp"

class Server;

class CommandHandling {
 public:
  CommandHandling();
  ~CommandHandling();
  CommandHandling(const CommandHandling& other) = delete;
  CommandHandling& operator=(const CommandHandling& other) = delete;
  CommandHandling(CommandHandling&& other) = delete;
  CommandHandling& operator=(CommandHandling&& other) = delete;

  void handleCommand(const std::string& raw, Client& client, Server& server);

 protected:
 private:
  std::map<std::string, ICommand*> _commands;
};
