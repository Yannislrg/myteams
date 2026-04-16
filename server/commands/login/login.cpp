/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** login
*/

#include "login.hpp"
#include <uuid/uuid.h>
#include <array>
#include "User.hpp"
#include "client.hpp"
#include "database/Database.hpp"
#include "logging_server.h"
#include "server.hpp"

static constexpr std::size_t UUID_LEN = 37;
static constexpr std::size_t MAX_NAME_LENGTH = 32;

namespace {
void createNewUser(Client& client, Server& server,
                   const std::string& username) {
  User newUser;
  newUser.setName(username);
  newUser.setConnected(true);
  uuid_t uuidObj;  // NOLINT(misc-include-cleaner)
  uuid_generate(uuidObj);
  std::array<char, UUID_LEN> uuidStr{};
  uuid_unparse_lower(uuidObj, uuidStr.data());
  newUser.setUuid(std::string(uuidStr.data()));
  server.getDb().getUsers().push_back(newUser);
  client.setUserUuid(newUser.getUuid());
  Server::sendToClient(
      "200 OK \"" + newUser.getUuid() + "\" \"" + newUser.getName() + "\"\r\n",
      client);
  server_event_user_created(newUser.getUuid().c_str(),
                            newUser.getName().c_str());
  server_event_user_logged_in(newUser.getUuid().c_str());
  server.broadcast("EVENT USER_LOGGED_IN \"" + newUser.getUuid() + "\" \"" +
                   newUser.getName() + "\"\r\n");
}

void logUserIn(Client& client, Server& server, const std::string& username) {
  auto* user = server.getDb().findUserByName(username);
  if (user == nullptr) {
    Server::sendToClient("404 NOT_FOUND\r\n", client);
    return;
  }
  if (user->isConnected()) {
    Server::sendToClient("403 FORBIDDEN\r\n", client);
    return;
  }
  user->setConnected(true);
  client.setUserUuid(user->getUuid());
  Server::sendToClient(
      "200 OK \"" + user->getUuid() + "\" \"" + user->getName() + "\"\r\n",
      client);
  server_event_user_logged_in(user->getUuid().c_str());
  server.broadcast("EVENT USER_LOGGED_IN \"" + user->getUuid() + "\" \"" +
                   user->getName() + "\"\r\n");
}
}  // namespace

void Login::execute(Client& client, Server& server) {
  const auto& args = client.getArgs();
  if (args.size() < 2) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  if (!client.getUserUuid().empty()) {
    Server::sendToClient("401 UNAUTHORIZED\r\n", client);
    return;
  }
  const std::string& username = args[1];
  if (username.empty() || username.size() > MAX_NAME_LENGTH) {
    Server::sendToClient("400 BAD_REQUEST\r\n", client);
    return;
  }
  if (server.getDb().findUserByName(username) == nullptr) {
    createNewUser(client, server, username);
    return;
  }
  logUserIn(client, server, username);
}
