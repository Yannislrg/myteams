/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** messages
*/

#include "messages.hpp"
#include "client.hpp"
#include "database/Database.hpp"
#include "models/User.hpp"
#include "server.hpp"
#include "utils.hpp"

namespace {
void sendMessageLine(Client& client, const Message& message) {
  Server::sendToClient(
      "210 " + Utils::quoteProtocolField(message.getSenderUuid()) + " " +
          std::to_string(message.getTimestamp()) + " " +
          Utils::quoteProtocolField(message.getBody()) + "\r\n",
      client);
}
}  // namespace

void Messages::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  if (args.size() < 2) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const auto& senderUser = server.getDb().findUser(client.getUserUuid());
  const auto& user_uuid = args[1];
  const auto& receiverUser = server.getDb().findUser(user_uuid);
  if (receiverUser == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND " + Utils::quoteProtocolField(user_uuid) + "\r\n",
        client);
    return;
  }
  const auto& messages = senderUser->getMessages();
  auto messageIterator = messages.find(user_uuid);
  if (messageIterator == messages.end()) {
    Server::sendToClient("210-BEGIN MESSAGES\r\n", client);
    Server::sendToClient("210-END MESSAGES\r\n", client);
    return;
  }
  Server::sendToClient("210-BEGIN MESSAGES\r\n", client);
  for (const auto& message : messageIterator->second) {
    sendMessageLine(client, message);
  }
  Server::sendToClient("210-END MESSAGES\r\n", client);
}
