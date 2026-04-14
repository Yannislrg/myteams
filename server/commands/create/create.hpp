/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** create
*/

#pragma once

#include "client/client.hpp"
#include "commands/ICommand.hpp"
#include "server.hpp"

class Create : public ICommand {
 public:
  Create() = default;
  ~Create() override;
  Create(const Create& other) = delete;
  Create& operator=(const Create& other) = delete;
  Create(Create&& other) = delete;
  Create& operator=(Create&& other) = delete;

  void execute(Client& client, Server& server) override;

 private:
  static void executeTeam(Client& client, Server& server);
  static void executeChannel(Client& client, Server& server);
  static void executeThread(Client& client, Server& server);
  static void executeReply(Client& client, Server& server);

 protected:
 private:
};
