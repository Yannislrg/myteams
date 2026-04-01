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
  ICommand() = default;
  virtual ~ICommand() = default;
  ICommand(const ICommand& other) = delete;
  ICommand& operator=(const ICommand& other) = delete;
  ICommand(ICommand&& other) = delete;
  ICommand& operator=(ICommand&& other) = delete;

  virtual void execute(Client& client, Server& server) = 0;

 protected:
 private:
};
