/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** create
*/

#include "create.hpp"
#include <uuid/uuid.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <ctime>
#include <string>
#include <vector>
#include "../../../libs/myteams/logging_server.h"
#include "client/client.hpp"
#include "models/Channel.hpp"
#include "models/Reply.hpp"
#include "models/Team.hpp"
#include "models/Thread.hpp"
#include "server.hpp"
#include "utils.hpp"

static constexpr std::size_t UUID_LEN = 37;
static constexpr std::size_t MAX_NAME_LENGTH = 32;
static constexpr std::size_t MAX_DESCRIPTION_LENGTH = 255;
static constexpr std::size_t MAX_BODY_LENGTH = 512;

namespace {
bool isInvalidLength(const std::string& value, std::size_t maxLength) {
  return value.empty() || value.size() > maxLength;
}

void sendBadRequest(Client& client) {
  Server::sendToClient("400 BAD_REQUEST\r\n", client);
}

void sendAlreadyExists(Client& client) {
  Server::sendToClient("409 ALREADY_EXISTS\r\n", client);
}

std::string generateUuid() {
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  return {uuidStr.data()};
}

bool ensureArgsCount(const std::vector<std::string>& args,
                     std::size_t minimumCount, Client& client) {
  if (args.size() < minimumCount) {
    sendBadRequest(client);
    return false;
  }
  return true;
}

Team* findAccessibleTeam(Client& client, Server& server,
                         const std::string& teamUuid) {
  Team* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND TEAM " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return nullptr;
  }
  if (!team->isUserSubscribed(client.getUserUuid())) {
    Server::sendToClient("403 FORBIDDEN\r\n", client);
    return nullptr;
  }
  return team;
}

Channel* findChannelOrSendNotFound(Server& server, const std::string& teamUuid,
                                   const std::string& channelUuid,
                                   Client& client) {
  Channel* channel = server.getDb().findChannel(teamUuid, channelUuid);
  if (channel != nullptr) {
    return channel;
  }
  Server::sendToClient("404 NOT_FOUND CHANNEL " +
                           Utils::quoteProtocolField(channelUuid) + "\r\n",
                       client);
  return nullptr;
}

Thread* findThreadOrSendNotFound(Server& server, const std::string& channelUuid,
                                 const std::string& threadUuid,
                                 Client& client) {
  Thread* thread = server.getDb().findThread(channelUuid, threadUuid);
  if (thread != nullptr) {
    return thread;
  }
  Server::sendToClient(
      "404 NOT_FOUND THREAD " + Utils::quoteProtocolField(threadUuid) + "\r\n",
      client);
  return nullptr;
}

bool hasTeamWithName(const std::vector<Team>& teams, const std::string& name) {
  return std::ranges::any_of(
      teams, [&name](const Team& team) { return team.getName() == name; });
}

bool hasChannelWithName(const std::vector<Channel>& channels,
                        const std::string& name) {
  return std::ranges::any_of(channels, [&name](const Channel& channel) {
    return channel.getName() == name;
  });
}

bool hasThreadWithTitle(const std::vector<Thread>& threads,
                        const std::string& title) {
  return std::ranges::any_of(threads, [&title](const Thread& thread) {
    return thread.getTitle() == title;
  });
}

Team buildTeam(const std::string& name, const std::string& description) {
  Team team;
  team.setUuid(generateUuid());
  team.setName(name);
  team.setDescription(description);
  return team;
}

Channel& addChannel(Team& team, const std::string& name,
                    const std::string& description) {
  auto& channels = team.getChannels();
  channels.emplace_back();
  Channel& channel = channels.back();
  channel.setUuid(generateUuid());
  channel.setName(name);
  channel.setDescription(description);
  return channel;
}

Thread buildThread(const std::string& userUuid, const std::string& title,
                   const std::string& body) {
  Thread thread;
  thread.setUuid(generateUuid());
  thread.setUserUuid(userUuid);
  thread.setTitle(title);
  thread.setBody(body);
  thread.setTimestamp(std::time(nullptr));
  return thread;
}

