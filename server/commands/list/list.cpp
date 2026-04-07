/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** list
*/

#include "list.hpp"
#include <cstddef>
#include "../../Context.hpp"
#include "../../client/client.hpp"
#include "../../server.hpp"

void List::executeReply(Client& client, Server& server) {
  auto threads = server.getDb().getAllThreads();
  for (const auto& thread : threads) {
    for (const auto& reply : thread.getReplies()) {
      server.sendToClient("200 : " + reply.getBody() + "\r\n", client);
    }
  }
}

void List::executeThread(Client& client, Server& server) {
  auto threads = server.getDb().getAllThreads();
  for (const auto& thread : threads) {
    server.sendToClient("200 : " + thread.getTitle() + "\r\n", client);
  }
}

void List::executeChannel(Client& client, Server& server) {
  auto channel = server.getDb().getAllChannels();
  for (const auto& current : channel) {
    server.sendToClient("200 : " + current.getName() + "\r\n", client);
  }
}

void List::executeTeam(Client& client, Server& server) {
  auto teams = server.getDb().getTeams();
  for (const auto& team : teams) {
    server.sendToClient("200 : " + team.getName() + "\r\n", client);
  }
}

void List::execute(Client& client, Server& server) {
  auto context = client.getContext();
  if (!context.channelUuid.empty() && !context.threadUuid.empty() &&
      context.teamUuid.empty()) {
    executeReply(client, server);
  } else if (!context.channelUuid.empty() && !context.teamUuid.empty()) {
    executeThread(client, server);
  } else if (!context.teamUuid.empty()) {
    executeChannel(client, server);
  } else {
    executeTeam(client, server);
  }
}
