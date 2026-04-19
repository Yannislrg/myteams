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
#include "utils.hpp"

void Unsubscribe::execute(Client& client, Server& server) {
  const auto userUuid = client.getUserUuid();
  if (userUuid.empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const auto& args = client.getArgs();
  if (args.size() < 2 || args[1].empty()) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const auto& teamUuid = args[1];
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND TEAM " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return;
  }
  if (!team->isUserSubscribed(userUuid)) {
    Server::sendToClient("403 FORBIDDEN\r\n", client);
    return;
  }
  if (!team->removeSubscriber(userUuid)) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  if (auto* user = server.getDb().findUser(userUuid); user != nullptr) {
    (void)user->removeTeam(teamUuid);
  }
  server_event_user_unsubscribed(teamUuid.c_str(), userUuid.c_str());
  server.notifySubscribers(teamUuid, "user_unsubscribed \"" + teamUuid +
                                         "\" \"" + userUuid + "\"\r\n");
  Server::sendToClient("200 UNSUBSCRIBE " +
                           Utils::quoteProtocolField(userUuid) + " " +
                           Utils::quoteProtocolField(teamUuid) + "\r\n",
                       client);
}
