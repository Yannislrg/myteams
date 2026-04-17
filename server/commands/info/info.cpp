/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** info
*/

#include "info.hpp"
#include <string>
#include "../../client/client.hpp"
#include "../../database/Database.hpp"
#include "../../server.hpp"
#include "../../utils/utils.hpp"

void Info::executeTeam(Client& client, Server& server) {
  const auto& teamUuid = client.getContext().teamUuid;
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND TEAM " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return;
  }
  Server::sendToClient(
      "200 INFO TEAM " + Utils::quoteProtocolField(team->getUuid()) + " " +
          Utils::quoteProtocolField(team->getName()) + " " +
          Utils::quoteProtocolField(team->getDescription()) + "\r\n",
      client);
}

void Info::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& channelUuid = context.channelUuid;
  auto* channel = server.getDb().findChannel(context.teamUuid, channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND CHANNEL " +
                             Utils::quoteProtocolField(channelUuid) + "\r\n",
                         client);
    return;
  }
  Server::sendToClient(
      "200 INFO CHANNEL " + Utils::quoteProtocolField(channel->getUuid()) +
          " " + Utils::quoteProtocolField(channel->getName()) + " " +
          Utils::quoteProtocolField(channel->getDescription()) + "\r\n",
      client);
}

void Info::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& threadUuid = context.threadUuid;
  auto* thread = server.getDb().findThread(context.channelUuid, threadUuid);
  if (thread == nullptr) {
    Server::sendToClient("404 NOT_FOUND THREAD " +
                             Utils::quoteProtocolField(threadUuid) + "\r\n",
                         client);
    return;
  }
  Server::sendToClient(
      "200 INFO THREAD " + Utils::quoteProtocolField(thread->getUuid()) + " " +
          Utils::quoteProtocolField(thread->getUserUuid()) + " " +
          std::to_string(thread->getTimestamp()) + " " +
          Utils::quoteProtocolField(thread->getTitle()) + " " +
          Utils::quoteProtocolField(thread->getBody()) + "\r\n",
      client);
}

void Info::executeUser(Client& client, Server& server) {
  const auto& userUuid = client.getUserUuid();
  if (userUuid.empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }

  auto* user = server.getDb().findUser(userUuid);
  if (user == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND USER " + Utils::quoteProtocolField(userUuid) + "\r\n",
        client);
    return;
  }
  Server::sendToClient("200 INFO USER " +
                           Utils::quoteProtocolField(user->getUuid()) + " " +
                           Utils::quoteProtocolField(user->getName()) + " " +
                           std::to_string(user->isConnected() ? 1 : 0) + "\r\n",
                       client);
}

void Info::execute(Client& client, Server& server) {
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
