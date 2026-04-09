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
    server.sendToClient("200 OK\r\n", client);
    return true;
  }

  const auto& teamUuid = args[1];
  if (server.getDb().findTeam(teamUuid) == nullptr) {
    server.sendToClient("404 NOT_FOUND \"" + teamUuid + "\"\r\n", client);
    return false;
  }

  ctx.teamUuid = teamUuid;
  ctx.channelUuid.clear();
  ctx.threadUuid.clear();
  server.sendToClient("200 OK\r\n", client);
  return true;
}

bool validateAndSetChannel(const std::vector<std::string>& args, Context& ctx,
                           Server& server, Client& client) {
  if (args.size() < 3) {
    return true;
  }

  const auto& channelUuid = args[2];
  if (server.getDb().findChannel(ctx.teamUuid, channelUuid) == nullptr) {
    server.sendToClient("404 NOT_FOUND \"" + channelUuid + "\"\r\n", client);
    return false;
  }

  ctx.channelUuid = channelUuid;
  ctx.threadUuid.clear();
  server.sendToClient("200 OK\r\n", client);
  return true;
}

bool validateAndSetThread(const std::vector<std::string>& args, Context& ctx,
                          Server& server, Client& client) {
  if (args.size() < 4) {
    return true;
  }

  const auto& threadUuid = args[3];
  if (server.getDb().findThread(ctx.channelUuid, threadUuid) == nullptr) {
    server.sendToClient("404 NOT_FOUND \"" + threadUuid + "\"\r\n", client);
    return false;
  }

  ctx.threadUuid = threadUuid;
  server.sendToClient("200 OK\r\n", client);
  return true;
}

}  // namespace

void Use::execute(Client& client, Server& server) {
  const auto args = client.getArgs();
  auto& ctx = client.getContext();

  if (!validateAndSetTeam(args, ctx, server, client)) {
    return;
  }
  if (!validateAndSetChannel(args, ctx, server, client)) {
    return;
  }
  if (!validateAndSetThread(args, ctx, server, client)) {
    return;
  }
}
