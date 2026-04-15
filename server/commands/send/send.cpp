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
#include "models/Message.hpp"
#include "models/User.hpp"
#include "server.hpp"
#include "utils.hpp"

void Send::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (args.size() < 3) {
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
        "404 NOT_FOUND " + Utils::quoteProtocolField(args[1]) + "\r\n", client);
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
  Server::sendToClient("200 OK\r\n", client);
}
