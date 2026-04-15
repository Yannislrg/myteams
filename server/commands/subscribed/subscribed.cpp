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
std::string quoteProtocolField(const std::string& value) {
  std::string escaped;
  escaped.reserve(value.size());
  for (const char character : value) {
    if (character == '\\' || character == '"') {
      escaped += '\\';
    }
    escaped += character;
  }
  return "\"" + escaped + "\"";
}

void listAllTeams(Client& client, Server& server) {
  auto& teams = server.getDb().getTeams();
  Server::sendToClient("210-BEGIN TEAMS\r\n", client);
  for (const auto& team : teams) {
    if (team.isUserSubscribed(client.getUserUuid())) {
      Server::sendToClient("210 " + quoteProtocolField(team.getUuid()) + " " +
                               quoteProtocolField(team.getName()) + " " +
                               quoteProtocolField(team.getDescription()) +
                               "\r\n",
                           client);
    }
  }
  Server::sendToClient("210-END TEAMS\r\n", client);
}

void listSubscribedUsers(Client& client, Server& server,
                         const std::string& teamUuid) {
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient("404 NOT_FOUND " + teamUuid + "\r\n", client);
    return;
  }
  Server::sendToClient("210-BEGIN USERS\r\n", client);
  for (const auto& subscriberUuid : team->getSubscriberUuids()) {
    auto* user = server.getDb().findUser(subscriberUuid);
    if (user == nullptr) {
      continue;
    }
    Server::sendToClient("210 \"" + user->getUuid() + "\" \"" +
                             user->getName() + "\" " +
                             (user->isConnected() ? "1" : "0") + "\r\n",
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
    listSubscribedUsers(client, server, teamUuid);
    return;
  }
  Server::sendToClient("400: Too many arguments\r\n", client);
}
