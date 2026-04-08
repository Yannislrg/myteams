/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** info
*/

#include "info.hpp"
#include "../../client/client.hpp"
#include "../../database/Database.hpp"
#include "../../server.hpp"

void Info::executeTeam(Client& client, Server& server) {
  auto* team = server.getDb().findTeam(client.getContext().teamUuid);
  server.sendToClient("200 : " + team->getName() + "\r\n", client);
}

void Info::executeChannel(Client& client, Server& server) {
  auto* channel = server.getDb().findChannel(client.getContext().teamUuid,
                                             client.getContext().channelUuid);
  server.sendToClient("200 : " + channel->getName() + "\r\n", client);
}

void Info::executeThread(Client& client, Server& server) {
  auto* thread = server.getDb().findThread(client.getContext().channelUuid,
                                           client.getContext().threadUuid);

  server.sendToClient("200 : " + thread->getTitle() + "\r\n", client);
}

void Info::executeUser(Client& client, Server& server) {
  auto* user = server.getDb().findUser(client.getUserUuid());
  if (user == nullptr) {
    return;
  }
  server.sendToClient("200 : " + user->getName() + "\r\n", client);
}

void Info::execute(Client& client, Server& server) {
  const auto& context = client.getContext();

  if (!context.threadUuid.empty()) {
    executeThread(client, server);
  } else if (!context.channelUuid.empty()) {
    executeChannel(client, server);
  } else if (!context.teamUuid.empty()) {
    executeTeam(client, server);
  } else {
    executeUser(client, server);
  }
}
