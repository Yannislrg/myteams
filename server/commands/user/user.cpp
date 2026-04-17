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
#include "utils.hpp"

void UserCommand::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  if (args.size() < 2) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const auto& user = server.getDb().findUser(args[1]);
  if (user == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND " + Utils::quoteProtocolField(args[1]) + "\r\n",
        client);
    return;
  }
  Server::sendToClient("200 USER " +
                           Utils::quoteProtocolField(user->getUuid()) + " " +
                           Utils::quoteProtocolField(user->getName()) + " " +
                           std::to_string(user->isConnected() ? 1 : 0) + "\r\n",
                       client);
}
