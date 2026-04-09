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
  const auto& context = client.getContext();
  if (context.teamUuid.empty()) {
    return;
  }
  auto* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  team->getSubscriberUuids().push_back(client.getUserUuid());
  server_event_user_subscribed(context.teamUuid.c_str(),
                               client.getUserUuid().c_str());
  server.notifySubscribers(context.teamUuid,
                           "user_subscribed \"" + context.teamUuid + "\" \"" +
                               client.getUserUuid() + "\"\r\n");
  server.sendToClient("200: " + context.teamUuid + " " + client.getUserUuid(),
                      client);
}
