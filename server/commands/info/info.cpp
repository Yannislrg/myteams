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

void Info::executeTeam(Client& client, Server& server) {
  const auto& teamUuid = client.getContext().teamUuid;
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient("404 NOT_FOUND \"" + teamUuid + "\"\r\n", client);
    return;
  }
  Server::sendToClient("200 INFO TEAM \"" + team->getUuid() + "\" \"" +
                           team->getName() + "\" \"" + team->getDescription() +
                           "\"\r\n",
                       client);
}

void Info::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& channelUuid = context.channelUuid;
  auto* channel = server.getDb().findChannel(context.teamUuid, channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND \"" + channelUuid + "\"\r\n", client);
    return;
  }
  Server::sendToClient("200 INFO CHANNEL \"" + channel->getUuid() + "\" \"" +
                           channel->getName() + "\" \"" +
                           channel->getDescription() + "\"\r\n",
                       client);
}

void Info::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& threadUuid = context.threadUuid;
  auto* thread = server.getDb().findThread(context.channelUuid, threadUuid);
  if (thread == nullptr) {
    Server::sendToClient("404 NOT_FOUND \"" + threadUuid + "\"\r\n", client);
    return;
  }
  Server::sendToClient("200 INFO THREAD \"" + thread->getUuid() + "\" \"" +
                           thread->getUserUuid() + "\" " +
                           std::to_string(thread->getTimestamp()) + " \"" +
                           thread->getTitle() + "\" \"" + thread->getBody() +
                           "\"\r\n",
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
    Server::sendToClient("404 NOT_FOUND \"" + userUuid + "\"\r\n", client);
    return;
  }
  Server::sendToClient("200 INFO USER \"" + user->getUuid() + "\" \"" +
                           user->getName() + "\" " +
                           std::to_string(user->isConnected() ? 1 : 0) + "\r\n",
                       client);
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
