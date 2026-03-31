/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ICommand
*/

#pragma once

#include "../client/client.hpp"

class Server;

class ICommand {
 public:
  ICommand();
  ~ICommand() = default;
  ICommand(const ICommand& other) = delete;
  ICommand& operator=(const ICommand& other) = delete;
  ICommand(ICommand&& other) = delete;
  ICommand& operator=(ICommand&& other) = delete;

  void execute(Client& client, Server& server);

 protected:
 private:
};
