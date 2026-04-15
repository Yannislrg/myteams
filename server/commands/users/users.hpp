/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** users
*/

#pragma once

#include "ICommand.hpp"

class UsersCommand : public ICommand {
 public:
  UsersCommand() = default;
  ~UsersCommand() override = default;
  UsersCommand(const UsersCommand& other) = delete;
  UsersCommand& operator=(const UsersCommand& other) = delete;
  UsersCommand(UsersCommand&& other) = delete;
  UsersCommand& operator=(UsersCommand&& other) = delete;

  void execute(Client& client, Server& server) override;

 private:
};
