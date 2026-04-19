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

std::string generateUuid() {
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  return {uuidStr.data()};
}

Team* findAccessibleTeam(Client& client, Server& server,
                         const std::string& teamUuid) {
  Team* team = server.getDb().findTeam(teamUuid);
  if (team == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND " + Utils::quoteProtocolField(teamUuid) + "\r\n",
        client);
    return nullptr;
  }
  if (!team->isUserSubscribed(client.getUserUuid())) {
    Server::sendToClient("403 FORBIDDEN\r\n", client);
    return nullptr;
  }
  return team;
}

Channel* findChannelOrSend404(Client& client, Server& server,
                              const std::string& teamUuid,
                              const std::string& channelUuid) {
  Channel* channel = server.getDb().findChannel(teamUuid, channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND " +
                             Utils::quoteProtocolField(channelUuid) + "\r\n",
                         client);
    return nullptr;
  }
  return channel;
}

Thread* findThreadOrSend404(Client& client, Server& server,
                            const std::string& channelUuid,
                            const std::string& threadUuid) {
  Thread* thread = server.getDb().findThread(channelUuid, threadUuid);
  if (thread == nullptr) {
    Server::sendToClient("404 NOT_FOUND " +
                             Utils::quoteProtocolField(threadUuid) + "\r\n",
                         client);
    return nullptr;
  }
  return thread;
}

void emitTeamCreated(Client& client, Server& server, const Team& team) {
  Server::sendToClient(
      "201 CREATED TEAM " + Utils::quoteProtocolField(team.getUuid()) + " " +
          Utils::quoteProtocolField(team.getName()) + " " +
          Utils::quoteProtocolField(team.getDescription()) + "\r\n",
      client);
  server_event_team_created(team.getUuid().c_str(), team.getName().c_str(),
                            client.getUserUuid().c_str());
  server.broadcast("EVENT TEAM_CREATED " +
                   Utils::quoteProtocolField(team.getUuid()) + " " +
                   Utils::quoteProtocolField(team.getName()) + " " +
                   Utils::quoteProtocolField(team.getDescription()) + "\r\n");
}

