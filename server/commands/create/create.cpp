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
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newTeam.setUuid(std::string(uuidStr.data()));
  newTeam.setName(teamName);
  newTeam.setDescription(teamDescription);
  if (!client.getUserUuid().empty()) {
    (void)newTeam.addSubscriber(client.getUserUuid());
  }
  teams.push_back(newTeam);

  Server::sendToClient(
      "201 CREATED TEAM " + Utils::quoteProtocolField(newTeam.getUuid()) + " " +
          Utils::quoteProtocolField(newTeam.getName()) + " " +
          Utils::quoteProtocolField(newTeam.getDescription()) + "\r\n",
      client);
  server_event_team_created(newTeam.getUuid().c_str(),
                            newTeam.getName().c_str(),
                            client.getUserUuid().c_str());
  server.broadcast(
      "EVENT TEAM_CREATED " + Utils::quoteProtocolField(newTeam.getUuid()) +
      " " + Utils::quoteProtocolField(newTeam.getName()) + " " +
      Utils::quoteProtocolField(newTeam.getDescription()) + "\r\n");
}

void Create::executeChannel(Client& client, Server& server) {
  const auto& context = client.getContext();
  Team* team = server.getDb().findTeam(context.teamUuid);
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
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  channel.setUuid(std::string(uuidStr.data()));
  channel.setName(channelName);
  channel.setDescription(channelDescription);

  Server::sendToClient(
      "201 CREATED CHANNEL " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n",
      client);
  server_event_channel_created(context.teamUuid.c_str(),
                               channel.getUuid().c_str(),
                               channel.getName().c_str());
  server.notifySubscribers(
      context.teamUuid,
      "EVENT CHANNEL_CREATED " + Utils::quoteProtocolField(channel.getUuid()) +
          " " + Utils::quoteProtocolField(channel.getName()) + " " +
          Utils::quoteProtocolField(channel.getDescription()) + "\r\n");
}

void Create::executeThread(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& team = server.getDb().findTeam(context.teamUuid);
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
  const auto& channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND CHANNEL " +
                             Utils::quoteProtocolField(context.channelUuid) +
                             "\r\n",
                         client);
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
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newThread.setUuid(std::string(uuidStr.data()));
  newThread.setUserUuid(client.getUserUuid());
  newThread.setTitle(threadTitle);
  newThread.setBody(threadMessage);
  newThread.setTimestamp(std::time(nullptr));
  threads.push_back(newThread);

  Server::sendToClient(
      "201 CREATED THREAD " + Utils::quoteProtocolField(newThread.getUuid()) +
          " " + Utils::quoteProtocolField(newThread.getUserUuid()) + " " +
          std::to_string(newThread.getTimestamp()) + " " +
          Utils::quoteProtocolField(newThread.getTitle()) + " " +
          Utils::quoteProtocolField(newThread.getBody()) + "\r\n",
      client);
  server_event_thread_created(
      context.channelUuid.c_str(), newThread.getUuid().c_str(),
      client.getUserUuid().c_str(), newThread.getTitle().c_str(),
      newThread.getBody().c_str());
  server.notifySubscribers(
      context.teamUuid,
      "EVENT THREAD_CREATED " + Utils::quoteProtocolField(newThread.getUuid()) +
          " " + Utils::quoteProtocolField(newThread.getUserUuid()) + " " +
          std::to_string(newThread.getTimestamp()) + " " +
          Utils::quoteProtocolField(newThread.getTitle()) + " " +
          Utils::quoteProtocolField(newThread.getBody()) + "\r\n");
}

void Create::executeReply(Client& client, Server& server) {
  const auto& context = client.getContext();
  const auto& team = server.getDb().findTeam(context.teamUuid);
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
  const auto& channel =
      server.getDb().findChannel(context.teamUuid, context.channelUuid);
  if (channel == nullptr) {
    Server::sendToClient("404 NOT_FOUND CHANNEL " +
                             Utils::quoteProtocolField(context.channelUuid) +
                             "\r\n",
                         client);
    return;
  }
  const auto& thread =
      server.getDb().findThread(context.channelUuid, context.threadUuid);
  if (thread == nullptr) {
    Server::sendToClient("404 NOT_FOUND THREAD " +
                             Utils::quoteProtocolField(context.threadUuid) +
                             "\r\n",
                         client);
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
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newReply.setUuid(std::string(uuidStr.data()));
  newReply.setUserUuid(client.getUserUuid());
  newReply.setBody(replyMessage);
  newReply.setTimestamp(std::time(nullptr));
  replies.push_back(newReply);

  Server::sendToClient(
      "201 CREATED REPLY " + Utils::quoteProtocolField(context.threadUuid) +
          " " + Utils::quoteProtocolField(newReply.getUserUuid()) + " " +
          std::to_string(newReply.getTimestamp()) + " " +
          Utils::quoteProtocolField(newReply.getBody()) + "\r\n",
      client);
  server_event_reply_created(context.threadUuid.c_str(),
                             client.getUserUuid().c_str(),
                             newReply.getBody().c_str());
  server.notifySubscribers(
      context.teamUuid,
      "EVENT REPLY_CREATED " + Utils::quoteProtocolField(context.teamUuid) +
          " " + Utils::quoteProtocolField(context.threadUuid) + " " +
          Utils::quoteProtocolField(newReply.getUserUuid()) + " " +
          Utils::quoteProtocolField(newReply.getBody()) + "\r\n");
}
