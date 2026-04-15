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

void UserCommand::execute(Client& client, Server& server) {
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
