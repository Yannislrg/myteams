/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** info
*/

#pragma once

#include "../../client/client.hpp"
#include "../../server.hpp"
#include "../ICommand.hpp"

class Info : public ICommand {
 public:
  Info() = default;
  ~Info() override = default;
  Info(const Info& other) = delete;
  Info& operator=(const Info& other) = delete;
  Info(Info&& other) = delete;
  Info& operator=(Info&& other) = delete;
  void execute(Client& client, Server& server) override;

 private:
  static void executeTeam(Client& client, Server& server);
  static void executeChannel(Client& client, Server& server);
  static void executeThread(Client& client, Server& server);
  static void executeUser(Client& client, Server& server);
};
