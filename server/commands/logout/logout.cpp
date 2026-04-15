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
  auto* user = server.getDb().findUser(client.getUserUuid());
  if (user != nullptr) {
    user->setConnected(false);
  }
  client.setUserUuid("");
  Server::sendToClient("200 OK\r\n", client);
  server_event_user_logged_out(client.getUserUuid().c_str());
}
