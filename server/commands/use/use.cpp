/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** use
*/

#include "use.hpp"
#include <cstddef>
#include "client.hpp"
#include "server.hpp"

namespace {

bool validateAndSetTeam(const std::vector<std::string>& args, Context& ctx,
                        Server& server, Client& client) {
  if (args.size() < 2) {
    ctx.teamUuid.clear();
    ctx.channelUuid.clear();
    ctx.threadUuid.clear();
    Server::sendToClient("200 OK\r\n", client);
    return true;
  }

  const auto& teamUuid = args[1];
  auto* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND TEAM " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return false;
  }
  if (!team->isUserSubscribed(client.getUserUuid())) {
    Server::sendToClient("403 FORBIDDEN\r\n", client);
    return false;
  }

  ctx.teamUuid = teamUuid;
  ctx.channelUuid.clear();
  ctx.threadUuid.clear();
  Server::sendToClient("200 OK\r\n", client);
  return true;
}

bool validateAndSetChannel(const std::vector<std::string>& args, Context& ctx,
                           Server& server, Client& client) {
  if (args.size() < 3) {
    return true;
  }

  const auto& channelUuid = args[2];
  if (server.getDb().findChannel(ctx.teamUuid, channelUuid) == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND CHANNEL " + Utils::quoteProtocolField(channelUuid) +
            "\r\n",
        client);
    return false;
  }

  ctx.channelUuid = channelUuid;
  ctx.threadUuid.clear();
  Server::sendToClient("200 OK\r\n", client);
  return true;
}

bool validateAndSetThread(const std::vector<std::string>& args, Context& ctx,
                          Server& server, Client& client) {
  if (args.size() < 4) {
    return true;
  }

  const auto& threadUuid = args[3];
  if (server.getDb().findThread(ctx.channelUuid, threadUuid) == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND THREAD " + Utils::quoteProtocolField(threadUuid) +
            "\r\n",
        client);
    return false;
  }

  ctx.threadUuid = threadUuid;
  Server::sendToClient("200 OK\r\n", client);
  return true;
}

}  // namespace

void Use::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  Context newContext;

  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }

  if (args.size() > 4) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }

  if (!validateAndSetTeam(args, newContext, server, client)) {
    return;
  }
  if (!validateAndSetChannel(args, newContext, server, client)) {
    return;
  }
  if (!validateAndSetThread(args, newContext, server, client)) {
    return;
  }
  client.setContext(newContext);
}
