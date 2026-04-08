/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** create
*/

#include "commands/create/create.hpp"
#include <uuid/uuid.h>
#include <array>
#include <chrono>
#include <cstddef>
#include <string>
#include <vector>
#include "client/client.hpp"
#include "logging_server.h"
#include "models/Channel.hpp"
#include "models/Reply.hpp"
#include "models/Team.hpp"
#include "models/Thread.hpp"
#include "server.hpp"

#ifndef UUID_STR_LEN
#define UUID_STR_LEN 37
#endif

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
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    return;
  }
  const std::string& teamName = args[1];
  const std::string& teamDescription = args[2];
  auto& teams = server.getDb().getTeams();
  Team newTeam;
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_STR_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newTeam.setUuid(std::string(uuidStr.data()));
  newTeam.setName(teamName);
  newTeam.setDescription(teamDescription);
  teams.push_back(newTeam);
  server_event_team_created(newTeam.getUuid().c_str(),
                            newTeam.getName().c_str(),
                            client.getUserUuid().c_str());
  server.notifySubscribers(newTeam.getUuid(),
                           "team_created \"" + newTeam.getUuid() + "\" \"" +
                               newTeam.getName() + "\" \"" +
                               newTeam.getDescription() + "\"\r\n");
}

void Create::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    return;
  }
  const std::string& channelName = args[1];
  const std::string& channelDescription = args[2];

  auto& channels = team->getChannels();
  channels.emplace_back();
  Channel& channel = channels.back();
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_STR_LEN> uuidStr{};
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
                               "\" \"" + channel.getDescription() + "\"\r\n");
}

void Create::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& team = server.getDb().findTeam(context.teamUuid);
  if (team == nullptr) {
    return;
  }
  const auto& channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  if (channel == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    return;
  }
  const std::string& threadTitle = args[1];
  const std::string& threadMessage = args[2];
  auto& threads = channel->getThreads();
  Thread newThread;
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_STR_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newThread.setUuid(std::string(uuidStr.data()));
  newThread.setTitle(threadTitle);
  newThread.setBody(threadMessage);
  newThread.setTimestamp(
      std::chrono::system_clock::now().time_since_epoch().count());
  threads.push_back(newThread);
  server_event_thread_created(
      context.channelUuid.c_str(), newThread.getUuid().c_str(),
      client.getUserUuid().c_str(), newThread.getTitle().c_str(),
      newThread.getBody().c_str());
  server.notifySubscribers(
      context.teamUuid,
      "thread_created \"" + context.teamUuid + "\" \"" + context.channelUuid +
          "\" \"" + newThread.getUuid() + "\" \"" + newThread.getTitle() +
          "\" \"" + newThread.getBody() + "\"\r\n");
}

void Create::executeReply(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& team = server.getDb().findTeam(context.teamUuid);
  const auto& channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  const auto& thread =
      server.getDb().findThread(context.channelUuid, context.threadUuid);
  if (team == nullptr || channel == nullptr || thread == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 2) {
    return;
  }
  const std::string& replyMessage = args[1];
  auto& replies = thread->getReplies();
  Reply newReply;
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_STR_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newReply.setUuid(std::string(uuidStr.data()));
  newReply.setBody(replyMessage);
  newReply.setTimestamp(
      std::chrono::system_clock::now().time_since_epoch().count());
  replies.push_back(newReply);
  server_event_reply_created(context.threadUuid.c_str(),
                             client.getUserUuid().c_str(),
                             newReply.getBody().c_str());
  server.notifySubscribers(
      context.teamUuid, "reply_created \"" + context.teamUuid + "\" \"" +
                            context.channelUuid + "\" \"" + context.threadUuid +
                            "\" \"" + newReply.getUuid() + "\" \"" +
                            newReply.getBody() + "\"\r\n");
}
