/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** logout
*/

#include "logout.hpp"
#include "client.hpp"
#include "database/Database.hpp"
#include "logging_server.h"
#include "models/User.hpp"
#include "server.hpp"

void Logout::execute(Client& client, Server& server) {
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const auto userUuid = client.getUserUuid();
  auto* user = server.getDb().findUser(client.getUserUuid());
  if (user != nullptr) {
    user->decrementConnection();
    Server::sendToClient("EVENT USER_LOGGED_OUT \"" + user->getUuid() +
                             "\" \"" + user->getName() + "\"\r\n",
                         client);
    if (!user->isConnected()) {
      server_event_user_logged_out(userUuid.c_str());
      for (const auto& [fd, otherClient] : server.getClients()) {
        if (otherClient.get() != &client) {
          Server::sendToClient("EVENT USER_LOGGED_OUT \"" + user->getUuid() +
                                   "\" \"" + user->getName() + "\"\r\n",
                               *otherClient);
        }
      }
    }
  }
  client.setUserUuid("");
  Server::sendToClient("200 OK\r\n", client);
}
