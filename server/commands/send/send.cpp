/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** send
*/

#include "send.hpp"
#include <ctime>
#include "client.hpp"
#include "database/Database.hpp"
#include "logging_server.h"
#include "models/Message.hpp"
#include "models/User.hpp"
#include "server.hpp"
#include "utils.hpp"

static constexpr std::size_t MAX_BODY_LENGTH = 512;

void Send::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (args.size() < 3) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  if (args[1].empty() || args[2].empty() || args[2].size() > MAX_BODY_LENGTH) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  const auto& senderUser = server.getDb().findUser(client.getUserUuid());
  if (senderUser == nullptr) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const auto& receiverUser = server.getDb().findUser(args[1]);
  if (receiverUser == nullptr) {
    Server::sendToClient(
        "404 NOT_FOUND USER " + Utils::quoteProtocolField(args[1]) + "\r\n",
        client);
    return;
  }
  const auto& messageBody = args[2];
  Message message;
  message.setSenderUuid(senderUser->getUuid());
  message.setReceiverUuid(receiverUser->getUuid());
  message.setTimestamp(std::time(nullptr));
  message.setBody(messageBody);
  senderUser->addMessage(receiverUser->getUuid(), message);
  receiverUser->addMessage(senderUser->getUuid(), message);
  server_event_private_message_sended(senderUser->getUuid().c_str(),
                                      receiverUser->getUuid().c_str(),
                                      messageBody.c_str());
  server.sendToUser(receiverUser->getUuid(),
                    "EVENT PRIVATE_MESSAGE " +
                        Utils::quoteProtocolField(senderUser->getUuid()) + " " +
                        Utils::quoteProtocolField(messageBody) + "\r\n");
  Server::sendToClient("200 OK\r\n", client);
}
