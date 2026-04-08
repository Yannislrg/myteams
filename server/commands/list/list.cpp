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
  auto context = client.getContext();
  auto threads = server.getDb().getAllThreads();
  for (const auto& thread : threads) {
    if (thread.getUuid() != context.threadUuid) {
      continue;
    }
    for (const auto& reply : thread.getReplies()) {
      server.sendToClient("200 : " + reply.getBody() + "\r\n", client);
    }
    break;
  }
}

void List::executeThread(Client& client, Server& server) {
  auto context = client.getContext();
  auto* channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  if (channel == nullptr) {
    return;
  }
  for (const auto& thread : channel->getThreads()) {
    server.sendToClient("200 : " + thread.getTitle() + "\r\n", client);
  }
}

void List::executeChannel(Client& client, Server& server) {
  auto context = client.getContext();
  auto* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  for (const auto& channel : team->getChannels()) {
    server.sendToClient("200 : " + channel.getName() + "\r\n", client);
  }
}

void List::executeTeam(Client& client, Server& server) {
  for (const auto& team : server.getDb().getTeams()) {
    server.sendToClient("200 : " + team.getName() + "\r\n", client);
  }
}

void List::execute(Client& client, Server& server) {
  auto context = client.getContext();
  if (!context.channelUuid.empty() && !context.threadUuid.empty() &&
      !context.teamUuid.empty()) {
    executeReply(client, server);
  } else if (!context.channelUuid.empty() && !context.teamUuid.empty()) {
    executeThread(client, server);
  } else if (!context.teamUuid.empty()) {
    executeChannel(client, server);
  } else {
    executeTeam(client, server);
  }
}
