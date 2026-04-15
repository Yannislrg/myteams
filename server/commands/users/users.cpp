/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** users
*/

#include "users.hpp"
#include "client.hpp"
#include "database/Database.hpp"
#include "server.hpp"

void UsersCommand::execute(Client& client, Server& server) {
  if (client.getUserUuid().empty()) {
    Server::sendToClient("530 Please login first\r\n", client);
    return;
  }
  const auto& users = server.getDb().getUsers();
  Server::sendToClient("210-BEGIN USERS\r\n", client);
  for (const auto& user : users) {
    Server::sendToClient("210 " + user.getUuid() + " " + user.getName() + " " +
                             std::to_string(user.isConnected() ? 1 : 0) +
                             "\r\n",
                         client);
  }
  Server::sendToClient("210-END USERS\r\n", client);
}