Reply buildReply(const std::string& userUuid, const std::string& body) {
  Reply reply;
  reply.setUuid(generateUuid());
  reply.setUserUuid(userUuid);
  reply.setBody(body);
  reply.setTimestamp(std::time(nullptr));
  return reply;
}

void sendTeamCreated(Client& client, const Team& team) {
  Server::sendToClient(
      "201 CREATED TEAM " + Utils::quoteProtocolField(team.getUuid()) + " " +
          Utils::quoteProtocolField(team.getName()) + " " +
          Utils::quoteProtocolField(team.getDescription()) + "\r\n",
      client);
}

void sendChannelCreated(Client& client, const Channel& channel) {
  Server::sendToClient(
      "201 CREATED CHANNEL " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n",
      client);
}

void sendThreadCreated(Client& client, const Thread& thread) {
  Server::sendToClient("201 CREATED THREAD " +
                           Utils::quoteProtocolField(thread.getUuid()) + " " +
                           Utils::quoteProtocolField(thread.getUserUuid()) +
                           " " + std::to_string(thread.getTimestamp()) + " " +
                           Utils::quoteProtocolField(thread.getTitle()) + " " +
                           Utils::quoteProtocolField(thread.getBody()) + "\r\n",
                       client);
}

void sendReplyCreated(Client& client, const std::string& threadUuid,
                      const Reply& reply) {
  Server::sendToClient("201 CREATED REPLY " +
                           Utils::quoteProtocolField(threadUuid) + " " +
                           Utils::quoteProtocolField(reply.getUserUuid()) +
                           " " + std::to_string(reply.getTimestamp()) + " " +
                           Utils::quoteProtocolField(reply.getBody()) + "\r\n",
                       client);
}

void logAndBroadcastTeamCreated(Server& server, const Team& team,
                                const std::string& creatorUuid) {
  server_event_team_created(team.getUuid().c_str(), team.getName().c_str(),
                            creatorUuid.c_str());
  server.broadcast("EVENT TEAM_CREATED " +
                   Utils::quoteProtocolField(team.getUuid()) + " " +
                   Utils::quoteProtocolField(team.getName()) + " " +
                   Utils::quoteProtocolField(team.getDescription()) + "\r\n");
}

void logAndNotifyChannelCreated(Server& server, const std::string& teamUuid,
                                const Channel& channel) {
  server_event_channel_created(teamUuid.c_str(), channel.getUuid().c_str(),
                               channel.getName().c_str());
  server.notifySubscribers(
      teamUuid,
      "EVENT CHANNEL_CREATED " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n");
}

void logAndNotifyThreadCreated(Server& server, const std::string& teamUuid,
                               const std::string& channelUuid,
                               const Thread& thread,
                               const std::string& userUuid) {
  server_event_thread_created(channelUuid.c_str(), thread.getUuid().c_str(),
                              userUuid.c_str(), thread.getTitle().c_str(),
                              thread.getBody().c_str());
  server.notifySubscribers(
      teamUuid, "EVENT THREAD_CREATED " +
                    Utils::quoteProtocolField(thread.getUuid()) + " " +
                    Utils::quoteProtocolField(thread.getUserUuid()) + " " +
                    std::to_string(thread.getTimestamp()) + " " +
                    Utils::quoteProtocolField(thread.getTitle()) + " " +
                    Utils::quoteProtocolField(thread.getBody()) + "\r\n");
}

void logAndNotifyReplyCreated(Server& server, const std::string& teamUuid,
                              const std::string& threadUuid, const Reply& reply,
                              const std::string& userUuid) {
  server_event_reply_created(threadUuid.c_str(), userUuid.c_str(),
                             reply.getBody().c_str());
  server.notifySubscribers(
      teamUuid, "EVENT REPLY_CREATED " + Utils::quoteProtocolField(teamUuid) +
                    " " + Utils::quoteProtocolField(threadUuid) + " " +
                    Utils::quoteProtocolField(reply.getUserUuid()) + " " +
                    Utils::quoteProtocolField(reply.getBody()) + "\r\n");
}
}  // namespace

