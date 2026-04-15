/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** subscribed
*/

#include "subscribed.hpp"
#include "client/client.hpp"
#include "models/Team.hpp"
#include "server/server.hpp"

namespace {
void listAllTeams(Client& client, Server& server) {
  auto& teams = server.getDb().getTeams();
  for (const auto& team : teams) {
    if (team.isUserSubscribed(client.getUserUuid())) {
      Server::sendToClient(
          "200: " + team.getUuid() + " " + team.getName() + "\r\n", client);
    }
  }
}
void listSubscribedUsers(Client& client, Server& server,
                         const std::string& teamUuid) {
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient("404: Team not found\r\n", client);
    return;
  }
  if (!team->isUserSubscribed(client.getUserUuid())) {
    Server::sendToClient("403: User not subscribed to team\r\n", client);
    return;
  }
  for (const auto& subscriberUuid : team->getSubscriberUuids()) {
    Server::sendToClient("200: " + subscriberUuid + "\r\n", client);
  }
}
}  // namespace

void Subscribed::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (args.size() < 2) {
    listAllTeams(client, server);
    return;
  }
  if (args.size() == 2) {
    const auto& teamUuid = args[1];
    listSubscribedUsers(client, server, teamUuid);
    return;
  }
  // handle error
}
