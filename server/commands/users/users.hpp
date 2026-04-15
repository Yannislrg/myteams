/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** users
*/

#pragma once

#include "ICommand.hpp"

class UserCommand : public ICommand {
 public:
  UserCommand();
  ~UserCommand() override;
  UserCommand(const UserCommand& other) = delete;
  UserCommand& operator=(const UserCommand& other) = delete;
  UserCommand(UserCommand&& other) = delete;
  UserCommand& operator=(UserCommand&& other) = delete;

  void execute(Client& client, Server& server) override;

 private:
};
