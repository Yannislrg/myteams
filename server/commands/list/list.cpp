/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** list
*/

#include "list.hpp"
#include <cstddef>
#include "Context.hpp"
#include "client/client.hpp"
#include "server.hpp"
#include "utils.hpp"

void List::executeReply(Client& client, Server& server) {
  const auto& context = client.getContext();
  auto* thread =
      server.getDb().findThread(context.channelUuid, context.threadUuid);
  if (thread == nullptr) {
    Server::sendToClient("404 NOT_FOUND THREAD " +
                             Utils::quoteProtocolField(context.threadUuid) +
                             "\r\n",
                         client);
    return;
  }

  Server::sendToClient("210-BEGIN REPLIES\r\n", client);
  for (const auto& reply : thread->getReplies()) {
    Server::sendToClient(
        "210 " + Utils::quoteProtocolField(thread->getUuid()) + " " +
            Utils::quoteProtocolField(reply.getUserUuid()) + " " +
            std::to_string(reply.getTimestamp()) + " " +
            Utils::quoteProtocolField(reply.getBody()) + "\r\n",
        client);
  }
  Server::sendToClient("210-END REPLIES\r\n", client);
}

void List::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  auto* channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND CHANNEL " +
                             Utils::quoteProtocolField(context.channelUuid) +
                             "\r\n",
                         client);
    return;
  }

  Server::sendToClient("210-BEGIN THREADS\r\n", client);
  for (const auto& thread : channel->getThreads()) {
    Server::sendToClient(
        "210 " + Utils::quoteProtocolField(thread.getUuid()) + " " +
            Utils::quoteProtocolField(thread.getUserUuid()) + " " +
            std::to_string(thread.getTimestamp()) + " " +
            Utils::quoteProtocolField(thread.getTitle()) + " " +
            Utils::quoteProtocolField(thread.getBody()) + "\r\n",
        client);
  }
  Server::sendToClient("210-END THREADS\r\n", client);
}

void List::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  auto* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    Server::sendToClient("404 NOT_FOUND TEAM " +
                             Utils::quoteProtocolField(context.teamUuid) +
                             "\r\n",
                         client);
    return;
  }

  Server::sendToClient("210-BEGIN CHANNELS\r\n", client);
  for (const auto& channel : team->getChannels()) {
    Server::sendToClient(
        "210 " + Utils::quoteProtocolField(channel.getUuid()) + " " +
            Utils::quoteProtocolField(channel.getName()) + " " +
            Utils::quoteProtocolField(channel.getDescription()) + "\r\n",
        client);
  }
  Server::sendToClient("210-END CHANNELS\r\n", client);
}

void List::executeTeam(Client& client, Server& server) {
  Server::sendToClient("210-BEGIN TEAMS\r\n", client);
  for (const auto& team : server.getDb().getTeams()) {
    Server::sendToClient(
        "210 " + Utils::quoteProtocolField(team.getUuid()) + " " +
            Utils::quoteProtocolField(team.getName()) + " " +
            Utils::quoteProtocolField(team.getDescription()) + "\r\n",
        client);
  }
  Server::sendToClient("210-END TEAMS\r\n", client);
}

void List::execute(Client& client, Server& server) {
  const auto& context = client.getContext();
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }

  if (!context.teamUuid.empty()) {
    const auto* team = server.getDb().findTeam(context.teamUuid);
    if (team == nullptr) {
      Server::sendToClient("404 NOT_FOUND TEAM " +
                               Utils::quoteProtocolField(context.teamUuid) +
                               "\r\n",
                           client);
      return;
    }
    if (!team->isUserSubscribed(client.getUserUuid())) {
      Server::sendToClient("403 FORBIDDEN\r\n", client);
      return;
    }
  }

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