void emitChannelCreated(Client& client, Server& server,
                        const std::string& teamUuid, const Channel& channel) {
  Server::sendToClient(
      "201 CREATED CHANNEL " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n",
      client);
  server_event_channel_created(teamUuid.c_str(), channel.getUuid().c_str(),
                               channel.getName().c_str());
  server.notifySubscribers(
      teamUuid,
      "EVENT CHANNEL_CREATED " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n");
}

void emitThreadCreated(Client& client, Server& server,
                       const std::string& teamUuid,
                       const std::string& channelUuid, const Thread& thread) {
  Server::sendToClient("201 CREATED THREAD " +
                           Utils::quoteProtocolField(thread.getUuid()) + " " +
                           Utils::quoteProtocolField(thread.getUserUuid()) +
                           " " + std::to_string(thread.getTimestamp()) + " " +
                           Utils::quoteProtocolField(thread.getTitle()) + " " +
                           Utils::quoteProtocolField(thread.getBody()) + "\r\n",
                       client);
  server_event_thread_created(channelUuid.c_str(), thread.getUuid().c_str(),
                              client.getUserUuid().c_str(),
                              thread.getTitle().c_str(),
                              thread.getBody().c_str());
  server.notifySubscribers(
      teamUuid, "EVENT THREAD_CREATED " +
                    Utils::quoteProtocolField(thread.getUuid()) + " " +
                    Utils::quoteProtocolField(thread.getUserUuid()) + " " +
                    std::to_string(thread.getTimestamp()) + " " +
                    Utils::quoteProtocolField(thread.getTitle()) + " " +
                    Utils::quoteProtocolField(thread.getBody()) + "\r\n");
}

void emitReplyCreated(Client& client, Server& server,
                      const std::string& teamUuid,
                      const std::string& threadUuid, const Reply& reply) {
  Server::sendToClient("201 CREATED REPLY " +
                           Utils::quoteProtocolField(threadUuid) + " " +
                           Utils::quoteProtocolField(reply.getUserUuid()) +
                           " " + std::to_string(reply.getTimestamp()) + " " +
                           Utils::quoteProtocolField(reply.getBody()) + "\r\n",
                       client);
  server_event_reply_created(threadUuid.c_str(), client.getUserUuid().c_str(),
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
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const std::string& teamName = args[1];
  const std::string& teamDescription = args[2];
  if (isInvalidLength(teamName, MAX_NAME_LENGTH) ||
      isInvalidLength(teamDescription, MAX_DESCRIPTION_LENGTH)) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  auto& teams = server.getDb().getTeams();
  for (const auto& team : teams) {
    if (team.getName() == teamName) {
      Server::sendToClient("409 ALREADY_EXISTS\r\n", client);
      return;
    }
  }
  Team newTeam;
  newTeam.setUuid(generateUuid());
  newTeam.setName(teamName);
  newTeam.setDescription(teamDescription);
  if (!client.getUserUuid().empty()) {
    (void)newTeam.addSubscriber(client.getUserUuid());
  }
  teams.push_back(newTeam);

  emitTeamCreated(client, server, newTeam);
}

void Create::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const std::string& channelName = args[1];
  const std::string& channelDescription = args[2];
  if (isInvalidLength(channelName, MAX_NAME_LENGTH) ||
      isInvalidLength(channelDescription, MAX_DESCRIPTION_LENGTH)) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }

  auto& channels = team->getChannels();
  for (const auto& channel : channels) {
    if (channel.getName() == channelName) {
      Server::sendToClient("409 ALREADY_EXISTS\r\n", client);
      return;
    }
  }
  channels.emplace_back();
  Channel& channel = channels.back();
  channel.setUuid(generateUuid());
  channel.setName(channelName);
  channel.setDescription(channelDescription);

  emitChannelCreated(client, server, context.teamUuid, channel);
}

void Create::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  Channel* channel = findChannelOrSend404(client, server, context.teamUuid,
                                          context.channelUuid);
  if (channel == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 3) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const std::string& threadTitle = args[1];
  const std::string& threadMessage = args[2];
  if (isInvalidLength(threadTitle, MAX_NAME_LENGTH) ||
      isInvalidLength(threadMessage, MAX_BODY_LENGTH)) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  auto& threads = channel->getThreads();
  for (const auto& thread : threads) {
    if (thread.getTitle() == threadTitle) {
      Server::sendToClient("409 ALREADY_EXISTS\r\n", client);
      return;
    }
  }
  Thread newThread;
  newThread.setUuid(generateUuid());
  newThread.setUserUuid(client.getUserUuid());
  newThread.setTitle(threadTitle);
  newThread.setBody(threadMessage);
  newThread.setTimestamp(std::time(nullptr));
  threads.push_back(newThread);

  emitThreadCreated(client, server, context.teamUuid, context.channelUuid,
                    newThread);
}

void Create::executeReply(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = findAccessibleTeam(client, server, context.teamUuid);
  if (team == nullptr) {
    return;
  }
  Channel* channel = findChannelOrSend404(client, server, context.teamUuid,
                                          context.channelUuid);
  if (channel == nullptr) {
    return;
  }
  Thread* thread = findThreadOrSend404(client, server, context.channelUuid,
                                       context.threadUuid);
  if (thread == nullptr) {
    return;
  }
  const std::vector<std::string>& args = client.getArgs();
  if (args.size() < 2) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const std::string& replyMessage = args[1];
  if (isInvalidLength(replyMessage, MAX_BODY_LENGTH)) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  auto& replies = thread->getReplies();
  Reply newReply;
  newReply.setUuid(generateUuid());
  newReply.setUserUuid(client.getUserUuid());
  newReply.setBody(replyMessage);
  newReply.setTimestamp(std::time(nullptr));
  replies.push_back(newReply);

  emitReplyCreated(client, server, context.teamUuid, context.threadUuid,
                   newReply);
}
