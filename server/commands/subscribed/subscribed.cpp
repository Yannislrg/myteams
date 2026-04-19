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
#include "utils/utils.hpp"

namespace {

void listAllTeams(Client& client, Server& server) {
  auto* user = server.getDb().findUser(client.getUserUuid());
  if (user == nullptr) {
    Server::sendToClient("404 NOT_FOUND USER " +
                             Utils::quoteProtocolField(client.getUserUuid()) +
                             "\r\n",
                         client);
    return;
  }
  auto& teams = server.getDb().getTeams();
  Server::sendToClient("210-BEGIN TEAMS\r\n", client);
  for (const auto& teamUuid : user->getTeams()) {
    for (const auto& team : teams) {
      if (team.getUuid() != teamUuid) {
        continue;
      }
      Server::sendToClient(
          "210 " + Utils::quoteProtocolField(team.getUuid()) + " " +
              Utils::quoteProtocolField(team.getName()) + " " +
              Utils::quoteProtocolField(team.getDescription()) + "\r\n",
          client);
      break;
    }
  }
  Server::sendToClient("210-END TEAMS\r\n", client);
}

void listSubscribedUsers(Client& client, Server& server,
                         const std::string& teamUuid) {
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND TEAM " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return;
  }
  Server::sendToClient("210-BEGIN USERS\r\n", client);
  for (const auto& subscriberUuid : team->getSubscriberUuids()) {
    auto* user = server.getDb().findUser(subscriberUuid);
    if (user == nullptr) {
      continue;
    }
    Server::sendToClient("210 " + Utils::quoteProtocolField(user->getUuid()) +
                             " " + Utils::quoteProtocolField(user->getName()) +
                             " " + (user->isConnected() ? "1" : "0") + "\r\n",
                         client);
  }
  Server::sendToClient("210-END USERS\r\n", client);
}
}  // namespace

void Subscribed::execute(Client& client, Server& server) {
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const auto& args = client.getArgs();
  if (args.size() < 2) {
    listAllTeams(client, server);
    return;
  }
  if (args.size() == 2) {
    const auto& teamUuid = args[1];
    if (teamUuid.empty()) {
      Server::sendToClient("400 BAD_REQUEST\r\n", client);
      return;
    }
    listSubscribedUsers(client, server, teamUuid);
    return;
  }
  Server::sendToClient("400 BAD_REQUEST\r\n", client);
}
