/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** unsubscribe
*/

#include "unsubscribe.hpp"
#include <algorithm>
#include "client/client.hpp"
#include "logging_server.h"
#include "server.hpp"

void Unsubscribe::execute(Client& client, Server& server) {
  const auto& context = client.getContext();
  if (context.teamUuid.empty()) {
    return;
  }
  auto* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  auto& subscriberUuids = team->getSubscriberUuids();
  const auto eraseFrom =
      std::ranges::remove(subscriberUuids, client.getUserUuid());
  subscriberUuids.erase(eraseFrom.begin(), eraseFrom.end());
  server_event_user_unsubscribed(context.teamUuid.c_str(),
                                 client.getUserUuid().c_str());
  server.notifySubscribers(context.teamUuid,
                           "user_unsubscribed \"" + context.teamUuid + "\" \"" +
                               client.getUserUuid() + "\"\r\n");
  Server::sendToClient("200: " + context.teamUuid + " " + client.getUserUuid(),
                       client);
}
