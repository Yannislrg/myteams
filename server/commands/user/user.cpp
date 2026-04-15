/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** user
*/

#include "user.hpp"
#include "client.hpp"
#include "database/Database.hpp"
#include "models/User.hpp"
#include "server.hpp"

void UserCommand::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (args.empty()) {
    Server::sendToClient("530 Please login first\r\n", client);
    return;
  }
  const auto& user = server.getDb().findUser(args[1]);
  if (user == nullptr) {
    Server::sendToClient("404 User not found\r\n", client);
    return;
  }
  Server::sendToClient("210 " + user->getUuid() + " " + user->getName() + " " +
                           std::to_string(user->isConnected() ? 1 : 0) + "\r\n",
                       client);
}