Create::~Create() = default;

void Create::execute(Client& client, Server& server) {
  const auto& context = client.getContext();
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
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
  const auto& args = client.getArgs();
  if (!ensureArgsCount(args, 3, client)) {
    return;
  }
  const std::string& teamName = args[1];
  const std::string& teamDescription = args[2];
  if (isInvalidLength(teamName, MAX_NAME_LENGTH) ||
      isInvalidLength(teamDescription, MAX_DESCRIPTION_LENGTH)) {
    sendBadRequest(client);
    return;
  }
  auto& teams = server.getDb().getTeams();
  if (hasTeamWithName(teams, teamName)) {
    sendAlreadyExists(client);
    return;
  }
  Team newTeam = buildTeam(teamName, teamDescription);
  teams.push_back(newTeam);
  sendTeamCreated(client, newTeam);
  logAndBroadcastTeamCreated(server, newTeam, client.getUserUuid());
}

void Create::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  const auto& args = client.getArgs();
  if (!ensureArgsCount(args, 3, client)) {
    return;
  }
  const std::string& channelName = args[1];
  const std::string& channelDescription = args[2];
  if (isInvalidLength(channelName, MAX_NAME_LENGTH) ||
      isInvalidLength(channelDescription, MAX_DESCRIPTION_LENGTH)) {
    sendBadRequest(client);
    return;
  }
  if (hasChannelWithName(team->getChannels(), channelName)) {
    sendAlreadyExists(client);
    return;
  }
  Channel& channel = addChannel(*team, channelName, channelDescription);
  sendChannelCreated(client, channel);
  logAndNotifyChannelCreated(server, context.teamUuid, channel);
}

void Create::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  Channel* channel = findChannelOrSendNotFound(server, context.teamUuid,
                                               context.channelUuid, client);
  if (channel == nullptr) {
    return;
  }
  const auto& args = client.getArgs();
  if (!ensureArgsCount(args, 3, client)) {
    return;
  }
  const std::string& threadTitle = args[1];
  const std::string& threadMessage = args[2];
  if (isInvalidLength(threadTitle, MAX_NAME_LENGTH) ||
      isInvalidLength(threadMessage, MAX_BODY_LENGTH)) {
    sendBadRequest(client);
    return;
  }
  auto& threads = channel->getThreads();
  if (hasThreadWithTitle(threads, threadTitle)) {
    sendAlreadyExists(client);
    return;
  }
  Thread newThread =
      buildThread(client.getUserUuid(), threadTitle, threadMessage);
  threads.push_back(newThread);
  sendThreadCreated(client, newThread);
  logAndNotifyThreadCreated(server, context.teamUuid, context.channelUuid,
                            newThread, client.getUserUuid());
}

void Create::executeReply(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  Channel* channel = findChannelOrSendNotFound(server, context.teamUuid,
                                               context.channelUuid, client);
  if (channel == nullptr) {
    return;
  }
  Thread* thread = findThreadOrSendNotFound(server, context.channelUuid,
                                            context.threadUuid, client);
  if (thread == nullptr) {
    return;
  }
  const auto& args = client.getArgs();
  if (!ensureArgsCount(args, 2, client)) {
    return;
  }
  const std::string& replyMessage = args[1];
  if (isInvalidLength(replyMessage, MAX_BODY_LENGTH)) {
    sendBadRequest(client);
    return;
  }
  auto& replies = thread->getReplies();
  Reply newReply = buildReply(client.getUserUuid(), replyMessage);
  replies.push_back(newReply);
  sendReplyCreated(client, context.threadUuid, newReply);
  logAndNotifyReplyCreated(server, context.teamUuid, context.threadUuid,
                           newReply, client.getUserUuid());
}
