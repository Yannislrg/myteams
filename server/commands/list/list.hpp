/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** list
*/

#pragma once

#include "../../client/client.hpp"
#include "../../server.hpp"
#include "../ICommand.hpp"

class List : public ICommand {
 public:
  List() = default;
  ~List() override = default;
  List(const List& other) = delete;
  List& operator=(const List& other) = delete;
  List(List&& other) = delete;
  List& operator=(List&& other) = delete;

  void execute(Client& client, Server& server) override;

 private:
  static void executeTeam(Client& client, Server& server);
  static void executeChannel(Client& client, Server& server);
  static void executeThread(Client& client, Server& server);
  static void executeReply(Client& client, Server& server);

 protected:
 private:
};
