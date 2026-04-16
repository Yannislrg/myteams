/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** subscribe
*/

#include "subscribe.hpp"
#include "client/client.hpp"
#include "logging_server.h"
#include "server.hpp"

void Subscribe::execute(Client& client, Server& server) {
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const auto& args = client.getArgs();
  if (args.size() < 2 || args[1].empty()) {
    if (args.size() < 2 || args[1].empty()) {
      Server::sendToClient("400 BAD_REQUEST\r\n", client);
    }
    return;
  }
  const auto& teamUuid = args[1];
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    return;
  }
  if (team->isUserSubscribed(client.getUserUuid())) {
    return;
  }
  if (!team->addSubscriber(client.getUserUuid())) {
    return;
  }
  server_event_user_subscribed(teamUuid.c_str(), client.getUserUuid().c_str());
  server.notifySubscribers(teamUuid, "user_subscribed \"" + teamUuid + "\" \"" +
                                         client.getUserUuid() + "\"\r\n");
  Server::sendToClient(
      "200 SUBSCRIBE \"" + client.getUserUuid() + "\" \"" + teamUuid + "\"\r\n",
      client);
}
