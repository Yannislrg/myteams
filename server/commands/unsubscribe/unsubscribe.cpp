/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** unsubscribe
*/

#include "unsubscribe.hpp"
#include "client/client.hpp"
#include "logging_server.h"
#include "server.hpp"

void Unsubscribe::execute(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto userUuid = client.getUserUuid();
  if (userUuid.empty()) {
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
  if (userUuid.empty()) {
    return;
  }
  auto* team = server.getDb().findTeam(args[1]);
  if (team == nullptr) {
    return;
  }
  if (!team->isUserSubscribed(userUuid)) {
    return;
  }
  if (!team->removeSubscriber(userUuid)) {
    return;
  }
  server_event_user_unsubscribed(context.teamUuid.c_str(), userUuid.c_str());
  server.notifySubscribers(context.teamUuid, "user_unsubscribed \"" +
                                                 context.teamUuid + "\" \"" +
                                                 userUuid + "\"\r\n");
  Server::sendToClient(
      "200 UNSUBSCRIBE \"" + userUuid + "\" \"" + context.teamUuid + "\"\r\n", client);
}
