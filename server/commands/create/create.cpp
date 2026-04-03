/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** create
*/

#include "create.hpp"
#include <uuid/uuid.h>
#include <array>
#include <cstddef>
#include <string>
#include <vector>
#include "../../../libs/logging_server.h"
#include "../../client/client.hpp"
#include "../../models/Channel.hpp"
#include "../../models/Team.hpp"
#include "../../server.hpp"

namespace {
std::vector<std::string> parseArgs(const std::string& input) {
  std::vector<std::string> args;
  std::size_t index = 0;

  while (index < input.size()) {
    if (input[index] == '"') {
      ++index;
      std::string token;
      while (index < input.size() && input[index] != '"') {
        token += input[index++];
      }
      if (index < input.size()) {
        ++index;
      }
      args.push_back(token);
    } else {
      ++index;
    }
  }
  return args;
}
}  // namespace

Create::~Create() = default;

void Create::execute(Client& client, Server& server) {
  const auto& context = client.getContext();

  if (context.teamUuid.empty()) {
    executeTeam(client, server);
  } else if (context.channelUuid.empty()) {
    executeChannel(client, server);
  } else if (context.threadUuid.empty()) {
    executeThread(client, server);
  } else {
    executeReply(client, server);
  }
}

void Create::executeTeam(Client& client, Server& server) {
  (void)client;
  (void)server;
  // TODO(yannis): Implement team creation based on context
}

void Create::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  const auto args = parseArgs(client.getReadBuffer());
  if (args.size() < 2) {
    return;
  }
  const std::string& channelName = args[0];
  const std::string& channelDescription = args[1];

  auto& channels = team->getChannels();
  channels.emplace_back();
  Channel& channel = channels.back();
  constexpr std::size_t uuidStrLen = 37;
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, uuidStrLen> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  channel.setUuid(std::string(uuidStr.data()));
  channel.setName(channelName);
  channel.setDescription(channelDescription);
  server_event_channel_created(context.teamUuid.c_str(),
                               channel.getUuid().c_str(),
                               channel.getName().c_str());
  server.notifySubscribers(context.teamUuid,
                           "channel_created \"" + context.teamUuid + "\" \"" +
                               channel.getUuid() + "\" \"" + channel.getName() +
                               "\" \"" + channel.getDescription() + "\"");
}

void Create::executeThread(Client& client, Server& server) {
  (void)client;
  (void)server;
  // TODO(yannis): Implement thread creation based on context
}

void Create::executeReply(Client& client, Server& server) {
  (void)client;
  (void)server;
  // TODO(yannis): Implement reply creation based on context
}
